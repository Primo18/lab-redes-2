# Transferencia de archivos en TCP con cifrado simétrico

## Descripción
* El proyecto transfiere archivos entre un cliente y un servidor usando el protocolo TCP. 
* El servidor escucha en el puerto 10000 y espera conexiones de clientes. 
* Una vez que un cliente se conecta, el servidor recibe un archivo y lo guarda en el directorio `archivos-servidor`. 
* El cliente envia archivos desde el directorio `archivos-cliente`.
* Se comprueba la integridad de los archivos usando el algoritmo MD5.
* Se usa cifrado simétrico para cifrar y descifrar los archivos.
* Se muestra el progreso de la transferencia de archivos en el cliente y en el servidor.
* Se puede transferir cualquier tipo de archivo.

### Si se desea cambiar el archivo a transferir, se debe cambiar el nombre del archivo en el cliente:
```console
# Abrir el archivo cliente.c
$ nano cliente.c
# Cambiar el nombre del archivo en la línea 4
  4           #define FILENAME "../archivos-cliente/otroArchivo.jpg"
```


El proyecto está escrito en C y usa CMake para la compilación. El código una vez compilado genera los siguientes ejecutables:

Ejecutables para transferencia de archivos cliente-servidor:
- `server`: Servidor que escucha en el puerto 10000 y espera conexiones de clientes.
- `client`: Cliente que se conecta al servidor y envía un mensaje.

Ejecutables para cifrado y descifrado de archivos:
- `keygen`: Genera una llave secreta para el cifrado y descifrado.
- `encrypt`: Cifra un archivo con la llave secreta.
- `decrypt`: Descifra un archivo con la llave secreta.

## Instalación y compilación

Para compilar este proyecto, basta con seguir los pasos habituales para un proyecto con CMake:
(Se recomienda borrar la carpeta build si ya existe)

```console
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Prueba de ejemplo (Estar dentro de la carpeta build)
```bash
# Generar llave secreta y cifrar archivo
$ ./keygen llave.key
$ ./encrypt llave.key ../archivos-cliente/archivo.txt ../archivos-cliente/archivo.txt.enc
# Ejecutar servidor
$ ./server
# Ejecutar cliente
$ ./client
# Descifrar archivo
$ ./decrypt llave.key ../archivos-servidor/archivo.txt.enc ../archivos-servidor/archivo.txt
# Comparar archivos
$ md5sum ../archivos-cliente/archivo.txt ../archivos-servidor/archivo.txt
```

## Ejecución

Para ejecutar el servidor, se debe ejecutar el siguiente comando:

```console
$ ./server
```

Para ejecutar el cliente, se debe ejecutar el siguiente comando:

```console
$ ./client
```

Antes de empezar con el cifrado y descifrado y la transferencia de archivo7, se debe generar una llave secreta. 

```console
$ ./keygen llave.key
```

El programa generará una llave secreta aleatoria, que se guardará en el archivo `llave.key`. Esta llave debe ser compartida previamente con el otro usuario que recibirá los archivos cifrados, idealmente por un canal seguro.

Hecho esto, podemos cifrar un archivo usando el programa `encrypt`:

```console
$ ./encrypt llave.key archivo.txt archivo.txt.enc
```

Con esto, generamos un archivo `archivo.txt.enc` que contiene el cifrado del archivo `archivo.txt` usando la llave secreta que generamos.

Ahora, para descifrar el archivo, podemos usar el programa `decrypt`:

```console
$ ./decrypt llave.key archivo.txt.enc archivo.txt
```

Pueden comprobar que el archivo descifrado es el mismo que el original si calculan el _hash_ de ambos archivos:

```console
$ ./decrypt llave.key archivo.txt.enc archivo.txt.out # Desciframos el archivo con otro nombre para poder comparar los hashes
$ md5sum archivo.txt archivo.txt.out
