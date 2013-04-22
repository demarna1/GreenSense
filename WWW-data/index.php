#!/opt/local/bin/php
<!DOCTYPE html>
<html>
  <head>
    <!-- Add cache -->
    <!-- meta http-equiv="cache-control" control="no-store" -->
    <title>GreenSense</title>
    <link type="text/css" href="css/style.css" rel="stylesheet"/>
    <script src="//ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
    <script src="http://code.highcharts.com/highcharts.js"></script>
    <script src="http://code.highcharts.com/modules/exporting.js"></script>
    <script>
      $(function () {
        $('#temp_graph').highcharts({
            chart: {
                zoomType: 'x',
                spacingRight: 20
            },
            title: {
                text: 'Greenhouse Temperature'
            },
	    subtitle: {
                text: 'Last 24 hours'
            },
            xAxis: {
                type: 'datetime',
                maxZoom: 1 * 24 * 3600000, //one day
                title: {
                    text: null
                }
            },
            yAxis: {
                title: {
                    text: 'Temp (in F)'
                }
            },
            tooltip: {
                shared: true
            },
            legend: {
                enabled: false
            },
            plotOptions: {
                area: {
                    fillColor: {
                        linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1},
                        stops: [
                            [0, Highcharts.getOptions().colors[9]],
                            [1,
			    Highcharts.Color(Highcharts.getOptions().colors[9]).setOpacity(0).get('rgba')]
                        ]
                    },
                    lineWidth: 1,
                    marker: {
                        enabled: false
                    },
                    shadow: false,
                    states: {
                        hover: {
                            lineWidth: 1
                        }
                    },
                    threshold: null
                }
            },
    
            series: [{
                type: 'area',
                name: 'temp',
                pointInterval: 24 * 3600 * 1000,
                pointStart: Date.UTC(2013, 4, 15),
                data: [
<?php
  $datafile = fopen("/home/nad213/WWW-data/data.txt","r");
  if (($line = fgets($datafile)) !== false) {
    $terms = explode(",", $line);
    echo $terms[1];
  }
  while (($line = fgets($datafile)) !== false) {
    $terms = explode(",", $line);
    echo ", ".$terms[1];
  }
  fclose($datafile);
?>
                ]
            }]
        });
    });
    </script>
    <script>
      $(function () {
        $('#humid_graph').highcharts({
            chart: {
                zoomType: 'x',
                spacingRight: 20
            },
            title: {
                text: 'Greenhouse Humidity'
            },
	    subtitle: {
                text: 'Last 24 hours'
            },
            xAxis: {
                type: 'datetime',
                maxZoom: 1 * 24 * 3600000, //one day
                title: {
                    text: null
                }
            },
            yAxis: {
                title: {
                    text: 'Humidity (%)'
                }
            },
            tooltip: {
                shared: true
            },
            legend: {
                enabled: false
            },
            plotOptions: {
                area: {
                    fillColor: {
                        linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1},
                        stops: [
                            [0, Highcharts.getOptions().colors[0]],
                            [1,
			    Highcharts.Color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
                        ]
                    },
                    lineWidth: 1,
                    marker: {
                        enabled: false
                    },
                    shadow: false,
                    states: {
                        hover: {
                            lineWidth: 1
                        }
                    },
                    threshold: null
                }
            },
    
            series: [{
                type: 'area',
                name: 'humid',
                pointInterval: 24 * 3600 * 1000,
                pointStart: Date.UTC(2013, 4, 15),
                data: [
<?php
  $datafile = fopen("/home/nad213/WWW-data/data.txt","r");
  if (($line = fgets($datafile)) !== false) {
    $terms = explode(",", $line);
    echo $terms[2];
  }
  while (($line = fgets($datafile)) !== false) {
    $terms = explode(",", $line);
    echo ", ".$terms[2];
  }
  fclose($datafile);
?>
                ]
            }]
        });
    });
    </script>
<!--Soil Moisture Graph -->
    <script>
      $(function () {
        $('#soil_graph').highcharts({
            chart: {
                zoomType: 'x',
                spacingRight: 20
            },
            title: {
                text: 'Soil Moisture'
            },
	    subtitle: {
                text: 'Last 24 hours'
            },
            xAxis: {
                type: 'datetime',
                maxZoom: 1 * 24 * 3600000, //one day
                title: {
                    text: null
                }
            },
            yAxis: {
                title: {
                    text: 'Moisture (Volts)'
                }
            },
            tooltip: {
                shared: true
            },
            legend: {
                enabled: false
            },
            plotOptions: {
                area: {
                    fillColor: {
                        linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1},
                        stops: [
                            [0, Highcharts.getOptions().colors[3]],
                            [1,
			    Highcharts.Color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
                        ]
                    },
                    lineWidth: 1,
                    marker: {
                        enabled: false
                    },
                    shadow: false,
                    states: {
                        hover: {
                            lineWidth: 1
                        }
                    },
                    threshold: null
                }
            },
    
            series: [{
                type: 'area',
                name: 'soil',
                pointInterval: 24 * 3600 * 1000,
                pointStart: Date.UTC(2013, 4, 15),
                data: [
<?php
  $datafile = fopen("/home/nad213/WWW-data/data.txt","r");
  if (($line = fgets($datafile)) !== false) {
    $terms = explode(",", $line);
    echo $terms[3];
  }
  while (($line = fgets($datafile)) !== false) {
    $terms = explode(",", $line);
    echo ", " . trim($terms[3]);
  }
  fclose($datafile);
?>
                ]
            }]
        });
    });
    </script>
  </head>
  <body>
    <div id="panel">
      <div id="content">
        <!-- Content -->
        <div class="center">
          <h1><span class="green">Green</span>Sense</h1>
          <h2>Greenhouse Automation & Sensing</h2>
	  <h3>Current Greenhouse Environment</h3>
	  <ul style="list-style: none;">
<?php
  $datafile = fopen("/home/nad213/WWW-data/data.txt","r");
  $cursor = -1;
  $line = '';

  fseek($datafile, $cursor, SEEK_END);
  $char = fgetc($datafile);

  while ($char === "\n" || $char === "\r") {
    fseek($datafile, $cursor--, SEEK_END);
    $char = fgetc($datafile);
  }

  while ($char !== false && $char !== "\n" && $char !== "\r") {
    $line = $char . $line;
    fseek($datafile, $cursor--, SEEK_END);
    $char = fgetc($datafile);
  }

  $terms = explode(",", $line);
  $dt = new DateTime("@$terms[0]");
  echo "<li>Last Updated: " . $dt->format('m-d-Y H:i:s') . "</li>";
  echo "<li>Temperature (F): " . $terms[1] . "</li>";
  echo "<li>Humidity (%RH): " . $terms[2] . "</li>";
  echo "<li>Soil Moisture (V): " . $terms[3] . "</li>";
  fclose($datafile);
?>
	  </ul>
	  <h3>Current Greenhouse Settings</h3>
	  <form name="input"
  action="http://wwwtest.cse.lehigh.edu/~nad213/adjust_fan.cgi" method="POST">
	    Control Fans: 
	    <input type="radio" name="fan" value="2">AUTO
	    <input type="radio" name="fan" value="1">ON
	    <input type="radio" name="fan" value="0">OFF<br>
	    <input type="submit" value="Submit Changes"><br>
	  </form>
        </div>
	<br>
        <div id="data_holder">
          <div id="temp_graph"></div>
	  <div id="humid_graph"></div>
	  <div id="soil_graph"></div>
        </div>
      </div>
    </div>
  </body>
</html>
