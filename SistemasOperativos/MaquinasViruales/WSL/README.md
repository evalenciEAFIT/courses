# Guía para Instalar el Subsistema de Windows para Linux (WSL)

El Subsistema de Windows para Linux (WSL) permite ejecutar un entorno Linux directamente en Windows, sin necesidad de una máquina virtual o dual boot. Esto es útil para desarrolladores y administradores que necesitan herramientas de Linux en un sistema Windows.

---

## Prerrequisitos

1. **Sistema Operativo**:
   - Windows 10 (versión 2004 y posteriores, build 19041 y superiores).
   - Windows 11.

2. **Privilegios de Administrador**:
   - Necesitarás acceso como administrador para habilitar las características de Windows y configurar WSL.

---

## Pasos para Instalar WSL

### 1. Habilitar las Características de Windows

1. Abre PowerShell como administrador.
   - Haz clic derecho en el botón de inicio y selecciona **Windows PowerShell (Admin)**.

2. Ejecuta el siguiente comando para habilitar WSL:
   ```powershell
   dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
   ```

3. Habilita la plataforma de máquina virtual (necesario para WSL 2):
   ```powershell
   dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
   ```

4. Reinicia tu equipo para aplicar los cambios.

### 2. Instalar el Kernel de WSL 2

1. Descarga el paquete de actualización del kernel de Linux desde:
   [Actualización del Kernel de WSL 2](https://aka.ms/wsl2kernel).

2. Ejecuta el instalador descargado y sigue las instrucciones.

### 3. Establecer WSL 2 como Versión Predeterminada

1. Abre PowerShell como administrador.

2. Configura WSL 2 como la versión predeterminada:
   ```powershell
   wsl --set-default-version 2
   ```

### 4. Instalar una Distribución de Linux

1. Abre la Microsoft Store.

2. Busca la distribución que deseas instalar, como:
   - **Ubuntu**
   - **Debian**
   - **Kali Linux**
   - **openSUSE**

3. Haz clic en **Obtener** para descargar e instalar la distribución.

4. Una vez instalada, ábrela desde el menú de inicio y sigue las instrucciones para configurarla (por ejemplo, crear un usuario y contraseña).

---

## Comandos Básicos para Administrar WSL

### Listar las Distribuciones Instaladas
```powershell
wsl --list --verbose
```

### Establecer una Distribución como Predeterminada
```powershell
wsl --set-default <NombreDeDistribución>
```

### Cambiar la Versión de WSL (1 o 2)
```powershell
wsl --set-version <NombreDeDistribución> 2
```

### Iniciar una Distribución Específica
```powershell
wsl -d <NombreDeDistribución>
```

### Detener WSL
```powershell
wsl --shutdown
```

---

## Recursos Adicionales

### Videos para Instalar WSL
1. [Instalar WSL 2 en Windows 11 - Paso a Paso](https://youtu.be/uJg5HVzNUN0?si=8otIUlJGsBu8BOyY)   
   [![Ver en YouTube](https://img.youtube.com/vi/uJg5HVzNUN0/0.jpg)](https://www.youtube.com/watch?v=uJg5HVzNUN0)   
   Video detallado que explica cómo habilitar y configurar WSL 2.
2. [Linux en Windows y Windows en Linux](https://youtu.be/Qy44XLpiChc?si=cup54IbBmQYU073k)   
   [![Ver en YouTube](https://img.youtube.com/vi/Qy44XLpiChc/0.jpg)](https://www.youtube.com/watch?v=Qy44XLpiChc) 
   

### Documentación Oficial

- [Documentación Oficial de WSL](https://docs.microsoft.com/es-es/windows/wsl/) - Información detallada sobre WSL.
- [Descargar Distribuciones desde la Microsoft Store](https://aka.ms/wslstore) - Acceso directo a las distribuciones disponibles.
- [Kernel de WSL 2](https://aka.ms/wsl2kernel) - Actualizaciones para el kernel de WSL 2.
