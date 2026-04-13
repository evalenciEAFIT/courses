/* ============================================================
 *  huffman.c  —  Implementación del algoritmo de Huffman
 * ============================================================ */

#include "huffman.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

/* ═══════════════════════════════════════════════════════════════════════════
 *  MIN-HEAP (cola de prioridad mínima)
 *  Usada para construir el árbol eficientemente: O(k log k)
 * ═══════════════════════════════════════════════════════════════════════════ */

static void heap_swap(MinHeap *h, int a, int b) {
    HuffNode *tmp = h->nodes[a];
    h->nodes[a]   = h->nodes[b];
    h->nodes[b]   = tmp;
}

/* Sube el nodo en posición i hasta su lugar correcto */
static void heap_sift_up(MinHeap *h, int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (h->nodes[parent]->freq > h->nodes[i]->freq) {
            heap_swap(h, parent, i);
            i = parent;
        } else break;
    }
}

/* Hunde el nodo en posición i hasta su lugar correcto */
static void heap_sift_down(MinHeap *h, int i) {
    while (1) {
        int left  = 2 * i + 1;
        int right = 2 * i + 2;
        int min   = i;
        if (left  < h->size && h->nodes[left]->freq  < h->nodes[min]->freq) min = left;
        if (right < h->size && h->nodes[right]->freq < h->nodes[min]->freq) min = right;
        if (min == i) break;
        heap_swap(h, i, min);
        i = min;
    }
}

static void heap_insert(MinHeap *h, HuffNode *node) {
    h->nodes[h->size++] = node;
    heap_sift_up(h, h->size - 1);
}

