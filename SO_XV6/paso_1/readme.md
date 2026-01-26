# **Guía Completa: Crear "Hola Mundo" en XV6 desde Cero**

## **🎬 PASO 1: INICIAR WSL UBUNTU**

1. **Presiona `Win + X`** → "Terminal de Windows (Admin)"
2. **Iniciar WSL:**
```powershell
wsl
```
3. **O abrir directamente Ubuntu desde el menú Inicio**
   - Busca "Ubuntu" y haz clic
   - Si no tienes: `wsl --install -d Ubuntu-22.04`

---

## **📦 PASO 2: VERIFICAR INSTALACIÓN PREVIA**

```bash
# Verificar que XV6 esté instalado
cd ~
ls -la | grep xv6

# Si no existe xv6-public, clonarlo:
git clone https://github.com/mit-pdos/xv6-public.git
cd xv6-public

# Verificar dependencias
make clean
make
```

---

## **🚀 PASO 3: INICIAR QEMU CON XV6**

```bash
# Desde el directorio xv6-public
make qemu
```

**Verás:**
```
Booting from Hard Disk...
xv6...
cpu0: starting
init: starting sh
$ 
```

**Para salir:** `Ctrl + A`, luego `X`

---

## **👨‍💻 PASO 4: CREAR PROGRAMA "HOLA MUNDO"**

### **4.1. Salir de QEMU y crear archivo:**
```bash
# Salir de QEMU (Ctrl+A X)
# Crear archivo hola.c en directorio user/
cd ~/xv6/xv6-public/user
code hola.c
```

### **4.2. Contenido de hola.c:**
```c
#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    printf(1, "¡Hola Mundo desde XV6!\n");
    printf(1, "Argumentos recibidos: %d\n", argc);
    
    for(int i = 0; i < argc; i++) {
        printf(1, "  argv[%d] = %s\n", i, argv[i]);
    }
    
    exit();
}
```

**Guardar archivo:** `Ctrl + S`

---

## **🔧 PASO 5: AGREGAR PROGRAMA AL MAKEFILE**

```bash
# Abrir Makefile
cd ~/xv6/xv6-public
code Makefile
```

### **Buscar la línea que dice:**
```
UPROGS=\
```

### **Agregar `_hola\` después de `_echo\`:**
```makefile
UPROGS=\
	_cat\
	_echo\
	_hola\    # ← Agregar esta línea
	_forktest\
	_grep\
	_init\
	_kill\
	_ln\
	_ls\
```

**Importante:** Mantener el formato con tabulaciones.

---

## **🔨 PASO 6: COMPILAR Y PROBAR**

### **6.1. Recompilar XV6:**
```bash
# Limpiar compilación anterior
make clean

# Compilar XV6 con el nuevo programa
make
```

### **6.2. Verificar que se creó hola:**
```bash
ls user/_hola
# Debe existir el archivo ejecutable
```

### **6.3. Ejecutar XV6:**
```bash
make qemu
```

---

## **🎯 PASO 7: PROBAR EL PROGRAMA EN XV6**

### **Dentro de XV6 (en el prompt `$`):**
```sh
# Ejecutar programa básico
$ hola

# Ejecutar con argumentos
$ hola mi nombre es pedro

# Ver todos los programas disponibles
$ ls

# Ver contenido de hola.c desde XV6
$ cat user/hola.c
```

**Salida esperada:**
```
$ hola
¡Hola Mundo desde XV6!
Argumentos recibidos: 1
  argv[0] = hola

$ hola prueba 123
¡Hola Mundo desde XV6!
Argumentos recibidos: 3
  argv[0] = hola
  argv[1] = prueba
  argv[2] = 123
```

---

## **💡 PASO 8: MODIFICAR Y REITERAR**

### **8.1. Modificar hola.c:**
```bash
# Salir de QEMU (Ctrl+A X)
cd ~/xv6/xv6-public/user
code hola.c
```

### **8.2. Agregar más funcionalidad:**
```c
#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    printf(1, "\n================================\n");
    printf(1, "PROGRAMA HOLA MUNDO MEJORADO\n");
    printf(1, "================================\n\n");
    
    printf(1, "ID del proceso: %d\n", getpid());
    printf(1, "PID del padre: %d\n", getppid());
    
    if(argc > 1) {
        printf(1, "Has dicho: ");
        for(int i = 1; i < argc; i++) {
            printf(1, "%s ", argv[i]);
        }
        printf(1, "\n");
    } else {
        printf(1, "Usa: hola [tu mensaje]\n");
    }
    
    printf(1, "\n¡Adiós!\n");
    exit();
}
```

### **8.3. Recompilar rápidamente:**
```bash
# Solo recompilar (sin limpiar todo)
make
make qemu
```

---

## **🔍 PASO 9: DEBUGGAR EL PROGRAMA**

### **9.1. Ejecutar en modo debug:**
```bash
# Terminal 1:
make qemu-gdb
```

### **9.2. En otra terminal de WSL:**
```bash
cd ~/xv6/xv6-public
gdb
```
```gdb
(gdb) target remote localhost:26000
(gdb) file user/_hola
(gdb) break main
(gdb) continue
(gdb) step
(gdb) print argc
(gdb) quit
```

---

## **📁 PASO 10: CREAR MÁS PROGRAMAS DE EJEMPLO**

### **10.1. Programa "suma.c":**
```c
#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf(1, "Uso: suma num1 num2\n");
        exit();
    }
    
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int resultado = a + b;
    
    printf(1, "%d + %d = %d\n", a, b, resultado);
    exit();
}
```

### **10.2. Agregar al Makefile:**
```makefile
UPROGS=\
	_cat\
	_echo\
	_hola\
	_suma\    # ← Nueva línea
	_forktest\
```

### **10.3. Probar:**
```sh
$ suma 5 7
5 + 7 = 12
```

---

## **🚨 SOLUCIÓN DE PROBLEMAS**

### **Problema: "make: Nothing to be done for 'all'"**
```bash
make clean
make
```

### **Problema: "hola: not found"**
```bash
# Verificar que está en UPROGS del Makefile
# Verificar que se compiló:
ls user/_hola
```

### **Problema: Error de compilación**
```bash
# Verificar includes
grep -n "#include" user/hola.c

# Verificar sintaxis
cd ~/xv6/xv6-public
make clean
make
```

---

## **📋 RESUMEN DE COMANDOS ESENCIALES**

```bash
# Iniciar WSL
wsl

# Navegar a XV6
cd ~/xv6/xv6-public

# Compilar
make clean && make

# Ejecutar
make qemu

# Crear nuevo programa
code user/nuevo.c

# Agregar al Makefile (línea UPROGS)
code Makefile

# Depurar
make qemu-gdb
```

---

## **✅ CHECKLIST FINAL**

- [ ] WSL Ubuntu iniciado ✓
- [ ] XV6 compilado sin errores ✓
- [ ] Programa hola.c creado en user/ ✓
- [ ] Agregado `_hola\` al Makefile ✓
- [ ] XV6 ejecutándose en QEMU ✓
- [ ] Comando `hola` funciona dentro de XV6 ✓
- [ ] Puedes modificar y recompilar ✓

---

**🎉 ¡Felicidades!** Ahora tienes:
1. XV6 ejecutándose en WSL
2. QEMU emulando el sistema
3. Tu primer programa "Hola Mundo" personalizado
4. Capacidad para crear más programas

**Próximo paso:** Explora modificando otros programas como `ls.c` o `cat.c` para entender el sistema operativo desde dentro.
