import http.client

def send_empty_request():
    try:
        # Connexion à un serveur local (127.0.0.1 sur le port 80)
        conn = http.client.HTTPConnection("127.0.0.1", 8080)
        
        # Envoi d'une requête GET vide sans chemin ni header
        conn.request("GET", "")
        
        # Récupération de la réponse
        response = conn.getresponse()
        print(f"Status: {response.status}")
        print(f"Reason: {response.reason}")
        print("Response Data:", response.read().decode())
        
        # Fermeture de la connexion
        conn.close()
    except Exception as e:
        print(f"Error: {e}")

# Appel de la fonction
for i in range(1000):
	send_empty_request()
