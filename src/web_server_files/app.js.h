const char app_js[] PROGMEM = R"rawliteral(
CHANGE_INPUT_DELAY_MS = 50

class API {
    $channelsDelays = {}
    constructor(url = document.URL) {
        this.url = new URL(url)
    }

    async writeChannel(channel = 1, value = 0) {
        let buffer = new ArrayBuffer(2)
        let view = new Uint8Array(buffer)
        view.set([channel, value])
        const response = await fetch(this.url + "updateChannel", {
            method: "POST", mode: "cors", cache: "no-cache", credentials: "same-origin", referrerPolicy: "no-referrer",
            headers: {"Content-Type": "application/octet-stream"}, body: buffer,
        });
        if (response.status) this.$channelsDelays[channel] = Date.now()
    }

    async writeChannelDelay(channel = 1, value = 0) {
        let lastChange = this.$channelsDelays[channel] ? this.$channelsDelays[channel] : 0
        if (Date.now() - lastChange < CHANGE_INPUT_DELAY_MS) return;
        this.$channelsDelays[channel] = Date.now()
        return this.writeChannel(channel, value)
    }
}

class ESPApp {
    $api = new API()
    constructor() {
        if (document.URL === "http://127.0.0.1:3000/") {
            document.title = "TEST FAIRY LIGHT"
            this.$api = new API("http://192.168.1.5/")
        }
        document.querySelectorAll("input.channel").forEach((node) => {
            node.addEventListener("change", (e) => this.$api.writeChannel(e.target.name, e.target.value))
        })
        document.querySelectorAll("input.channel").forEach((node) => {
            node.addEventListener("input", (e) => this.$api.writeChannelDelay(e.target.name, e.target.value))
        })

    }

    async start() {

    }
}

app = new ESPApp()
document.addEventListener('DOMContentLoaded', () => app.start())
)rawliteral";
