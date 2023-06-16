#include "hash.h"
#define BUFFER_SIZE 4096

void calculate_sha256(const char *file_path, unsigned char *hash)
{
    // abrir archivo
    FILE *file = fopen(file_path, "rb");
    if (!file)
    {
        perror("Error al abrir el archivo");
        return;
    }

    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);

    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    // almacenar la cantidad de bytes leídos en cada iteración del bucle.
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0)
    {
        SHA256_Update(&sha256_ctx, buffer, bytes_read);
    }
    // cálculo del hash
    SHA256_Final(hash, &sha256_ctx);
    // imprimir
    /*
    printf("Hash SHA-256 del archivo:\n");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\n");
    */
    fclose(file);
    return;
}
void compare_hashes(const unsigned char *hash1, const unsigned char *hash2)
{
    size_t hash_length = sizeof(hash1) / sizeof(hash1[0]);
    for (size_t i = 0; i < hash_length; i++)
    {
        if (hash1[i] != hash2[i])
        {
            printf("Los hashes son diferentes\n");
            return; // Los hashes no son iguales
        }
    }
    printf("Los hashes son iguales\n");
    return;
}