static HuffNode *heap_pop_min(MinHeap *h) {
    HuffNode *min   = h->nodes[0];
    h->nodes[0]     = h->nodes[--h->size];
    heap_sift_down(h, 0);
    return min;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  CREACIÓN DE NODOS
 * ═══════════════════════════════════════════════════════════════════════════ */
static HuffNode *new_leaf(uint8_t sym, long freq) {
    HuffNode *n = (HuffNode *)calloc(1, sizeof(HuffNode));
    n->symbol   = sym;
    n->freq     = freq;
    n->is_leaf  = 1;
    return n;
}

static HuffNode *new_internal(HuffNode *left, HuffNode *right) {
    HuffNode *n = (HuffNode *)calloc(1, sizeof(HuffNode));
    n->freq     = left->freq + right->freq;
    n->left     = left;
    n->right    = right;
    n->is_leaf  = 0;
    return n;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  GENERACIÓN RECURSIVA DE LA TABLA DE CÓDIGOS
 * ═══════════════════════════════════════════════════════════════════════════ */
static void build_codes(const HuffNode *node, char *path, int depth,
                         HuffCode table[256]) {
    if (!node) return;
    if (node->is_leaf) {
        path[depth] = '\0';
        strncpy(table[node->symbol].code, path, MAX_CODE_LEN - 1);
        table[node->symbol].length = depth;
        /* Caso especial: un único símbolo → código "0" */
        if (depth == 0) {
            table[node->symbol].code[0] = '0';
            table[node->symbol].code[1] = '\0';
            table[node->symbol].length  = 1;
        }
        return;
    }
    path[depth] = '0';
    build_codes(node->left,  path, depth + 1, table);
    path[depth] = '1';
    build_codes(node->right, path, depth + 1, table);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  VISUALIZACIONES PEDAGÓGICAS
 * ═══════════════════════════════════════════════════════════════════════════ */
void huff_print_freq(const long freq[256]) {
    printf(BOLD BCYAN "\n  ┌─ Tabla de Frecuencias ─────────────────────────────\n" RESET);
    printf(BOLD "  │ Se cuentan las ocurrencias de cada byte en el archivo.\n" RESET);
    printf(BOLD "  │ Sólo se muestran los que aparecen al menos una vez.\n" RESET);
    PRINT_THIN_SEP();
    printf(DIM "  %-6s %-6s %-8s  Histograma\n" RESET, "Byte", "Char", "Frec.");
    PRINT_DASH_SEP();

    /* Ordenar por frecuencia descendiente (burbuja simple — sólo 256 elems) */
    int order[256];
    for (int i = 0; i < 256; i++) order[i] = i;
    for (int i = 0; i < 255; i++)
        for (int j = i + 1; j < 256; j++)
            if (freq[order[j]] > freq[order[i]]) {
                int tmp = order[i]; order[i] = order[j]; order[j] = tmp;
            }

    long max_f = freq[order[0]];
    for (int k = 0; k < 256 && freq[order[k]] > 0; k++) {
        int    sym  = order[k];
        long   f    = freq[sym];
        char   disp = (sym >= 32 && sym < 127) ? (char)sym : '?';
        int    bar  = (max_f > 0) ? (int)((double)f / max_f * 30) : 0;

        printf("  0x%02X  " BWHITE "'%c'" RESET "  " BYELLOW "%6ld " RESET BGREEN,
               sym, disp, f);
        for (int b = 0; b < bar; b++) printf("▪");
        printf(RESET "\n");
    }
    printf(BOLD BCYAN "  └──────────────────────────────────────────────────\n" RESET);
}

void huff_print_table(const HuffCode table[256], const long freq[256]) {
    printf(BOLD BCYAN "\n  ┌─ Tabla de Códigos Huffman ──────────────────────────\n" RESET);
    printf(BOLD "  │ Asignación de códigos binarios óptimos.\n" RESET);
    printf(BOLD "  │ Menor frecuencia → código más largo.\n" RESET);
    PRINT_THIN_SEP();
    printf(DIM "  %-6s %-6s %-8s %-5s  Código Huffman\n" RESET,
           "Byte", "Char", "Frec.", "Bits");
    PRINT_DASH_SEP();

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            char disp = (i >= 32 && i < 127) ? (char)i : '?';
            printf("  0x%02X  " BWHITE "'%c'" RESET
                   "  " BYELLOW "%6ld " RESET
                   "  " BCYAN "%3d  " RESET
                   BGREEN "%s" RESET "\n",
                   i, disp, freq[i], table[i].length, table[i].code);
        }
    }
    printf(BOLD BCYAN "  └────────────────────────────────────────────────────\n" RESET);
}

void huff_print_tree(const HuffNode *node, int depth, char branch) {
    if (!node) return;
    /* Imprime el subárbol derecho primero (se ve "girado" en consola) */
    huff_print_tree(node->right, depth + 1, '/');

    for (int i = 0; i < depth * 4; i++) printf(" ");
    if (depth > 0) printf("%c── ", branch);

    if (node->is_leaf) {
        char disp = (node->symbol >= 32 && node->symbol < 127)
                    ? (char)node->symbol : '?';
        printf(BGREEN "[%c|%ld]" RESET "\n", disp, node->freq);
    } else {
        printf(BYELLOW "(%ld)" RESET "\n", node->freq);
    }
    huff_print_tree(node->left, depth + 1, '\\');
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  COMPRESIÓN
 * ═══════════════════════════════════════════════════════════════════════════ */
HuffResult huff_compress(const uint8_t *input, long in_size, HuffMode mode) {
    HuffResult result;
    memset(&result, 0, sizeof(result));
    if (!input || in_size <= 0) return result;

    /* ── Fase 1: Frecuencias ─────────────────────────────────────────────── */
    for (long i = 0; i < in_size; i++) result.freq[input[i]]++;

    if (mode == HUFF_VERBOSE) {
        printf(BOLD BBLUE "\n  ╔══ HUFFMAN — Fase 1: Conteo de Frecuencias ════════╗\n" RESET);
        huff_print_freq(result.freq);
        pause_step("Ver construcción del árbol...");
    }

    /* ── Fase 2: Construir árbol con Min-Heap ───────────────────────────── */
    MinHeap heap;
    heap.size = 0;

    for (int i = 0; i < 256; i++) {
        if (result.freq[i] > 0)
            heap_insert(&heap, new_leaf((uint8_t)i, result.freq[i]));
    }

    if (mode == HUFF_VERBOSE) {
        printf(BOLD BBLUE "\n  ╔══ HUFFMAN — Fase 2: Construcción del Árbol ═══════╗\n" RESET);
        printf(DIM "  Cola de prioridad inicial: %d nodos\n" RESET, heap.size);
        printf(DIM "  Se fusionan los DOS nodos de menor frecuencia en cada paso.\n\n" RESET);
        int step = 0;
        (void)step; /* se usa abajo */
    }

    int merge_step = 0;
    while (heap.size > 1) {
        HuffNode *left  = heap_pop_min(&heap);
        HuffNode *right = heap_pop_min(&heap);
        HuffNode *parent = new_internal(left, right);

        if (mode == HUFF_VERBOSE) {
            merge_step++;
            char ld = (left->is_leaf  && left->symbol  >= 32) ? (char)left->symbol  : '?';
            char rd = (right->is_leaf && right->symbol >= 32) ? (char)right->symbol : '?';
            printf("  Paso %2d: fusiona (freq=" BYELLOW "%ld" RESET,
                   merge_step, left->freq);
            if (left->is_leaf)  printf(" sym='%c'", ld);
            printf(") + (freq=" BYELLOW "%ld" RESET, right->freq);
            if (right->is_leaf) printf(" sym='%c'", rd);
            printf(") → nodo interno (freq=" BGREEN "%ld" RESET ")\n", parent->freq);

            if (merge_step % 8 == 0)
                pause_step("Seguir fusionando...");
        }

        heap_insert(&heap, parent);
    }

    result.tree_root = (heap.size == 1) ? heap_pop_min(&heap) : NULL;

    if (mode == HUFF_VERBOSE) {
        printf(BOLD BBLUE "\n  ╔══ HUFFMAN — Árbol final ═══════════════════════════╗\n" RESET);
        printf(DIM "  (Árbol rotado 90°: izquierda = arriba, derecha = abajo)\n" RESET);
        printf(DIM "  Rama '\\' = 0 (izquierda), Rama '/' = 1 (derecha)\n\n" RESET);
        huff_print_tree(result.tree_root, 0, ' ');
        pause_step("Ver tabla de códigos...");
    }

    /* ── Fase 3: Generar tabla de códigos ───────────────────────────────── */
    char path[MAX_CODE_LEN] = {0};
    build_codes(result.tree_root, path, 0, result.table);

    if (mode == HUFF_VERBOSE) {
        printf(BOLD BBLUE "\n  ╔══ HUFFMAN — Fase 3: Tabla de Códigos ══════════════╗\n" RESET);
        huff_print_table(result.table, result.freq);
        pause_step("Ver codificación del archivo...");
    }

    /* ── Fase 4: Empaquetar bits ─────────────────────────────────────────── */
    /* Calculamos el número total de bits */
    long total_bits = 0;
    for (long i = 0; i < in_size; i++)
        total_bits += result.table[input[i]].length;

    long out_bytes = (total_bits + 7) / 8;
    uint8_t *out   = (uint8_t *)calloc(out_bytes + 1, 1);
    if (!out) return result;

    long bit_pos = 0;
    for (long i = 0; i < in_size; i++) {
        const char *code = result.table[input[i]].code;
        int         clen = result.table[input[i]].length;

        if (mode == HUFF_VERBOSE && i < 16) {
            char disp = (input[i] >= 32 && input[i] < 127) ? (char)input[i] : '?';
            printf("  Byte[%3ld] '%c' → " BGREEN "%s" RESET "\n", i, disp, code);
        }
        if (mode == HUFF_VERBOSE && i == 16)
            printf(DIM "  ... (mostrando sólo los primeros 16 bytes)\n" RESET);

        for (int b = 0; b < clen; b++) {
            if (code[b] == '1') {
                out[bit_pos / 8] |= (1 << (7 - (bit_pos % 8)));
            }
            bit_pos++;
        }
    }

    result.data      = out;
    result.size      = out_bytes;
    result.bit_count = total_bits;

    if (mode == HUFF_VERBOSE) {
        printf(BOLD BBLUE "  ╚══ FIN COMPRESIÓN HUFFMAN ══════════════════════════╝\n" RESET);
        printf("  Bits totales generados: " BYELLOW "%ld" RESET
               " → %ld bytes\n", total_bits, out_bytes);
    }

    return result;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  DESCOMPRESIÓN
 * ═══════════════════════════════════════════════════════════════════════════ */
HuffDecompResult huff_decompress(const uint8_t *compressed, long comp_size,
                                  long bit_count, HuffNode *tree_root,
                                  HuffMode mode) {
    HuffDecompResult result = { NULL, 0 };
    if (!compressed || !tree_root || bit_count <= 0) return result;
    (void)comp_size; /* el tamaño en bytes no se usa; se trabaja con bit_count */

    /* Estimamos el buffer de salida (en el peor caso = bit_count bytes) */
    uint8_t *out = (uint8_t *)malloc(bit_count + 1);
    if (!out) return result;
    long out_idx = 0;

    if (mode == HUFF_VERBOSE) {
        printf(BOLD BBLUE "\n  ╔══ HUFFMAN — DESCOMPRESIÓN ══════════════════════════╗\n" RESET);
        printf(DIM "  Recorre el árbol bit a bit.\n"
               "  '0' → rama izquierda, '1' → rama derecha.\n"
               "  Al llegar a una hoja, emite el símbolo.\n\n" RESET);
    }

    HuffNode *curr = tree_root;
    int syms_shown = 0;

    for (long bit = 0; bit < bit_count; bit++) {
        int bval = (compressed[bit / 8] >> (7 - (bit % 8))) & 1;

        curr = bval ? curr->right : curr->left;
        if (!curr) break;   /* árbol malformado */

        if (curr->is_leaf) {
            if (mode == HUFF_VERBOSE && syms_shown < 16) {
                char disp = (curr->symbol >= 32 && curr->symbol < 127)
                            ? (char)curr->symbol : '?';
                printf("  Bit %4ld → hoja → '" BGREEN "%c" RESET
                       "' (0x%02X)\n", bit, disp, curr->symbol);
                syms_shown++;
                if (syms_shown == 16)
                    printf(DIM "  ... (sólo se muestran 16 símbolos)\n" RESET);
            }
            out[out_idx++] = curr->symbol;
            curr = tree_root;   /* reinicia desde la raíz */
        }
    }

    out[out_idx] = '\0';
    result.data  = out;
    result.size  = out_idx;

    if (mode == HUFF_VERBOSE) {
        printf(BOLD BBLUE "  ╚══ FIN DESCOMPRESIÓN HUFFMAN ════════════════════════╝\n" RESET);
        printf("  Bytes restaurados: " BCYAN "%ld" RESET "\n", out_idx);
    }

    return result;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  LIBERACIÓN DE MEMORIA DEL ÁRBOL
 * ═══════════════════════════════════════════════════════════════════════════ */
void huff_free_tree(HuffNode *node) {
    if (!node) return;
    huff_free_tree(node->left);
    huff_free_tree(node->right);
    free(node);
}
