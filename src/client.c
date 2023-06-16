#include "tcp.h"
#include "hash.h"

#define FILENAME "archivo.txt"
#define SERVER_PORT 10000

static void send_file(const char *filepath, int sock)
{
    // Abrir archivo para lectura
    FILE *fp = fopen(filepath, "rb");
    if (!fp)
    {
        fprintf(stderr, "Error al abrir el archivo \n");
        exit(EXIT_FAILURE);
    }

    // Obtener tamaño del archivo
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    printf("Enviando archivo: %s (%zu bytes)\n", FILENAME, file_size);

    // Cerrar archivo
    fclose(fp);

    // Enviar nombre del archivo y tamaño en una sola llamada a tcp_send()
    char header[64];
    snprintf(header, sizeof(header), "%s:%zu", FILENAME, file_size);
    tcp_send(sock, header, strlen(header) + 1);

    // Enviar archivo
    tcp_sendfile(sock, filepath);
}

int main(void)
{
    // Conectarse al servidor
    struct tcp_client_t client;
    tcp_client_connect(&client, "127.0.0.1", SERVER_PORT);

    // Directorio donde se guardan los archivos a enviar
    const char *save_dir = "../archivos-cliente/";

    // Construir la ruta completa al archivo
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s%s", save_dir, FILENAME);
    // Enviar archivo
    send_file(filepath, client.sock);

    // calcular hash
    unsigned char hash[SHA256_DIGEST_LENGTH];
    calculate_sha256(filepath, hash);

    // enviar hash para verificacion
    tcp_send(client.sock, hash, SHA256_DIGEST_LENGTH);

    // Cerrar conexión
    tcp_close(client.sock);

    return 0;
}
