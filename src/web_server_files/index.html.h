const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="ru">
  <head>
    <meta charset="utf-8">
    <title>Fairy Lights | Controller</title>
    <link rel="stylesheet" href="/styles.css">
  </head>
  <body>
    <div class="wrapper">
        <header>
            <h1>ESP Fairy Light Controller</h1>
            <p>The site is intended for managing light output sources.</p>
        </header>
        <main>
            <h3>Loading sources...</h3>
        </main>
    </div>
    <footer>
        <p>Developed by <a href="https://github.com/ArthurKoba" target="_blank">Arthur Koba</a> in 2023.</p>
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
        <header style="text-align: center;">
            <h1>Error 404</h1>
            <h3>Page not found!</h3>
            <nav>
                <ul>
                    <li><a href="/">Main page.</a></li>
                </ul>
            </nav>
        </header>
    </body>
</html>
)rawliteral";
