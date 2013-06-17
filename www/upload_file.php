<!DOCTYPE html>
<html lang="fr">
  <head>
    <meta charset="utf-8">
    <title>Projet Gutenberg</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="Site permettant d'afficher les données d'un fichier issu d'Illustrator et à destination de la graveuse">
    <meta name="author" content="ENSIIE">

    <!-- Le styles -->
    <script src="http://code.jquery.com/jquery-1.10.1.min.js"></script>
    
    <link href="css/bootstrap.css" rel="stylesheet">
    <style type="text/css">
      body {
        padding-top: 20px;
        padding-bottom: 40px;
      }
      /* Custom container */
      .container-narrow {
        margin: 0 auto;
        max-width: 700px;
      }
      .container-narrow > hr {
        margin: 30px 0;
      }

      .woot {
	text-align: center;
      }

    </style>
    <link href="../assets/css/bootstrap-responsive.css" rel="stylesheet">

    <!-- HTML5 shim, for IE6-8 support of HTML5 elements -->
    <!--[if lt IE 9]>
      <script src="../assets/js/html5shiv.js"></script>
    <![endif]-->

    <!-- Fav and touch icons -->
    <link rel="apple-touch-icon-precomposed" sizes="144x144" href="../assets/ico/apple-touch-icon-144-precomposed.png">
    <link rel="apple-touch-icon-precomposed" sizes="114x114" href="../assets/ico/apple-touch-icon-114-precomposed.png">
      <link rel="apple-touch-icon-precomposed" sizes="72x72" href="../assets/ico/apple-touch-icon-72-precomposed.png">
                    <link rel="apple-touch-icon-precomposed" href="../assets/ico/apple-touch-icon-57-precomposed.png">
                                   <link rel="shortcut icon" href="../assets/ico/favicon.png">
  </head>

  <body>
    <div class="container-narrow">
      <div class="masthead">
        <ul class="nav nav-pills pull-right">
          <li class="active"><a href="#">Accueil</a></li>
          <li><a href="#">À propos</a></li>
          <li><a href="#">Contact</a></li>
        </ul>
        <h3 class="muted">Projet Gutenberg</h3>
      </div>

      <hr>
    </div>

    <div class="container">

      <!-- Main hero unit for a primary marketing message or call to action -->
      <div class="hero-unit">
        <h1>Fichier optimisé!</h1>
<hr>
        <p>Votre fichier a bien été analysé. Nous avons appliqué un algorithme ayant pour but d'optimiser les déplacements de la tête de gravure. Cliquez ci-dessous afin de télécharger le nouveau fichier généré. Vous pourrez alors l'imprimer tout simplement avec la gravure.</p>

<?php
    $allowedExts = array("prn");
    $extension = end(explode(".", $_FILES["file"]["name"]));
    if (($_FILES["file"]["size"] < 2000000)
        && in_array($extension, $allowedExts))
    {
        if ($_FILES["file"]["error"] != 0)
        {
            echo "Return Code: " . $_FILES["file"]["error"] . '<br>';
        }
        else
        {
//            echo "Upload: " . $_FILES["file"]["name"] . "<br>";
//            echo "Type: " . $_FILES["file"]["type"] . "<br>";
//            echo "Size: " . ($_FILES["file"]["size"] / 1024) . " kB<br>";
//            echo "Temp file: " . $_FILES["file"]["tmp_name"] . "<br>";
    
            if (file_exists("upload/" . $_FILES["file"]["name"]))
            {
                echo $_FILES["file"]["name"] . " already exists. ";
            }
            else
            {
                move_uploaded_file($_FILES["file"]["tmp_name"],
                    "upload/" . $_FILES["file"]["name"]);
//                echo "Stored in: " . "upload/" . $_FILES["file"]["name"] . "<br>";
            }
        }
        chdir('upload');
        exec('./prog '.$_FILES["file"]["name"],$result);
        // echo "Result : <br>";
        // print_r($result);
        
        echo "<script type='text/javascript'> var res = new Array();";
            foreach ($result as $cle =>$val){
                echo "res[".$cle."] = \"".$val."\"\n;";   
            }
        echo "</script>";        
        
        exec('rm ./'.$_FILES["file"]["name"], $resultRM);
//        print_r($resultRM);
//        echo "Cliquez ici pour télécharger le nouveau fichier";
        echo "<p><a href=\"upload/new_". $_FILES['file']['name']."\" class=\"btn btn-large btn-success\">Telecharger</a>";
    }
    else
    {
        echo "Invalid file ".$_FILES["file"]["size"]." et ".$extension;
    }
?> 
      </div>

      <!-- Example row of columns -->
      <div class="row">
        <div class="span4">
        </div>
        <div class="span4" style='width:600px;margin-left:20%'>
          <h2>Temps de découpe</h2>
            <p> Sélectionnez le matériel dans lequel vous effectuez la découpe :
		<select id='materiel'>
		<option value="defaut">---</option>
		<option value="plexi">Plexiglas</option>
		<option value="carton">Carton souple</option>
		</select>
       	</p>
          <p>Le temps de découpe approximatif pour cette forme est de <span id='tempsDecoupe'>---</span> secondes.
       	</div>
        <div class="span4">
        </div>
      </div>

      <hr>

      <footer>
        <p>&copy; ENSIIE 2013</p>
      </footer>

    </div> <!-- /container -->

    <!-- Le javascript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="js/canvas.js"></script>
    <script src="../assets/js/jquery.js"></script>
    <script src="../assets/js/bootstrap-transition.js"></script>
    <script src="../assets/js/bootstrap-alert.js"></script>
    <script src="../assets/js/bootstrap-modal.js"></script>
    <script src="../assets/js/bootstrap-dropdown.js"></script>
    <script src="../assets/js/bootstrap-scrollspy.js"></script>
    <script src="../assets/js/bootstrap-tab.js"></script>
    <script src="../assets/js/bootstrap-tooltip.js"></script>
    <script src="../assets/js/bootstrap-popover.js"></script>
    <script src="../assets/js/bootstrap-button.js"></script>
    <script src="../assets/js/bootstrap-collapse.js"></script>
    <script src="../assets/js/bootstrap-carousel.js"></script>
    <script src="../assets/js/bootstrap-typeahead.js"></script>

  </body>
</html>

