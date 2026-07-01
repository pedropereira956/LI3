#include "tests/testQ1.h"
#include "menuNdata/queries.h"  
#include "IO/output.h"
#include "Catalogos/manager_c.h"
#include "Catalogos/airports_c.h"
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> // Necessário para struct timespec

void query1_test(MANAGER_C manager) {

    FILE* analysis = fopen("resultados/analysisTest.txt", "w");
    if (!analysis) {
        perror("Erro ao criar analysisTest.txt");
        return;
    }

    struct timespec start, end;
    double elapsed;

    fprintf(analysis, "=== TESTES DA QUERY 1 ===\n\n");

    // --------------------------------------------------------
    // 1. ARGUMENTO VÁLIDO (Formato Normal ';')
    // --------------------------------------------------------
    char** args_valid = malloc(2 * sizeof(char*)); // Aloca 2 para garantir NULL no fim
    args_valid[0] = "OPO";
    args_valid[1] = NULL;

    clock_gettime(CLOCK_REALTIME, &start);
    
    // Passamos 0 para formato padrão (;)
    void* r1 = query1(manager, args_valid, 0); 
    
    clock_gettime(CLOCK_REALTIME, &end);

    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    fprintf(analysis, "[VALID - Format 0 (;)] 'OPO'\n");
    fprintf(analysis, "Tempo: %.6f s\n", elapsed);
    
    // Pequeno debug no ficheiro para veres o resultado
    if (r1) fprintf(analysis, "Output: %s\n", (char*)r1); 
    else fprintf(analysis, "Output: NULL\n");

    if (r1) free(r1); // Em vez de free_query, usamos free direto pois sabemos que é char*

    // --------------------------------------------------------
    // 2. ARGUMENTO VÁLIDO (Formato 'S' '=')
    // --------------------------------------------------------
    // Reutilizamos os args, mas testamos o formato 1
    
    clock_gettime(CLOCK_REALTIME, &start);
    
    // Passamos 1 para formato especial (=)
    void* r1s = query1(manager, args_valid, 1); 
    
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    fprintf(analysis, "[VALID - Format 1 (=)] 'OPO'\n");
    fprintf(analysis, "Tempo: %.6f s\n", elapsed);
    if (r1s) fprintf(analysis, "Output: %s\n", (char*)r1s);

    if (r1s) free(r1s);
    free(args_valid);

    // --------------------------------------------------------
    // 3. ARGUMENTO INVÁLIDO
    // --------------------------------------------------------
    char** args_invalid = malloc(2 * sizeof(char*));
    args_invalid[0] = "ZZZ";
    args_invalid[1] = NULL;

    clock_gettime(CLOCK_REALTIME, &start);
    
    // Tanto faz o formato aqui, deve dar vazio
    void* r2 = query1(manager, args_invalid, 0); 
    
    clock_gettime(CLOCK_REALTIME, &end);

    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    fprintf(analysis, "[INVALID] 'ZZZ'\n");
    fprintf(analysis, "Tempo: %.6f s\n", elapsed);
    if (r2) fprintf(analysis, "Output: %s\n", (char*)r2); // Deve ser apenas \n

    if (r2) free(r2);
    free(args_invalid);

    fclose(analysis);
    printf("Teste Q1 concluido. Verifica 'resultados/analysisTest.txt'.\n");
}