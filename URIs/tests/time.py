#!/usr/bin/python3

import datetime

# Obtenir l'heure actuelle du serveur
current_time = datetime.datetime.now()

# Formater l'heure actuelle en une chaîne lisible
formatted_time = current_time.strftime("%A, %d %B %Y, %H:%M:%S")

# Générer la sortie HTML
body = f"""\
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Heure du Serveur</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            background-color: #1e272e;
            color: #f5f6fa;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }}

        .container {{
            text-align: center;
            background-color: #2f3640;
            padding: 30px;
            border-radius: 10px;
            box-shadow: 0 8px 16px rgba(0, 0, 0, 0.3);
        }}

        h1 {{
            font-size: 2em;
            color: #00a8ff;
            margin-bottom: 10px;
        }}

        p {{
            font-size: 1.5em;
            color: #ffffff;
        }}

        #refreshButton, #goBackButton {{
            margin-top: 20px;
            padding: 10px 20px;
            font-size: 1.2em;
            background-color: #00a8ff;
            color: #fff;
            border: none;
            border-radius: 8px;
            cursor: pointer;
            transition: background-color 0.3s ease;
            width: 100%;
        }}

        #refreshButton:hover, #goBackButton:hover {{
            background-color: #0097e6;
        }}

        .goBackButtonContainer {{
            margin-top: 10px;
        }}
    </style>
</head>
<body>
    <div class="container">
        <h1>Heure du Serveur</h1>
        <p>{formatted_time}</p>
        <button id="refreshButton" onclick="refreshPage()">Actualiser</button>
        <div class="goBackButtonContainer">
            <button id="goBackButton" onclick="goBack()">Retour</button>
        </div>
    </div>

    <script>
    function refreshPage() {{
        location.reload();
    }}

    function goBack() {{
        window.history.back();
    }}
    </script>
</body>
</html>
"""

# Calcul de la longueur du contenu HTML
content_length = len(body)

# Imprimer les en-têtes HTTP et le contenu HTML
print(f"Content-Type: text/html\r")
print(f"Content-Length: {content_length}\r")
print("\r")  # Ligne vide pour séparer les en-têtes du corps
print(body)
