<?php
// information concernant le fichier à télécharger
$fichier = 'newFile.prn';
$nom_fichier = 'newFile.prn';

// téléchargement du fichier
header('Content-disposition: attachment; filename='.$nom_fichier);
header('Content-Type: application/force-download');
header('Content-Transfer-Encoding: fichier');
header('Content-Length: '.filesize($fichier));
header('Pragma: no-cache');
header('Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0');
header('Expires: 0');
readfile($fichier);
exit();
?>
