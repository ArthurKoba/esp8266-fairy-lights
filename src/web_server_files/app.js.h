const char app_js[] PROGMEM = R"rawliteral(
const reqParams = {mode: "cors", cache: "no-cache", credentials: "same-origin", referrerPolicy: "no-referrer", headers: {}}
const emptySource = {name: "", mode: 0, channels: Array()}
const emptyChannel = {name: ""}
const m = {OFF_MODE: 0, COLOR: 1, FADE: 2, SMOOTH: 3}
const newTag = (tagName) => {return document.createElement(tagName)}
const getInput = (id, value) => {
    let i = newTag("input")
    i.setAttribute("type", "range")
    i.setAttribute("min", "0")
    i.setAttribute("max", "255")
    i.name = id
    i.value = value
    return i
}

class API {
    constructor(url = document.URL) {
        this.url = new URL(url)
    }

    async writeBrightToChannel(source, channel, value) {
        let buffer = new ArrayBuffer(3)
        let view = new Uint8Array(buffer)
        view.set([source, channel, value])
        const response = await fetch(this.url + "changeBright", {method: "POST", body: buffer, ...reqParams});
        if (response.status !== 200) return console.error(response)
    }


    async changeMode(source, mode) {
        let buffer = new ArrayBuffer(2)
        let view = new Uint8Array(buffer)
        view.set([source, mode])
        const response = await fetch(this.url + "changeMode", {method: "POST", body: buffer, ...reqParams});
        if (response.status !== 200) return console.error(response)
    }

    async getSourcesInfo() {
        const response = await fetch(this.url + "getSourcesInfo", {method: "GET", ...reqParams});
        if (response.status !== 200) return console.error(response)
        let buffer = await response.arrayBuffer()
        let view = new Uint8Array(buffer)
        let sources = []
        let source = {...emptySource}
        let channel = {...emptyChannel}
        let index = 0
        let state = 0
        let countSources = 0;
        let countChannels = 0;
        while (index <= view.length) {
            if (state === 0) { // count sources
                countSources = view.at(index++)
                state += 1
            } else if (state === 1) { // source mode
                source.mode = view.at(index++)
                state += 1
            } else if (state === 2) { // source count chars and name
                for (let i = view.at(index++); i > 0; i--) source.name += String.fromCharCode(view.at(index++))
                state += 1
            } else if (state === 3) { // count channels in source
                countChannels = view.at(index++)
                state += 1
            } else if (state === 4) { // channel bright
                channel.bright = view.at(index++)
                state += 1
            } else if (state === 5) { // channel count chars and name
                for (let i = view.at(index++); i > 0; i--) channel.name += String.fromCharCode(view.at(index++))
                source.channels.push(channel)
                channel = {...emptyChannel}
                countChannels -= 1
                if (countChannels > 0) {
                    state = 4
                    continue
                }
                countSources -= 1
                sources.push(source)
                source = {...emptySource, channels: Array()}
                if (countSources > 0) state = 1
                else break
            }
        }
        return sources
    }
}


class ESPApp {
    $api = new API()
    $mainNode = document.querySelector("main")
    $channelsDelays = {}

    constructor() {
        if (document.URL.includes("127.0.0.1") || document.URL.includes("localhost")) {
            document.title = "TEST | " + document.title
            this.$api = new API("http://192.168.1.5/")
        }
    }

    async writeBrightDelay(source, channel, value) {
        let id = source + channel
        if (!this.$channelsDelays[id]) this.$channelsDelays[id] = {busy: false}

        if (this.$channelsDelays[id].busy) {
            this.$channelsDelays[id].data = [source, channel, value]
            return;
        }
        this.$channelsDelays[id].busy = true
        await this.$api.writeBrightToChannel(source, channel, value)
        this.$channelsDelays[id].busy = false
    }

    get_input_for_section = (sourceId, defaultValue) => {
        let select = newTag("select")
        select.setAttribute("name", sourceId)
        let opt = newTag("option")
        for (let i = 0; i < 4; i++) {
            opt.value = i
            opt.innerHTML = !i ? "OFF" : i===1 ? "COLOR" : i===2 ? "FADE" : "SMOOTH"
            if (i === defaultValue) opt.setAttribute("selected", "")
            select.add(opt)
            opt = newTag("option")
        }
        return select
    }

    async updateSourcesInfo() {
        let sections = []
        let sources = await this.$api.getSourcesInfo()
        sources.forEach((s, i_s) => {
            let sectionNode = newTag("section")
            let h1 = newTag("h3")
            h1.innerHTML = "Source: " + s.name
            let selectNode = this.get_input_for_section(i_s, s.mode)
            let colorChangeNode = newTag("div")
            colorChangeNode.classList.add("bright")
            if (s.mode !== 1) colorChangeNode.classList.add("d-none")
            s.channels.forEach((c, i_c) => {
                let label = newTag("label")
                label.innerHTML = c.name;
                let input = getInput(i_c, c.bright)
                input.addEventListener("input", (e) => {
                    this.writeBrightDelay(i_s, e.target.name, e.target.value)
                })
                colorChangeNode.appendChild(label)
                colorChangeNode.appendChild(input)
            })
            selectNode.addEventListener("change", (e) => {
                let mode = parseInt(e.target.value)
                this.$api.changeMode(parseInt(e.target.name), mode);
                if (mode === m.COLOR) colorChangeNode.classList.remove("d-none")
                else colorChangeNode.classList.add("d-none")
            })
            sectionNode.replaceChildren(h1, selectNode, colorChangeNode)
            sections.push(sectionNode)
        })
        this.$mainNode.replaceChildren(...sections)
    }

    async loop() {
        while (true) {await new Promise(resolve => setTimeout(resolve, 10));
            for (let key of Object.keys(this.$channelsDelays)) {
                if (!this.$channelsDelays[key].data || !this.$channelsDelays[key].busy) return;
                this.$channelsDelays[key].busy = true
                await this.$api.writeBrightToChannel(...this.$channelsDelays[key].data)
                this.$channelsDelays[key].busy = false
                this.$channelsDelays[key].data = null
            }
        }
    }

    async start() {
        await this.updateSourcesInfo()
        this.loop()
    }
}

app = new ESPApp()
document.addEventListener('DOMContentLoaded', () => app.start())

)rawliteral";
