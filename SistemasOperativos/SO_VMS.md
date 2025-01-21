# Sistemas Operativos y Máquinas Virtuales

## ¿Qué es un Sistema Operativo?

Un **sistema operativo (SO)** es un software fundamental que actúa como intermediario entre el hardware de una computadora y las aplicaciones o usuarios que la utilizan. Su función principal es gestionar los recursos del sistema (como la CPU, memoria, dispositivos de entrada/salida) y proporcionar un entorno estable y consistente para ejecutar programas.

### Funciones principales del sistema operativo
1. **Gestión de procesos**: Coordina la ejecución de programas y asigna recursos.
2. **Gestión de memoria**: Administra la memoria RAM, asignándola y liberándola según las necesidades de las aplicaciones.
3. **Gestión de almacenamiento**: Controla el acceso a sistemas de archivos y dispositivos de almacenamiento.
4. **Gestión de dispositivos**: Facilita la comunicación entre el hardware y el software a través de controladores (drivers).
5. **Gestión de usuarios**: Administra permisos y perfiles de los usuarios.
6. **Interfaz de usuario**: Proporciona una manera de interactuar con el sistema, ya sea mediante líneas de comandos (CLI) o interfaces gráficas (GUI).

---

## Clasificación de los Sistemas Operativos

### 1. Según su propósito
- **De propósito general**: Diseñados para ejecutar una amplia gama de aplicaciones. Ejemplo: Windows, macOS, Linux.
- **De propósito específico**: Diseñados para tareas concretas, como sistemas empotrados en electrodomésticos o automóviles.

### 2. Según su estructura
- **Monolíticos**: Todo el sistema operativo se ejecuta como un solo programa en modo kernel. Ejemplo: versiones iniciales de UNIX.
- **Microkernel**: El núcleo contiene solo las funciones esenciales, y el resto se implementa como procesos de usuario. Ejemplo: Minix, QNX.
- **Híbridos**: Combinan características de los monolíticos y microkernels. Ejemplo: Windows NT, macOS.

### 3. Según su interfaz de usuario
- **Basados en línea de comandos (CLI)**: Requieren que el usuario interactúe mediante comandos. Ejemplo: MS-DOS, terminal de Linux.
- **Basados en interfaces gráficas (GUI)**: Utilizan elementos visuales como ventanas e iconos. Ejemplo: Windows, macOS.

### 4. Según su capacidad de tareas
- **Monotarea**: Permiten ejecutar un solo programa a la vez. Ejemplo: MS-DOS.
- **Multitarea**: Soportan la ejecución de múltiples tareas simultáneamente. Ejemplo: Linux.

### 5. Según su capacidad de usuarios
- **Monousuario**: Diseñados para ser utilizados por un solo usuario a la vez. Ejemplo: versiones antiguas de macOS.
- **Multiusuario**: Permiten que múltiples usuarios accedan y utilicen los recursos del sistema simultáneamente. Ejemplo: UNIX.

### 6. Según su ubicación
- **De escritorio**: Diseñados para computadoras personales. Ejemplo: Windows, macOS.
- **Empotrados**: Usados en dispositivos especializados como televisores, cámaras o vehículos. Ejemplo: FreeRTOS.
- **De servidor**: Optimizados para manejar múltiples conexiones simultáneas y altas cargas de trabajo. Ejemplo: Windows Server, CentOS.
- **De tiempo real**: Responden rápidamente a eventos del entorno, con tiempos de respuesta garantizados. Ejemplo: VxWorks, RTLinux.

### 7. Según su diseño y filosofía
- **Abiertos**: Permiten acceso al código fuente, como Linux.
- **Cerrados**: Su código fuente no está disponible al público. Ejemplo: Windows, iOS.

---

## Componentes de los Sistemas Operativos

### 1. Núcleo o Kernel
- Es el **núcleo del sistema operativo** y se ejecuta en modo privilegiado (modo kernel).
- Gestiona directamente el hardware y los recursos del sistema.
- **Funciones principales**:
  - **Gestión de procesos**: Planificación, sincronización y comunicación entre procesos.
  - **Gestión de memoria**: Controla el uso y la asignación de la memoria.
  - **Gestión de dispositivos**: Interactúa con el hardware a través de controladores.
  - **Gestión de almacenamiento**: Maneja los sistemas de archivos.

### 2. Interfaz de usuario
- Permite que los usuarios interactúen con el sistema operativo.
- **Tipos principales**:
  - **CLI (Command Line Interface)**: Interacción basada en comandos. Ejemplo: terminal de Linux.
  - **GUI (Graphical User Interface)**: Interacción visual con ventanas, iconos y menús. Ejemplo: escritorio de Windows.

