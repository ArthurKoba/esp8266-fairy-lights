const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="ru">
  <head>
    <meta charset="utf-8">
    <title>Заголовок страницы</title>
    <link rel="stylesheet" href="/styles.css">
  </head>
  <body>
    <header>
      <h1 class="text-white">Личный сайт</h1>
      <p>Который сделан на основе готового шаблона</p>
      <nav>
        <ul>
          <li><a href="/">Эта страница</a></li>
        </ul>
      </nav>
    </header>
    <main>
      <article>
        <section>
          <h2>Первая секция</h2>
          <p>Она обо мне</p>
          <p>Но может быть и о семантике, я пока не решил.</p>
        </section>
        <section>
          <h2>Вторая секция</h2>
          <button id="test" type="button">test click</button>
        </section>
        <section>
          <h2>И третья</h2>
          <p>Вы уже должны были начать догадываться.</p>
        </section>
      </article>
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
