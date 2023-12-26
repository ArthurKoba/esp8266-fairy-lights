const char styles_css[] PROGMEM = R"rawliteral(

input[type="range"] {
  -webkit-appearance: none;
  appearance: none;
  background: transparent;
  cursor: pointer;
  width: 15rem;
}

/***** Chrome, Safari, Opera, and Edge Chromium *****/
input[type="range"]::-webkit-slider-runnable-track {
  background: #053a5f;
  height: 0.5rem;
}

/******** Firefox ********/
input[type="range"]::-moz-range-track {
  background: #053a5f;
  height: 0.5rem;
}

)rawliteral";