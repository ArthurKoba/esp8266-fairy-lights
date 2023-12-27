const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="ru">
  <head>
    <meta charset="utf-8">
    <title>ESP8266 Fairy Lights</title>
    <link rel="stylesheet" href="/styles.css">
  </head>
  <body>
    <header>
      <h1 class="text-white">Личный сайт</h1>
      <p>Который сделан на основе готового шаблона</p>
      <nav>
        <ul>
          <li><a href="/404.html">Эта страница</a></li>
        </ul>
      </nav>
    </header>
    <main>
      <section id="one">
        <h1>Light One - Channels</h1>
        <div>
          <input type="range" class="channel" name="0" min="0" max="255" value="0" step="1" />
          <label for="cowbell">Red</label>
        </div>
        <div>
          <input type="range" class="channel" name="1" min="0" max="255" value="0" step="1" />
          <label for="cowbell">Yellow</label>
        </div>
        <div>
          <input type="range" class="channel" name="2" min="0" max="255" value="0" step="1" />
          <label for="cowbell">Blue-Green</label>
        </div>
      </section>
      <section id="two">
        <h1>Light Two - Channels</h1>
        <div>
          <input type="range" class="channel" name="3" min="0" max="255" value="0" step="1" />
          <label for="cowbell">Red-Yellow</label>
        </div>
        <div>
          <input type="range" class="channel" name="4" min="0" max="255" value="0" step="1" />
          <label for="cowbell">Blue-Green</label>
        </div>
      </section>
    </main>
    <footer>
        <p>Сюда бы я вписал информацию об авторе и ссылки на другие сайты</p>
    </footer>
    <script src="/app.js" defer></script>
  </body>
</html>
)rawliteral";

const char not_found_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="ru">
  <head>
    <meta charset="utf-8">
    <title>Заголовок страницы</title>
    <link rel="stylesheet" href="/styles.css">
  </head>
  <body>
    <header>
      <h1 class="text-white">Error 404: Page not found!</h1>
      <nav>
        <ul>
          <li><a href="/">Main page.</a></li>
        </ul>
      </nav>
    </header>
  </body>
</html>
)rawliteral";
