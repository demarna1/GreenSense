#!/opt/local/bin/php
<!DOCTYPE html>
<html>
  <head>
    <!-- Add cache -->
    <!-- meta http-equiv="cache-control" control="no-store" -->
    <title>GreenSense</title>
    <link type="text/css" href="css/style.css" rel="stylesheet"/>
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
          <ul id="data_list">
	    <?php
	      echo "<li>Temp Humid Soil</li>";
	      $datafile=fopen("data.txt","r");
	      while(!feof($datafile)) { 
	        echo "<li>".fgets($datafile)."</li>";
	      }
	      fclose($datafile);
	    ?>
          </ul>
        </div>
      </div>
    </div>
  </body>
</html>
