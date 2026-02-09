/*
 * ============================================================================
 * SOLUCIÓN EJERCICIO 2: Memoria Compartida con mmap()
 * ============================================================================
 * 
 * Objetivo:
 *   Implementar comunicación entre procesos (IPC) usando memoria compartida
 *   mapeada con mmap(). Dos procesos (padre e hijo) compartirán un buffer
 *   para intercambiar mensajes.
 * 
 * Conceptos demostrados:
 *   - Memoria compartida con MAP_SHARED
 *   - Comunicación entre procesos (IPC)
 *   - Sincronización básica con flags
 *   - fork() y espacios de direcciones compartidos
 * 
 * Compilación:
 *   gcc -Wall -Wextra -o ejercicio2_sol ejercicio2_shared_memory_solucion.c
 * 
 * Ejecución:
 *   ./ejercicio2_sol
 * ============================================================================
 */

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

#define PAGE_SIZE 4096
#define MAX_MESSAGE_SIZE 256

/*
 * ============================================================================
 * ESTRUCTURAS DE DATOS COMPARTIDAS
 * ============================================================================
 */

/**
 * Estructura de mensaje en memoria compartida
 * 
 * Layout:
 *   [ready_flag][sender_id][message_text]
 */
typedef struct shared_message {
    volatile int ready;              // Flag: 1 = mensaje listo, 0 = leído
    pid_t sender_pid;                // PID del proceso que envió el mensaje
    char message[MAX_MESSAGE_SIZE];  // Contenido del mensaje
} shared_message_t;

/**
 * Buffer circular para múltiples mensajes
 */
typedef struct shared_buffer {
    volatile int write_index;        // Índice de escritura
    volatile int read_index;         // Índice de lectura
    volatile int count;              // Número de mensajes pendientes
    int capacity;                    // Capacidad máxima
    shared_message_t messages[10];   // Array de mensajes
} shared_buffer_t;

/*
 * ============================================================================
 * FUNCIONES AUXILIARES
 * ============================================================================
 */

/**
 * Crea un área de memoria compartida usando mmap()
 */
void* create_shared_memory(size_t size) {
    void* mem = mmap(
        NULL,                           // Dirección: kernel elige
        size,                           // Tamaño
        PROT_READ | PROT_WRITE,        // Permisos: lectura + escritura
        MAP_SHARED | MAP_ANONYMOUS,    // SHARED (no PRIVATE!) + anónimo
        -1,                            // No file descriptor
        0                              // No offset
    );
    
    if (mem == MAP_FAILED) {
        perror("mmap failed");
        return NULL;
    }
    
    // Inicializar a cero
    memset(mem, 0, size);
    
    return mem;
}

/**
 * Espera activa simple (busy waiting)
 * Nota: En producción usarías semáforos o mutexes
 */
void wait_for_message(volatile int* ready_flag) {
    while (*ready_flag == 0) {
        usleep(1000);  // Dormir 1ms para no saturar CPU
    }
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 1: Comunicación Simple Padre-Hijo
 * ============================================================================
 */

void demo1_simple_communication() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Demo 1: Comunicación Simple Padre-Hijo                       ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    // Crear memoria compartida
    shared_message_t* shared_msg = (shared_message_t*)create_shared_memory(sizeof(shared_message_t));
    if (shared_msg == NULL) {
        fprintf(stderr, "Error creando memoria compartida\n");
        return;
    }
    
    printf("✅ Memoria compartida creada en: %p\n", (void*)shared_msg);
    printf("   Tamaño: %zu bytes\n\n", sizeof(shared_message_t));
    
    // Crear proceso hijo
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork failed");
        munmap(shared_msg, sizeof(shared_message_t));
        return;
    }
    
    if (pid == 0) {
        // ═══════════════════════════════════════════════════════════════
        // PROCESO HIJO
        // ═══════════════════════════════════════════════════════════════
        
        printf("[HIJO %d] Iniciado\n", getpid());
        printf("[HIJO %d] Esperando mensaje del padre...\n", getpid());
        
        // Esperar mensaje del padre
        wait_for_message(&shared_msg->ready);
        
        printf("[HIJO %d] ¡Mensaje recibido!\n", getpid());
        printf("[HIJO %d] De: PID %d\n", getpid(), shared_msg->sender_pid);
        printf("[HIJO %d] Mensaje: \"%s\"\n", getpid(), shared_msg->message);
        
        // Marcar como leído
        shared_msg->ready = 0;
        
        // Enviar respuesta
        sleep(1);
        shared_msg->sender_pid = getpid();
        strcpy(shared_msg->message, "¡Hola Padre! Mensaje recibido correctamente.");
        shared_msg->ready = 1;
        
        printf("[HIJO %d] Respuesta enviada\n", getpid());
        
        exit(0);
        
    } else {
        // ═══════════════════════════════════════════════════════════════
        // PROCESO PADRE
        // ═══════════════════════════════════════════════════════════════
        
        printf("[PADRE %d] Proceso hijo creado: PID %d\n", getpid(), pid);
        
        sleep(1);  // Dar tiempo al hijo para iniciar
        
        // Enviar mensaje al hijo
        printf("[PADRE %d] Enviando mensaje al hijo...\n", getpid());
        shared_msg->sender_pid = getpid();
        strcpy(shared_msg->message, "¡Hola Hijo! Este es un mensaje desde el padre.");
        shared_msg->ready = 1;
        
        // Esperar respuesta
        sleep(2);
        wait_for_message(&shared_msg->ready);
        
        printf("[PADRE %d] ¡Respuesta recibida!\n", getpid());
        printf("[PADRE %d] De: PID %d\n", getpid(), shared_msg->sender_pid);
        printf("[PADRE %d] Mensaje: \"%s\"\n", getpid(), shared_msg->message);
        
        // Esperar a que el hijo termine
        wait(NULL);
        
        printf("\n[PADRE %d] Hijo terminado. Limpiando...\n", getpid());
        
        // Liberar memoria compartida
        munmap(shared_msg, sizeof(shared_message_t));
        
        printf("✅ Demo 1 completada\n");
    }
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 2: Buffer Circular con Múltiples Mensajes
 * ============================================================================
 */

