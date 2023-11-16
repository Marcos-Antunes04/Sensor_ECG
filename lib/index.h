const char* html = R"rawliteral(
  <html>
    <head>
      <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/css/all.min.css">
      <script src='https://cdn.plot.ly/plotly-latest.min.js'></script>
    
    <style>
    html {
      font-family: Arial;
      display: inline-block;
      margin: 0px auto;
      text-align: center;
    }
    h1 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels {
      font-size: 1.5rem;
      vertical-align: middle;
      padding-bottom: 15px;
    }
  </style>
    
    </head>
    <body>
      <div >
      <h1>ESP32 ECG</h1>
      <p>
      <i class="fas fa-heartbeat heartbeat-icon" style="color:red"></i>
      </p>
      </div>
      <div id='chart'></div>
      <script>
        var xValues = [];
        var yValues = [];
        var startTime = new Date().getTime();

        function updateGraph() {
          fetch('/data')
            .then(response => response.json())
            .then(data => {
              var currentTime = new Date().getTime() - startTime;
              xValues.push(currentTime);
              yValues.push(data.value);

              // Manter apenas os últimos 10 segundos de dados
              var removeIndex = 0;
              for (var i = 0; i < xValues.length; i++) {
                if (xValues[i] < (currentTime - 10000)) {
                  removeIndex = i;
                } else {
                  break;
                }
              }
              if (removeIndex > 0) {
                xValues.splice(0, removeIndex);
                yValues.splice(0, removeIndex);
              }

              Plotly.newPlot('chart', [{x: xValues, y: yValues}], {title: 'Leitura Eletrocardiograma' , line:{color:'red'}});
              Plotly.restyle(chart, {line: {color: 'red'}});
            });
        }

        var graphUpdate = setInterval(updateGraph, 100);
      </script>
    </body>
  </html>
)rawliteral";