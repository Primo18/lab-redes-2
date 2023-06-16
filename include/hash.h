#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <openssl/sha.h>

//calcular hash
void calculate_sha256(const char *file_path, unsigned char *hash);
//comparar hashes
void compare_hashes(const unsigned char *hash1, const unsigned char *hash2);

#endif // HASH_H
