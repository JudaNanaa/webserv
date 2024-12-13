import requests
import threading


def serv_test(server_url):
    request_count = 100

    for i in range(request_count):
        try:
            response = requests.get(server_url, timeout=3)
            if response.status_code != 200:
                print(
                    f"\033[31mRequete {i + 1} ({server_url}) Status innatendu ({response.status_code})\033[0m")
                return False
        except requests.exceptions.Timeout:
            print(f"\033[34mRequête {i + 1}: TIMEOUT pour {server_url}\033[0m")
            return False
        except requests.RequestException as e:
            print(
                f"\033[34mRequete {i + 1}: Echec - {e} pour {server_url}\033[0m")
            return False
    return True


def test_port(server_url, tests_per_port, success_flag):
    """Exécute les tests pour un port donné dans un thread."""
    for i in range(tests_per_port):
        success = serv_test(server_url)
        if not success:
            print(
                f"\033[31m❌ Test échoué pour {server_url} - {i + 1}/{tests_per_port}\033[0m")
            success_flag[0] = False  # Flag indiquant l'échec du test
            break
    if success_flag[0]:
        print(f"\033[32m✅ Tous les tests réussis pour {server_url}\033[0m")


def test_loop():
    tab = [8080, 8081, 8082, 8083, 8084, 8085, 8086, 8087, 8088]
    base_url = "http://localhost:"

    request_count = 300
    ports_count = len(tab)

    tests_per_port = request_count // ports_count
    remaining_tests = request_count % ports_count

    threads = []

    for i in range(ports_count):
        server_url = f"{base_url}{tab[i]}"

        # Calcul du nombre de tests à effectuer pour ce port
        current_test_count = tests_per_port + (1 if i < remaining_tests else 0)

        print(f"\033[34mDémarrage des tests pour le port {tab[i]}\033[0m")

        # Flag pour savoir si le test a réussi pour ce port
        success_flag = [True]

        # Créer un thread pour chaque port
        thread = threading.Thread(target=test_port, args=(
            server_url, current_test_count, success_flag))
        threads.append(thread)
        thread.start()

    # Attendre que tous les threads terminent leur exécution
    for thread in threads:
        thread.join()


if __name__ == "__main__":
    test_loop()
