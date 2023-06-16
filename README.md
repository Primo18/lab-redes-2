# Servidor HTTP Simple

## Descripción
* El proyecto consiste en un servidor HTTP simple que recibe peticiones GET.
* El servidor escucha en el puerto 10080 y espera conexiones de clientes. 
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
# Ejecutar servidor
$ ./server
# Ejecutar cliente
$ ./client
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

