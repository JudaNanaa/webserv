#!/bin/bash

# Génération du contenu HTML amélioré
body='<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Test CGI Amélioré</title>
    <style>
        body {
            font-family: "Arial", sans-serif;
            text-align: center;
            background: linear-gradient(to bottom right, #8ec5fc, #e0c3fc);
            color: #333;
            padding: 20px;
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
        }

        .container {
            background-color: rgba(255, 255, 255, 0.9);
            padding: 30px;
            border-radius: 15px;
            box-shadow: 0 8px 16px rgba(0, 0, 0, 0.2);
            width: 100%;
            max-width: 400px;
        }

        h2 {
            font-size: 2em;
            color: #4a90e2;
            margin-bottom: 20px;
        }

        #goBackButton {
            padding: 12px 25px;
            font-size: 1.2em;
            color: white;
            background-color: #4a90e2;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            transition: background-color 0.3s ease, transform 0.2s ease;
            text-decoration: none;
        }

        #goBackButton:hover {
            background-color: #357abd;
            transform: scale(1.05);
        }

        #goBackButton:active {
            transform: scale(0.95);
        }
    </style>
</head>
<body>
    <div class="container">
        <h2>I guess I'\''m the output of a CGI, huh?</h2>
        <a id="goBackButton" href="javascript:history.back()">Retour</a>
    </div>
</body>
</html>'

# Calcul de la longueur du contenu
content_length=${#body}

# Impression des en-têtes HTTP
echo "Content-Type: text/html"
echo "Content-Length: $content_length"
echo
# Impression du contenu HTML
echo "$body"