### 3. Gestor de procesos
- Coordina la ejecución de programas y tareas.
- Realiza la **planificación de procesos**, decide qué proceso ejecuta la CPU y cuándo.
- Maneja estados de procesos: listo, en ejecución, bloqueado.

### 4. Gestor de memoria
- Administra el uso de la memoria principal (RAM).
- Asigna y libera memoria para procesos.
- Implementa **técnicas de memoria virtual**, como paginación o segmentación.

### 5. Sistema de archivos
- Proporciona una estructura jerárquica para almacenar y recuperar datos en dispositivos de almacenamiento.
- Gestiona permisos de acceso y organiza los archivos en directorios.

### 6. Gestor de dispositivos
- Controla la comunicación entre el sistema operativo y los dispositivos de hardware (teclado, disco duro, impresoras, etc.).
- Utiliza **controladores** (drivers) para traducir comandos del SO a instrucciones comprensibles por el hardware.

### 7. Seguridad y protección
- Controla el acceso a recursos, previniendo accesos no autorizados.
- Gestiona usuarios, permisos y políticas de seguridad.

### 8. Red y comunicación
- Permite la comunicación entre computadoras a través de redes.
- Gestiona protocolos de red, transferencia de datos y configuraciones de red.

### 9. Módulos adicionales
- **Control de energía**: Gestiona el consumo energético, especialmente en dispositivos móviles.
- **Gestión de dispositivos de E/S**: Sincroniza las operaciones de entrada y salida.

---

## Máquinas Virtuales

Una **máquina virtual (VM)** es un entorno de software que emula un sistema informático completo, permitiendo ejecutar sistemas operativos y aplicaciones de forma aislada en el hardware físico.

### Componentes de una máquina virtual

1. **Hipervisor**
   - Software que permite ejecutar y gestionar múltiples máquinas virtuales en un único sistema físico.
   - **Tipos**:
     - **Hipervisor tipo 1**: Corre directamente sobre el hardware (bare-metal). Ejemplo: VMware ESXi, Microsoft Hyper-V.
     - **Hipervisor tipo 2**: Corre sobre un sistema operativo anfitrión. Ejemplo: VirtualBox, VMware Workstation.

2. **Máquina virtual invitada (Guest VM)**
   - Simula un sistema operativo completo.
   - Ejecuta aplicaciones como si estuviera en hardware real.

3. **Hardware virtualizado**
   - Incluye CPU, memoria, almacenamiento y dispositivos de red virtuales.
   - Emula hardware físico para cada máquina virtual.

4. **Archivos de disco virtual (VDI, VHD, QCOW2)**
   - Representan discos duros virtuales que almacenan el sistema operativo y datos de la VM.

### Ventajas de las máquinas virtuales
- **Aislamiento**: Cada VM opera de manera independiente, reduciendo riesgos de interferencia.
- **Consolidación**: Varias VM pueden ejecutarse en una sola máquina física.
- **Pruebas y desarrollo**: Permiten probar software o sistemas operativos sin afectar el sistema anfitrión.
- **Escalabilidad**: Facilitan la adición o eliminación de recursos según sea necesario.

### Desventajas
- **Rendimiento**: Pueden ser más lentas que los sistemas ejecutados en hardware real debido a la sobrecarga del hipervisor.
- **Requisitos de recursos**: Consumen más memoria y CPU que un sistema físico único.

---

## ¿Qué es WSL?

**WSL (Windows Subsystem for Linux)** es una capa de compatibilidad desarrollada por Microsoft que permite ejecutar distribuciones de Linux directamente sobre Windows, sin necesidad de máquinas virtuales o arranque dual.

### Características principales de WSL
- **Integración con Windows**: Permite acceder a archivos y recursos de Windows desde el entorno de Linux y viceversa.
- **Distribuciones compatibles**: Admite varias distribuciones de Linux como Ubuntu, Debian, Fedora, entre otras.
- **Ejecución eficiente**: WSL 2 utiliza un kernel Linux real dentro de un entorno virtualizado, ofreciendo mejor rendimiento que WSL 1.
- **Soporte de herramientas de desarrollo**: Ideal para desarrolladores que necesitan herramientas y entornos de Linux mientras trabajan en Windows.

### Ventajas de WSL
- Elimina la necesidad de software adicional como máquinas virtuales o arranque dual.
- Bajo consumo de recursos comparado con una máquina virtual.
- Fácil instalación desde la Microsoft Store.

### Limitaciones
- Algunas aplicaciones que dependen de hardware específico pueden no ser compatibles.
- Requiere la versión adecuada de Windows (Windows 10 o superior).

