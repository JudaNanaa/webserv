#!/usr/bin/php
<?php
// Définir l'en-tête pour indiquer qu'il s'agit d'une réponse HTTP
header("Content-Type: text/html; charset=UTF-8");

// Afficher un message de bienvenue ou un contenu dynamique
echo "<html><body>";
echo "<h1>Bienvenue sur mon script PHP CGI!</h1>";
echo "<p>Le script a été exécuté en tant que CGI.</p>";
echo "<p>Voici la date et l'heure actuelles: " . date('Y-m-d H:i:s') . "</p>";
echo "<p>Voici les variables de l'environnement :</p><pre>";
print_r($_SERVER);
echo "</pre>";
echo "</body></html>";
?>
