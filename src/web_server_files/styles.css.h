const char styles_css[] PROGMEM = R"rawliteral(

:root {
    --main-color: #000;
    --primary-color: #aaf;
    --interim-color: #777;
    --bg-color: #fff;
    --border-color: var(--primary-color);
}

@media (prefers-color-scheme: dark) {
    :root {
        --main-color: #fff;
        --primary-color: #2933bb;
        --bg-color: #333;
        --border-color: var(--main-color);
    }
}

* {
    margin: 0;
    padding: 0;
}

html, body, .wrapper{
    height: 100%;
}

body {
    background: var(--bg-color);
    color: var(--main-color);
    min-width: 320px;
}

.wrapper {
    height: 95vh;
}
main, header, footer {
    padding: 0.5rem;
}

footer {
    color: var(--interim-color);
    text-align: right;
    height: 2vh;
}
footer a {
    text-underline: none;
    color: #2933bb;
}

main>section {
    padding: .5rem;
    margin-top: .5rem;
    margin-right: 0;
    margin-left: 0;
    border: 1px solid var(--border-color);
    border-top-left-radius: .25rem;
    border-top-right-radius: .25rem;
}

main>section>select {
    width: 100%;
    padding: 0.375rem 2.25rem 0.375rem 0.75rem;
    border: 1px solid var(--border-color);
    font-size: 1rem;
    border-radius: 0.25rem;
    transition: border-color .15s ease-in-out,box-shadow .15s ease-in-out;
    background-color: rgba(0,0,0,0);
    color: var(--main-color);
}

main>section>select:focus {
    outline: 0;
    border-color: var(--border-color);
    box-shadow: 0 0 3pt 2pt var(--primary-color);
}

option {
    color: var(--main-color);
    background-color: var(--bg-color);
}

option:checked {
    background-color: var(--primary-color);
}

)rawliteral";