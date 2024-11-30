#!/usr/bin/python3

# Génération du contenu HTML amélioré
body = """\
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Test CGI Amélioré</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            background-color: #f0f0f0;
            color: #333;
            padding: 20px;
        }

        h2 {
            font-size: 2em;
            color: #007bff;
            margin-bottom: 20px;
        }

        #goBackButton {
            padding: 10px 20px;
            font-size: 1.2em;
            color: #fff;
            background-color: #007bff;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            transition: background-color 0.3s ease;
            text-decoration: none;
        }

        #goBackButton:hover {
            background-color: #0056b3;
        }
    </style>
</head>
<body>
    <h2>I guess I'm the output of a CGI, huh?</h2>
    <a id="goBackButton" href="javascript:history.back()">Go Back</a>
</body>
</html>
"""

# Calcul de la longueur du contenu
content_length = len(body)

# Impression des en-têtes HTTP et du contenu
print(f"Content-Type: text/html\r")
print(f"Content-Length: {content_length}\r")
print("\r")  # Ligne vide pour séparer les en-têtes du corps
print(body)