void demo2_circular_buffer() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Demo 2: Buffer Circular con Múltiples Mensajes               ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    // Crear buffer compartido
    shared_buffer_t* buffer = (shared_buffer_t*)create_shared_memory(sizeof(shared_buffer_t));
    if (buffer == NULL) {
        fprintf(stderr, "Error creando buffer compartido\n");
        return;
    }
    
    buffer->capacity = 10;
    buffer->write_index = 0;
    buffer->read_index = 0;
    buffer->count = 0;
    
    printf("✅ Buffer circular creado\n");
    printf("   Capacidad: %d mensajes\n", buffer->capacity);
    printf("   Tamaño total: %zu bytes\n\n", sizeof(shared_buffer_t));
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // ═══════════════════════════════════════════════════════════════
        // PROCESO HIJO (PRODUCTOR)
        // ═══════════════════════════════════════════════════════════════
        
        printf("[PRODUCTOR %d] Iniciado\n", getpid());
        
        for (int i = 0; i < 5; i++) {
            // Esperar si el buffer está lleno
            while (buffer->count >= buffer->capacity) {
                usleep(1000);
            }
            
            // Escribir mensaje
            int idx = buffer->write_index;
            buffer->messages[idx].sender_pid = getpid();
            snprintf(buffer->messages[idx].message, MAX_MESSAGE_SIZE, 
                     "Mensaje #%d desde el productor", i + 1);
            buffer->messages[idx].ready = 1;
            
            // Actualizar índices
            buffer->write_index = (buffer->write_index + 1) % buffer->capacity;
            __sync_fetch_and_add(&buffer->count, 1);
            
            printf("[PRODUCTOR %d] Enviado: \"%s\"\n", 
                   getpid(), buffer->messages[idx].message);
            
            usleep(500000);  // 500ms entre mensajes
        }
        
        printf("[PRODUCTOR %d] Terminado\n", getpid());
        exit(0);
        
    } else {
        // ═══════════════════════════════════════════════════════════════
        // PROCESO PADRE (CONSUMIDOR)
        // ═══════════════════════════════════════════════════════════════
        
        printf("[CONSUMIDOR %d] Iniciado\n", getpid());
        
        int received = 0;
        while (received < 5) {
            // Esperar si el buffer está vacío
            while (buffer->count == 0) {
                usleep(1000);
            }
            
            // Leer mensaje
            int idx = buffer->read_index;
            if (buffer->messages[idx].ready) {
                printf("[CONSUMIDOR %d] Recibido: \"%s\" (de PID %d)\n",
                       getpid(), buffer->messages[idx].message,
                       buffer->messages[idx].sender_pid);
                
                buffer->messages[idx].ready = 0;
                buffer->read_index = (buffer->read_index + 1) % buffer->capacity;
                __sync_fetch_and_sub(&buffer->count, 1);
                
                received++;
            }
            
            usleep(800000);  // 800ms entre lecturas (más lento que productor)
        }
        
        printf("[CONSUMIDOR %d] Terminado\n", getpid());
        
        // Esperar al hijo
        wait(NULL);
        
        // Liberar memoria
        munmap(buffer, sizeof(shared_buffer_t));
        
        printf("\n✅ Demo 2 completada\n");
    }
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 3: Contador Compartido (Race Condition)
 * ============================================================================
 */

typedef struct shared_counter {
    volatile int value;
    volatile int iterations;
} shared_counter_t;

