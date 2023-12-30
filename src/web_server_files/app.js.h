const char app_js[] PROGMEM = R"rawliteral(
CHANGE_INPUT_DELAY_MS = 50

const reqParams = {mode: "cors", cache: "no-cache", credentials: "same-origin", referrerPolicy: "no-referrer", headers: {}}

class API {
    $channelsDelays = {}
    constructor(url = document.URL) {
        this.url = new URL(url)
    }

    async writeBrightToChannelDelay(channel = 1, value = 0) {
        let lastChange = this.$channelsDelays[channel] ? this.$channelsDelays[channel] : 0
        if (Date.now() - lastChange < CHANGE_INPUT_DELAY_MS) return;
        this.$channelsDelays[channel] = Date.now()
        return this.writeBrightToChannel(channel, value)
    }

    async writeBrightToChannel(channel = 1, value = 0) {
        let buffer = new ArrayBuffer(2)
        let view = new Uint8Array(buffer)
        view.set([channel, value])
        const response = await fetch(this.url + "updateChannel", {method: "POST", body: buffer, ...reqParams});
        if (response.status) this.$channelsDelays[channel] = Date.now()
    }

    async getBrightChannels() {
        const response = await fetch(this.url + "getChannelsBright", {method: "GET", ...reqParams});
        if (response.status !== 200) return console.error(response)
        let buffer = await response.arrayBuffer()
        return new Uint8Array(buffer)
    }

    async changeMode(source, mode) {
        let buffer = new ArrayBuffer(2)
        let view = new Uint8Array(buffer)
        view.set([source, mode])
        const response = await fetch(this.url + "changeMode", {method: "POST", body: buffer, ...reqParams});
    }

    async getSources() {
        const response = await fetch(this.url + "getSources", {method: "GET", ...reqParams});
        if (response.status !== 200) return console.error(response)
        let buffer = await response.arrayBuffer()
        let view = new Uint8Array(buffer)
        let sources = []
        let source = {name: ""}
        let index = 0
        let state = 0
        while (index <= view.length) {
            if (state === 0) {
                source.mode = view.at(index++)
                state += 1
            } else if (state === 1) {
                for (let i = view.at(index++); i > 0; i--) source.name += String.fromCharCode(view.at(index++))
                state += 1
            } else {
                state = 0
                sources.push(source)
                source = {name: ""}
            }
        }
        return sources
    }
}

class UI {
    get_input_for_section = (sourceId, defaultValue) => {
        let select = document.createElement("select")
        select.setAttribute("name", sourceId)
        let opt = document.createElement("option")
        for (let i = 0; i < 4; i++) {
            opt.value = i
            opt.innerHTML = !i ? "OFF" : i===1 ? "COLOR" : i===2 ? "FADE" : "SMOOTH"
            if (i === defaultValue) opt.setAttribute("selected", "")
            select.add(opt)
            opt = document.createElement("option")
        }
        return select
    }
}

class ESPApp {
    $api = new API()
    $ui = new UI()
    $mainNode = document.querySelector("main")

    constructor() {
        if (document.URL.substring("127.0.0.1") || document.URL.substring("localhost")) {
            document.title = "TEST FAIRY LIGHT"
            this.$api = new API("http://192.168.1.5/")
        }
        // this.$inputChannelsNodes.forEach((node) => {
        //     node.addEventListener("change", (e) => this.$api.writeBrightToChannelDelay(e.target.name, e.target.value))
        //     node.addEventListener("input", (e) => this.$api.writeBrightToChannelDelay(e.target.name, e.target.value))
        // })
    }

    async updateBrights() {
        let sources = await this.$api.getSources()
        console.log(sources)
        if (brights.length !== this.$inputChannelsNodes.length)
            return console.error("server channels length not equals client")
        this.$inputChannelsNodes.forEach((node, key) => node.value = brights[key])
    }

    async updateSources() {
        let sections = []
        let sources = await this.$api.getSources()
        sources.forEach((s, i) => {
            let sectionNode = document.createElement("section")
            let h1 = document.createElement("h3")
            h1.innerHTML = "Source: " + s.name
            sectionNode.appendChild(h1)
            let selectNode = this.$ui.get_input_for_section(i, s.mode)
            selectNode.addEventListener("change", (e) => {
                this.$api.changeMode(parseInt(e.target.name), parseInt(e.target.value));
            })
            sectionNode.appendChild(selectNode)
            sections.push(sectionNode)
        })
        this.$mainNode.replaceChildren(...sections)
    }

    async start() {
        // await this.updateBrights()

        await this.updateSources()
    }
}

app = new ESPApp()
document.addEventListener('DOMContentLoaded', () => app.start())
)rawliteral";
