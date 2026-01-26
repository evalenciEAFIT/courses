# **Guía Paso a Paso: Instalar QEMU y XV6 en WSL Ubuntu (Windows 11)**

## **PASO 1: Instalar WSL con Ubuntu**

### **1.1. Habilitar WSL en Windows 11**
1. Abre **PowerShell como Administrador** (Click derecho → "Ejecutar como administrador")
2. Ejecuta:
```powershell
wsl --install -d Ubuntu-22.04
```
3. Si ya tienes WSL instalado, solo instala Ubuntu:
```powershell
wsl --install -d Ubuntu-22.04
```

### **1.2. Alternativa si no funciona:**
```powershell
# Habilitar características de Windows
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart

# Reiniciar el equipo
```

### **1.3. Configurar WSL2 como predeterminado:**
```powershell
wsl --set-default-version 2
```

---

## **PASO 2: Configurar Ubuntu en WSL**

### **2.1. Abrir Ubuntu por primera vez**
1. Busca "Ubuntu" en el menú Inicio
2. La primera vez, te pedirá crear un usuario y contraseña
3. **Ejemplo:**
   ```
   Enter new UNIX username: tuusuario
   New password: [tucontraseña]
   Retype password: [tucontraseña]
   ```

### **2.2. Actualizar Ubuntu**
En la terminal de Ubuntu:
```bash
sudo apt update && sudo apt upgrade -y
```

---

## **PASO 3: Instalar Dependencias Necesarias**

### **3.1. Instalar herramientas de compilación y QEMU**
```bash
# Instalar paquetes esenciales
sudo apt install -y build-essential git gdb

# Instalar QEMU (emulador para ejecutar XV6)
sudo apt install -y qemu-system-x86 qemu-utils

# Instalar compilador de 32 bits (necesario para XV6)
sudo apt install -y gcc-multilib

# Verificar instalaciones
gcc --version
qemu-system-x86_64 --version
```

### **3.2. Instalar herramientas adicionales (opcional pero recomendado)**
```bash
sudo apt install -y wget curl vim make nasm
```

---

## **PASO 4: Clonar y Compilar XV6**

### **4.1. Clonar el repositorio de XV6**
```bash
# Crear directorio para proyectos
mkdir ~/xv6
cd ~/xv6

# Clonar repositorio oficial de XV6 (MIT)
git clone https://github.com/mit-pdos/xv6-public.git

# O clonar versión más reciente (recomendado):
git clone https://github.com/mit-pdos/xv6-riscv.git xv6-riscv

# Entrar al directorio
cd xv6-public
```

### **4.2. Compilar XV6**
```bash
# Compilar el sistema operativo XV6
make

# Si aparece error, prueba:
make clean
make
```

**Si hay errores de compilación:**
```bash
# Error común: falta gcc-8
sudo apt install gcc-8 g++-8
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 8

# O usar GCC actual
make CC=gcc
```

---

## **PASO 5: Ejecutar XV6 en QEMU**

### **5.1. Ejecutar XV6 por primera vez**
```bash
# Desde el directorio xv6-public/
make qemu
```

### **5.2. Si aparece error de QEMU:**
```bash
# Ejecutar manualmente
qemu-system-i386 -serial mon:stdio -hda xv6.img

# O con más memoria
qemu-system-i386 -drive file=xv6.img,index=0,media=disk,format=raw -m 512 -serial mon:stdio
```

---

## **PASO 6: Probar XV6**

### **6.1. Comandos dentro de XV6:**
Cuando XV6 esté ejecutándose, verás un prompt `$`. Prueba:
```
$ ls
$ cat README
$ echo "Hola XV6"
$ usertests
```

### **6.2. Salir de QEMU:**
Presiona:
```
CTRL + A, luego X
```
(No presiones CTRL+X directamente, es: CTRL+A, suelta, luego X)

---

## **PASO 7: Configurar Entorno de Desarrollo**

### **7.1. Instalar editor de texto (VS Code con WSL)**
1. Instala **Visual Studio Code** en Windows
2. Instala extensión "Remote - WSL"
3. En terminal de Ubuntu:
```bash
code .
```

### **7.2. Configurar Git (opcional)**
```bash
git config --global user.name "Tu Nombre"
git config --global user.email "tu@email.com"
```

---

## **PASO 8: Depurar XV6 (Opcional)**

### **8.1. Ejecutar XV6 en modo depuración**
**Terminal 1:**
```bash
make qemu-gdb
```

**Terminal 2 (en otra ventana de Ubuntu):**
```bash
cd ~/xv6/xv6-public
gdb
```
Dentro de GDB:
```
(gdb) target remote localhost:26000
(gdb) file kernel
(gdb) break main
(gdb) continue
```

---

## **PASO 9: Crear Scripts de Ejecución**

### **9.1. Crear script para facilitar ejecución**
```bash
# Crear script ejecutar_xv6.sh
cat > ~/ejecutar_xv6.sh << 'EOF'
#!/bin/bash
cd ~/xv6/xv6-public
make clean
make
make qemu
EOF

# Dar permisos de ejecución
chmod +x ~/ejecutar_xv6.sh

# Ejecutar
~/ejecutar_xv6.sh
```

---

## **PASO 10: Solucionar Problemas Comunes**

### **10.1. Error: "make: gcc: Command not found"**
```bash
sudo apt install gcc
```

### **10.2. Error: QEMU no abre ventana**
```bash
# Usar modo terminal
make qemu-nox
```

### **10.3. Error: "Could not open option rom"**
```bash
# Instalar bios de QEMU
sudo apt install seabios
```

### **10.4. WSL no tiene interfaz gráfica**
Si QEMU necesita GUI, instala un servidor X en Windows:
1. Descarga **VcXsrv** desde: https://sourceforge.net/projects/vcxsrv/
2. Instala y ejecuta XLaunch
3. En Ubuntu:
```bash
export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}'):0
```

---

## **PASO 11: Verificar Instalación Completa**

### **11.1. Prueba final**
```bash
cd ~/xv6/xv6-public
make clean
make
make qemu
```

Deberías ver:
```
Booting from Hard Disk...
xv6...
cpu0: starting
init: starting sh
$ 
```

---

## **Resumen de Comandos Esenciales**
```bash
# 1. Actualizar sistema
sudo apt update && sudo apt upgrade -y

# 2. Instalar dependencias
sudo apt install build-essential git gdb qemu-system-x86 gcc-multilib -y

# 3. Clonar XV6
cd ~
git clone https://github.com/mit-pdos/xv6-public.git

# 4. Compilar y ejecutar
cd xv6-public
make
make qemu
```

---

## **Recursos Adicionales**
1. **Documentación oficial de XV6**: https://pdos.csail.mit.edu/6.828/2022/xv6.html
2. **Guías de laboratorio MIT**: https://pdos.csail.mit.edu/6.828/2022/labs/util.html
3. **Código fuente comentado**: Revisa los archivos `.c` y `.h` en el directorio xv6-public/

¡Listo! Ahora tienes XV6 funcionando en WSL Ubuntu en Windows 11. Puedes modificar el código fuente y recompilar para experimentar con el sistema operativo.
