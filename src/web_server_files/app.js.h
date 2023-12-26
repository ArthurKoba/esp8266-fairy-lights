const char app_js[] PROGMEM = R"rawliteral(

  async function postData(channel = 0, value = 0) {
    let buffer = new ArrayBuffer(3)
    let view = new Uint8Array(buffer)
    view[0] = 50
    view[1] = channel
    view[2] = value
    const response = await fetch("/update", {
        method: "POST", mode: "cors", cache: "no-cache", credentials: "same-origin", referrerPolicy: "no-referrer",
        headers: {"Content-Type": "application/octet-stream"},
        body: buffer,
    });

  return response.arrayBuffer();
}

const watcher = (node) =>  postData(parseInt(node.name), parseInt(node.value))

document.querySelectorAll("input.channel").forEach((node) => node.addEventListener("input", (e) => watcher(e.target)))
)rawliteral";
