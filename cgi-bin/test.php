#!/usr/bin/php
<?php
header("Content-Type: text/html; charset=UTF-8");

echo "<html><body>";
echo "<h1>CGI PHP</h1>";

// Récupération des données GET
$query = getenv("QUERY_STRING");
parse_str($query, $getParams);
echo "<p>Données GET :</p><pre>";
print_r($getParams);
echo "</pre>";

// Récupération des données POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $postData = file_get_contents('php://input');
    echo "<p>Données POST :</p><pre>";
    echo htmlspecialchars($postData);
    echo "</pre>";
}

echo "</body></html>";
?>
