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
        $('#graph_holder').highcharts({
            chart: {
                zoomType: 'x',
                spacingRight: 50,
		spacingLeft: 50
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
                pointStart: Date.UTC(2006, 0, 01),
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
  </head>
  <body>
    <div id="panel">
      <div id="content">
        <!-- Content -->
        <div class="center">
          <h1><span class="green">Green</span>Sense</h1>
          <h2>Greenhouse Automation & Sensing</h2>
        </div>
        <div id="data_holder">
          <div id="graph_holder"></div>
        </div>
      </div>
    </div>
  </body>
</html>
