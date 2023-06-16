#include "tcp.h"
#include "hash.h"
#include <sys/stat.h>

static void handle_http_request(const char *save_dir, const char *request, int client_sock);
static void send_response(int client_sock, int status_code, const char *status_text, const char *content_type, const char *content);

int main(void)
{
    struct tcp_server_t server;

    // Directorio donde se guardarán los archivos a servir
    const char *serve_dir = "../archivos-servidor/";

    // Crear servidor TCP en el puerto 10088 (puerto por defecto)
    tcp_server_create(&server, 10088);

    printf("Servidor HTTP escuchando en el puerto 10088\n");

    while (1)
    {
        // Aceptar la conexión entrante del cliente
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sock = tcp_server_accept(&server, &client_addr, &client_addr_len);

        printf("Cliente conectado\n");

        // Recibir y manejar la solicitud HTTP
        char request[1024];
        tcp_recv(client_sock, request, sizeof(request));
        handle_http_request(serve_dir, request, client_sock);

        // Cerrar conexión
        tcp_close(client_sock);
    }

    return 0;
}

static void handle_http_request(const char *save_dir, const char *request, int client_sock)
{
    // Parsear la solicitud HTTP para obtener el método y la ruta
    char method[16];
    char path[256];
    sscanf(request, "%15s %255s", method, path);

    // Verificar el método de la solicitud
    if (strcmp(method, "GET") != 0)
    {
        // Método no permitido (405 Method Not Allowed)
        send_response(client_sock, 405, "Method Not Allowed", "text/plain", "Method not allowed");
        return;
    }

    // Construir la ruta completa del archivo solicitado
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s%s", save_dir, path);

    // Verificar si la ruta es un directorio y ajustar la ruta si es necesario
    struct stat path_stat;
    if (stat(filepath, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
    {
        // Agregar "/index.html" al final de la ruta
        strncat(filepath, "/index.html", sizeof(filepath) - strlen(filepath) - 1);
    }

    // Abrir el archivo solicitado
    FILE *file = fopen(filepath, "rb");
    if (file == NULL)
    {
        // Archivo no encontrado (404 Not Found)
        send_response(client_sock, 404, "Not Found", "text/plain", "File not found");
        return;
    }

    // Determinar el tipo de contenido (Content-Type) según la extensión del archivo
    const char *content_type = "application/octet-stream";
    const char *extension = strrchr(filepath, '.');
    if (extension != NULL)
    {
        if (strcmp(extension, ".html") == 0 || strcmp(extension, ".htm") == 0)
        {
            content_type = "text/html";
        }
        else if (strcmp(extension, ".css") == 0)
        {
            content_type = "text/css";
        }
        else if (strcmp(extension, ".js") == 0)
        {
            content_type = "application/javascript";
        }
        else if (strcmp(extension, ".png") == 0)
        {
            content_type = "image/png";
        }
        else if (strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0)
        {
            content_type = "image/jpeg";
        }
        else if (strcmp(extension, ".txt") == 0)
        {
            content_type = "text/plain";
        }
    }

    // Leer el contenido del archivo y determinar su tamaño
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Leer el contenido del archivo en un búfer
    char *file_content = (char *)malloc(file_size);
    fread(file_content, file_size, 1, file);

    // Enviar la respuesta HTTP exitosa (200 OK) con el contenido del archivo
    send_response(client_sock, 200, "OK", content_type, file_content);

    // Liberar la memoria y cerrar el archivo
    free(file_content);
    fclose(file);
}

static void send_response(int client_sock, int status_code, const char *status_text, const char *content_type, const char *content)
{
    // Construir el encabezado de la respuesta HTTP
    char response[1024];
    snprintf(response, sizeof(response),
             "HTTP/1.1 %d %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %zu\r\n"
             "\r\n"
             "%s",
             status_code, status_text, content_type, strlen(content), content);

    // Enviar la respuesta al cliente
    tcp_send(client_sock, response, strlen(response));
}
