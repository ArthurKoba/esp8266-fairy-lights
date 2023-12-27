const char app_js[] PROGMEM = R"rawliteral(
CHANGE_INPUT_DELAY_MS = 50

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
        const response = await fetch(this.url + "updateChannel", {
            method: "POST", mode: "cors", cache: "no-cache",
            credentials: "same-origin", referrerPolicy: "no-referrer",
            headers: {}, body: buffer,
        });
        if (response.status) this.$channelsDelays[channel] = Date.now()
    }

    async getBrightChannels() {
        const response = await fetch(this.url + "getChannelsBright", {
            method: "GET", "mode": "cors", cache: "no-cache",
            credentials: "same-origin", referrerPolicy: "no-referrer", headers: {}
        });
        if (response.status !== 200) return console.error(response)
        let buffer = await response.arrayBuffer()
        return new Uint8Array(buffer)
    }
}

class ESPApp {
    $api = new API()
    $inputChannelsNodes = document.querySelectorAll("input.channel")
    constructor() {
        if (document.URL === "http://127.0.0.1:3000/") {
            document.title = "TEST FAIRY LIGHT"
            this.$api = new API("http://192.168.1.5/")
        }
        this.$inputChannelsNodes.forEach((node) => {node.addEventListener("change", (e) =>
            this.$api.writeBrightToChannelDelay(e.target.name, e.target.value))
        })
        this.$inputChannelsNodes.forEach((node) => {node.addEventListener("input", (e) =>
            this.$api.writeBrightToChannelDelay(e.target.name, e.target.value))
        })

    }

    async updateBrights() {
        let brights = await this.$api.getBrightChannels()
        if (brights.length !== this.$inputChannelsNodes.length)
            return console.error("server channels length not equals client")
        this.$inputChannelsNodes.forEach((node, key) => node.value = brights[key])
    }

    async start() {
        await this.updateBrights()
    }
}

app = new ESPApp()
document.addEventListener('DOMContentLoaded', () => app.start())
)rawliteral";
