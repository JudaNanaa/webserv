#!/usr/bin/python3

# Génération du contenu HTML
body = """\
<!DOCTYPE html>
<html>
<head>
    <title>Test CGI</title>
</head>
<body>
    <h2>I guess I'm the output of a CGI, huh?</h2>
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
