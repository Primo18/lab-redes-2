#include "tcp.h"
#include "hash.h"

static void receive_file(const char *save_dir, int client_sock);

int main(void)
{
    struct tcp_server_t server;

    // Directorio donde se guardaran los archivos recibidos
    const char *save_dir = "../archivos-servidor/";

    // Crear servidor TCP en el puerto 10000
    tcp_server_create(&server, 10000);

    printf("Servidor escuchando en el puerto 10000\n");

    while (1)
    {
        // Aceptar la conexión entrante del cliente
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sock = tcp_server_accept(&server, &client_addr, &client_addr_len);

        // Habilitar reutilización de direcciones
        int optval = 1;
        if (setsockopt(client_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        {
            // Si hubo algún error, imprimimos un mensaje y salimos
            fprintf(stderr, "%s: setsockopt falló\n", __func__);
            return 1;
        }

        printf("Cliente conectado\n");

        // Recibir y enviar el archivo
        receive_file(save_dir, client_sock);
    }

    return 0;
}

static void receive_file(const char *save_dir, int client_sock)
{
    // Recibir el nombre y del archivo que el cliente desea enviar
    char filename[256];
    tcp_recv(client_sock, filename, sizeof(filename));
    char *header = strtok(filename, ":");
    char *file_size_str = strtok(NULL, ":");
    size_t file_size = atoi(file_size_str);
    printf("Solicitando archivo: %s (%zu bytes)\n", header, file_size);

    // Recibir y Guardar archivo
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s%s", save_dir, filename);
    tcp_recvfile(client_sock, filepath, file_size);

    // calcular hash
    unsigned char hash[SHA256_DIGEST_LENGTH];
    calculate_sha256(filepath, hash);
    // recibir hash y comparar
    unsigned char last_hash[SHA256_DIGEST_LENGTH];
    tcp_recv(client_sock, last_hash, SHA256_DIGEST_LENGTH);
    compare_hashes(hash, last_hash);

    // Cerrar conexión
    tcp_close(client_sock);
}
