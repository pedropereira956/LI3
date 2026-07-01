#include "tests/testQ2.h"
#include "menuNdata/queries.h"
#include "IO/output.h"   
#include "Catalogos/manager_c.h"
#include "Catalogos/aircrafts_c.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Para struct timespec

void query2_test(MANAGER_C manager) {

    FILE* analysis = fopen("resultados/analysisTest.txt", "a");
    if (!analysis) {
        perror("Erro ao abrir analysisTest.txt");
        return;
    }
    
    fprintf(analysis, "=== TESTES DA QUERY 2 ===\n\n");

    struct timespec start, end;
    double elapsed;

    // ---------- Teste 1: sem filtro (Formato Padrão ';') ----------
    char* args1[] = { "5", NULL };

    clock_gettime(CLOCK_REALTIME, &start);
    // Adicionado o '0' para indicar formato padrão
    void* r1 = query2(manager, args1, 0); 
    clock_gettime(CLOCK_REALTIME, &end);

    elapsed = (end.tv_sec - start.tv_sec) +
              (end.tv_nsec - start.tv_nsec) / 1e9;

    fprintf(analysis, "[TOP 5 SEM FILTRO - Default ';']\nTempo: %.6f s\n", elapsed);
    if (r1) {
        // Opcional: imprimir preview
        // fprintf(analysis, "Preview: %.50s...\n", (char*)r1);
        free(r1); // Usamos free() porque query2 retorna char*
    }
    fprintf(analysis, "\n");

    // ---------- Teste 2: com filtro (Formato Padrão ';') ----------
    char* args2[] = { "5", "Boeing", NULL };

    clock_gettime(CLOCK_REALTIME, &start);
    // Adicionado o '0'
    void* r2 = query2(manager, args2, 0);
    clock_gettime(CLOCK_REALTIME, &end);

    elapsed = (end.tv_sec - start.tv_sec) +
              (end.tv_nsec - start.tv_nsec) / 1e9;

    fprintf(analysis, "[TOP 5 FABRICANTE 'Boeing' - Default ';']\nTempo: %.6f s\n", elapsed);
    if (r2) free(r2);
    fprintf(analysis, "\n");

    // ---------- Teste 3: com filtro (Formato Especial '=') ----------
    // Reutilizamos args2 ("5", "Boeing")
    
    clock_gettime(CLOCK_REALTIME, &start);
    // Adicionado o '1' para testar o separador '='
    void* r3 = query2(manager, args2, 1);
    clock_gettime(CLOCK_REALTIME, &end);

    elapsed = (end.tv_sec - start.tv_sec) +
              (end.tv_nsec - start.tv_nsec) / 1e9;

    fprintf(analysis, "[TOP 5 FABRICANTE 'Boeing' - Format '=']\nTempo: %.6f s\n", elapsed);
    
    // Debug: Verifica se o separador está correto no output
    if (r3) {
        char *ptr = strchr((char*)r3, '=');
        if (ptr) fprintf(analysis, "VERIFICAÇÃO: Separador '=' ENCONTRADO.\n");
        else     fprintf(analysis, "VERIFICAÇÃO: Separador '=' NÃO ENCONTRADO.\n");
        free(r3);
    }
    fprintf(analysis, "\n");

    fclose(analysis);
    printf("Teste Q2 concluido.\n");
}