void demo3_race_condition() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Demo 3: Race Condition en Contador Compartido                ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    shared_counter_t* counter = (shared_counter_t*)create_shared_memory(sizeof(shared_counter_t));
    if (counter == NULL) {
        fprintf(stderr, "Error creando contador compartido\n");
        return;
    }
    
    counter->value = 0;
    counter->iterations = 100000;
    
    printf("⚠️  ADVERTENCIA: Esta demo muestra una RACE CONDITION\n");
    printf("   Dos procesos incrementarán el mismo contador sin sincronización\n\n");
    
    printf("Configuración:\n");
    printf("  - Valor inicial: %d\n", counter->value);
    printf("  - Iteraciones por proceso: %d\n", counter->iterations);
    printf("  - Valor esperado final: %d\n\n", counter->iterations * 2);
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // PROCESO HIJO
        for (int i = 0; i < counter->iterations; i++) {
            counter->value++;  // ¡RACE CONDITION!
        }
        exit(0);
    } else {
        // PROCESO PADRE
        for (int i = 0; i < counter->iterations; i++) {
            counter->value++;  // ¡RACE CONDITION!
        }
        
        wait(NULL);
        
        printf("Resultados:\n");
        printf("  - Valor esperado: %d\n", counter->iterations * 2);
        printf("  - Valor obtenido: %d\n", counter->value);
        
        if (counter->value == counter->iterations * 2) {
            printf("  - ✅ Correcto (¡suerte!)\n");
        } else {
            printf("  - ❌ Incorrecto (race condition detectada)\n");
            printf("  - Diferencia: %d incrementos perdidos\n", 
                   (counter->iterations * 2) - counter->value);
        }
        
        printf("\n💡 Solución: Usar __sync_fetch_and_add() o semáforos\n");
        
        munmap(counter, sizeof(shared_counter_t));
        printf("\n✅ Demo 3 completada\n");
    }
}

/*
 * ============================================================================
 * DEMOSTRACIÓN 4: Sincronización con Atomic Operations
 * ============================================================================
 */

void demo4_atomic_operations() {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Demo 4: Sincronización con Operaciones Atómicas              ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");
    
    shared_counter_t* counter = (shared_counter_t*)create_shared_memory(sizeof(shared_counter_t));
    if (counter == NULL) {
        fprintf(stderr, "Error creando contador compartido\n");
        return;
    }
    
    counter->value = 0;
    counter->iterations = 100000;
    
    printf("✅ Esta demo usa operaciones ATÓMICAS para evitar race conditions\n\n");
    
    printf("Configuración:\n");
    printf("  - Valor inicial: %d\n", counter->value);
    printf("  - Iteraciones por proceso: %d\n", counter->iterations);
    printf("  - Valor esperado final: %d\n\n", counter->iterations * 2);
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // PROCESO HIJO
        for (int i = 0; i < counter->iterations; i++) {
            __sync_fetch_and_add(&counter->value, 1);  // ATÓMICO
        }
        exit(0);
    } else {
        // PROCESO PADRE
        for (int i = 0; i < counter->iterations; i++) {
            __sync_fetch_and_add(&counter->value, 1);  // ATÓMICO
        }
        
        wait(NULL);
        
        printf("Resultados:\n");
        printf("  - Valor esperado: %d\n", counter->iterations * 2);
        printf("  - Valor obtenido: %d\n", counter->value);
        
        if (counter->value == counter->iterations * 2) {
            printf("  - ✅ Correcto (operaciones atómicas funcionan!)\n");
        } else {
            printf("  - ❌ Incorrecto (esto no debería pasar)\n");
        }
        
        munmap(counter, sizeof(shared_counter_t));
        printf("\n✅ Demo 4 completada\n");
    }
}

/*
 * ============================================================================
 * MAIN
 * ============================================================================
 */

int main() {
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  SOLUCIÓN EJERCICIO 2: Memoria Compartida                     ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n📚 Este programa demuestra 4 conceptos de memoria compartida:\n");
    printf("   1. Comunicación simple padre-hijo\n");
    printf("   2. Buffer circular productor-consumidor\n");
    printf("   3. Race condition (problema)\n");
    printf("   4. Operaciones atómicas (solución)\n");
    
    // Ejecutar demostraciones
    demo1_simple_communication();
    demo2_circular_buffer();
    demo3_race_condition();
    demo4_atomic_operations();
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  ✅ Todas las demostraciones completadas                      ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    printf("\n💡 Conceptos clave aprendidos:\n");
    printf("   • MAP_SHARED permite compartir memoria entre procesos\n");
    printf("   • fork() crea un nuevo proceso que hereda el mapeo compartido\n");
    printf("   • Se necesita sincronización para evitar race conditions\n");
    printf("   • Las operaciones atómicas son una solución simple\n");
    printf("   • En producción, usa semáforos o mutexes (pthread)\n");
    
    return 0;
}
