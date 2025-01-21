# Guía de Linux y Uso de la Línea de Comandos (CLI)

Linux es un sistema operativo de código abierto ampliamente utilizado por su flexibilidad, seguridad y eficiencia. La línea de comandos (Command Line Interface, CLI) es una herramienta esencial en Linux para interactuar directamente con el sistema mediante comandos.

---

## ¿Qué es la CLI?

La CLI es una interfaz basada en texto donde los usuarios pueden ejecutar comandos para realizar tareas como gestionar archivos, procesos, redes, y más. Es una forma poderosa y eficiente de administrar sistemas Linux, especialmente en servidores y entornos sin interfaz gráfica.

---

## Estructura de los Comandos en Linux

Un comando en Linux sigue generalmente esta estructura:

```bash
comando [opciones] [argumentos]
```

### Componentes:
1. **Comando**: El programa o utilidad a ejecutar.
2. **Opciones**: Modificadores que alteran el comportamiento del comando (precedidas por `-` o `--`).
3. **Argumentos**: Los objetos sobre los que actúa el comando (archivos, directorios, procesos, etc.).

### Ejemplo:
```bash
ls -l /home
```
- **Comando**: `ls` (lista el contenido de un directorio).
- **Opción**: `-l` (muestra detalles en formato largo).
- **Argumento**: `/home` (el directorio a listar).

---

## Principales Comandos Agrupados por Temas

### 1. Gestión de Archivos y Directorios
| Comando         | Descripción                                    |
|-----------------|------------------------------------------------|
| `ls`           | Lista los archivos y directorios.             |
| `cd`           | Cambia el directorio actual.                  |
| `pwd`          | Muestra el directorio actual.                 |
| `mkdir`        | Crea un nuevo directorio.                     |
| `rmdir`        | Elimina un directorio vacío.                  |
| `cp`           | Copia archivos o directorios.                 |
| `mv`           | Mueve o renombra archivos o directorios.      |
| `rm`           | Elimina archivos o directorios.               |
| `find`         | Busca archivos en un directorio.              |
| `touch`        | Crea archivos vacíos o actualiza su marca de tiempo. |

### 2. Gestión de Procesos
| Comando         | Descripción                                    |
|-----------------|------------------------------------------------|
| `ps`           | Muestra los procesos en ejecución.            |
| `top`          | Muestra información dinámica de procesos.      |
| `htop`         | Similar a `top` con una interfaz interactiva.  |
| `kill`         | Termina un proceso mediante su PID.            |
| `pkill`        | Termina procesos por nombre.                   |
| `jobs`         | Lista los trabajos en segundo plano.           |
| `bg`           | Reanuda un trabajo en segundo plano.           |
| `fg`           | Reanuda un trabajo en primer plano.            |

### 3. Gestión de Permisos y Usuarios
| Comando         | Descripción                                    |
|-----------------|------------------------------------------------|
| `chmod`        | Cambia permisos de archivos y directorios.     |
| `chown`        | Cambia el propietario de un archivo.           |
| `chgrp`        | Cambia el grupo de un archivo.                 |
| `whoami`       | Muestra el usuario actual.                     |
| `id`           | Muestra información sobre el usuario actual.   |
| `su`           | Cambia al usuario superusuario o a otro usuario.|
| `sudo`         | Ejecuta comandos como superusuario.            |
| `passwd`       | Cambia la contraseña de un usuario.            |

### 4. Comandos de Red
| Comando         | Descripción                                    |
|-----------------|------------------------------------------------|
| `ping`         | Comprueba la conectividad con otro host.       |
| `ifconfig`     | Configura interfaces de red (reemplazado por `ip`). |
| `ip`           | Muestra y configura interfaces de red.         |
| `netstat`      | Muestra conexiones de red y puertos.           |
| `curl`         | Descarga datos desde una URL.                  |
| `wget`         | Descarga archivos desde una URL.               |
| `ssh`          | Conecta a un servidor mediante SSH.            |
| `scp`          | Copia archivos mediante SSH.                   |
| `ftp`          | Cliente FTP para transferencia de archivos.    |

### 5. Gestión del Sistema
| Comando         | Descripción                                    |
|-----------------|------------------------------------------------|
| `df`           | Muestra el uso del disco.                     |
| `du`           | Muestra el uso de espacio por archivo/directorio. |
| `uptime`       | Muestra el tiempo que el sistema lleva encendido.|
| `uname`        | Muestra información del sistema.               |
| `free`         | Muestra el uso de memoria.                     |
| `reboot`       | Reinicia el sistema.                           |
| `shutdown`     | Apaga el sistema.                              |
| `dmesg`        | Muestra mensajes del kernel.                   |
| `systemctl`    | Gestiona servicios y estados del sistema.      |

### 6. Editores de Texto
| Comando         | Descripción                                    |
|-----------------|------------------------------------------------|
| `nano`         | Editor de texto simple.                        |
| `vim`          | Editor avanzado con modos de edición.          |
| `emacs`        | Editor de texto avanzado y personalizable.     |
| `gedit`        | Editor de texto con interfaz gráfica.          |

### 7. Compresión y Archivado
| Comando         | Descripción                                    |
|-----------------|------------------------------------------------|
| `tar`          | Crea y extrae archivos tar.                    |
| `gzip`         | Comprime archivos con el formato gzip.         |
| `gunzip`       | Descomprime archivos gzip.                     |
| `zip`          | Crea archivos zip.                            |
| `unzip`        | Extrae archivos zip.                           |

---

## Consejos para Usar la CLI

1. **Autocompletar**:
   - Usa la tecla `Tab` para completar comandos y nombres de archivos automáticamente.

2. **Historial**:
   - Usa las teclas de flecha `Arriba` y `Abajo` para navegar por comandos anteriores.

3. **Manuales**:
   - Usa `man comando` para ver el manual de un comando, por ejemplo:
     ```bash
     man ls
     ```

4. **Ayuda Rápida**:
   - Algunos comandos aceptan la opción `--help` para mostrar información breve:
     ```bash
     ls --help
     ```

5. **Alias**:
   - Crea alias para comandos largos en tu archivo `~/.bashrc`:
     ```bash
     alias ll='ls -l'
     ```

---

## Recursos Adicionales

Para ampliar tus conocimientos sobre comandos de Linux, puedes visitar los siguientes sitios web:

1. [The Linux Command Line](https://linuxcommand.org/) - Una guía completa para principiantes y usuarios avanzados.
2. [Explainshell](https://explainshell.com/) - Descompone y explica comandos de Linux.
3. [TLDR Pages](https://tldr.sh/) - Resúmenes concisos de comandos con ejemplos prácticos.
4. [SS64 Linux](https://ss64.com/bash/) - Referencia rápida para comandos y scripts Bash.
5. [Red Hat Linux Commands](https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/) - Documentación oficial para entornos empresariales.
