#!/usr/bin/php
<?php
// Démarrer la capture de la sortie
ob_start();

// Afficher un message de bienvenue ou un contenu dynamique
echo "<html><body>";
echo "<h1>Bienvenue sur mon script PHP CGI!</h1>";
echo "<p>Le script a été exécuté en tant que CGI.</p>";
echo "<p>Voici la date et l'heure actuelles: " . date('Y-m-d H:i:s') . "</p>";
echo "<p>Voici les variables de l'environnement :</p><pre>";
print_r($_SERVER);
echo "</pre>";
echo "</body></html>";

// Récupérer la sortie capturée
$output = ob_get_clean();

// Calculer la longueur du contenu
$contentLength = strlen($output);

// Afficher l'en-tête Content-Length
echo "Content-Type: text/html; charset=UTF-8\r\n";
echo "Content-Length: $contentLength\r\n";
echo "\r\n";

// Afficher le contenu généré
echo $output;
?>
