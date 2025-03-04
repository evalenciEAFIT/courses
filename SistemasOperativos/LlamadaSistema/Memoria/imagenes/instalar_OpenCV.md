# Instalación de OpenCV y STB Image en Diferentes Sistemas Operativos

## Introducción
Este documento describe los pasos para instalar **OpenCV** y las librerías **STB Image** (`stb_image.h` y `stb_image_write.h`) en **Fedora, Ubuntu, Kali Linux y Windows Subsystem for Linux (WSL)**.

---

## 🔧 Instalación en Fedora
```bash
sudo dnf install -y opencv opencv-devel gcc-c++ make wget curl
```
**Descargar STB Image con `wget` o `curl`**
```bash
# Opción 1: Usando wget
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h

# Opción 2: Usando curl
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h
```

---

## 🔧 Instalación en Ubuntu (22.04 o superior)
```bash
sudo apt update && sudo apt install -y libopencv-dev g++ make wget curl
```
**Descargar STB Image con `wget` o `curl`**
```bash
# Opción 1: Usando wget
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h

# Opción 2: Usando curl
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h
```

---

## 🔧 Instalación en Kali Linux
```bash
sudo apt update && sudo apt install -y libopencv-dev g++ make wget curl
```
**Descargar STB Image con `wget` o `curl`**
```bash
# Opción 1: Usando wget
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h

# Opción 2: Usando curl
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h
```

---

## 🔧 Instalación en Windows Subsystem for Linux (WSL)
**Para WSL con Ubuntu (Recomendado)**
```bash
sudo apt update && sudo apt install -y libopencv-dev g++ make wget curl
```
**Descargar STB Image con `wget` o `curl`**
```bash
# Opción 1: Usando wget
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h

# Opción 2: Usando curl
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h
```

**Para WSL con Fedora**
```bash
sudo dnf install -y opencv opencv-devel gcc-c++ make wget curl
```
**Descargar STB Image con `wget` o `curl`**
```bash
# Opción 1: Usando wget
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h

# Opción 2: Usando curl
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h
```

---

## Verificación de Instalación
Ejecuta el siguiente comando para verificar que OpenCV está correctamente instalado:
```bash
g++ -o test_opencv test.cpp `pkg-config --cflags --libs opencv4`
```
Si no hay errores de compilación, OpenCV está listo para usarse.

Para verificar los archivos de **STB Image**, ejecuta:
```bash
ls -l stb_image.h stb_image_write.h
```
Si los archivos existen en el directorio, la instalación fue exitosa.

---

