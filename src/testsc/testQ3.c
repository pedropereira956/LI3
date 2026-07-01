#include "tests/testQ3.h"
#include "IO/output.h"
#include "menuNdata/queries.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h> // Para struct timespec

void query3_test(MANAGER_C manager) {

    FILE* analysis = fopen("resultados/analysisTest.txt", "a");
    if (!analysis) {
        perror("Erro ao abrir analysisTest.txt");
        return;
    }

    fprintf(analysis, "=== TESTES DA QUERY 3 ===\n\n");

    struct timespec start, end;
    double elapsed;

    // ---------- Teste 1: Intervalo com voos (Formato Padrão ';') ----------
    char* args1[] = { "2023-01-01", "2023-12-31", NULL };

    clock_gettime(CLOCK_REALTIME, &start);
    // Adicionado o '0' (Formato padrão)
    void* r1 = query3(manager, args1, 0);
    clock_gettime(CLOCK_REALTIME, &end);

    elapsed = (end.tv_sec - start.tv_sec) +
              (end.tv_nsec - start.tv_nsec) / 1e9;

    fprintf(analysis, "[INTERVALO REAL - Default ';']\nTempo: %.6f s\n", elapsed);
    if (r1) {
        // Opcional: mostrar output no log
        // fprintf(analysis, "Output: %s", (char*)r1);
        free(r1); // Libertamos a string diretamente
    }
    fprintf(analysis, "\n");

    // ---------- Teste 2: Intervalo sem voos (Formato Padrão ';') ----------
    char* args2[] = { "1900-01-01", "1900-12-31", NULL };

    clock_gettime(CLOCK_REALTIME, &start);
    // Adicionado o '0'
    void* r2 = query3(manager, args2, 0);
    clock_gettime(CLOCK_REALTIME, &end);

    elapsed = (end.tv_sec - start.tv_sec) +
              (end.tv_nsec - start.tv_nsec) / 1e9;

    fprintf(analysis, "[INTERVALO SEM VOOS]\nTempo: %.6f s\n", elapsed);
    if (r2) free(r2); // Deve ser apenas um \n ou vazio
    fprintf(analysis, "\n");

    // ---------- Teste 3: Intervalo com voos (Formato Especial '=') ----------
    // Reutilizamos args1
    
    clock_gettime(CLOCK_REALTIME, &start);
    // Adicionado o '1' (Formato especial)
    void* r3 = query3(manager, args1, 1);
    clock_gettime(CLOCK_REALTIME, &end);

    elapsed = (end.tv_sec - start.tv_sec) +
              (end.tv_nsec - start.tv_nsec) / 1e9;

    fprintf(analysis, "[INTERVALO REAL - Format '=']\nTempo: %.6f s\n", elapsed);
    
    if (r3) {
        char *ptr = strchr((char*)r3, '=');
        if (ptr) fprintf(analysis, "VERIFICAÇÃO: Separador '=' ENCONTRADO.\n");
        else     fprintf(analysis, "VERIFICAÇÃO: Separador '=' NÃO ENCONTRADO.\n");
        free(r3);
    }
    fprintf(analysis, "\n");

    fclose(analysis);
    printf("Teste Q3 concluido.\n");
}