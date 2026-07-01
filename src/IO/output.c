#include "IO/output.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <glib.h>

FILE* create_output_file(int cmd_n) {
    char filename[64];
    // Garante que os ficheiros são criados na pasta de resultados
    snprintf(filename, sizeof(filename), "resultados/command%d_output.txt", cmd_n);

    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("Erro ao criar ficheiro de output");
        return NULL;
    }

    return f;
}

void output_query(FILE* f, void* result, int query_id) {
    (void)query_id;
    if (!f) return;
    
    // Se a query devolver uma string (como a Q5 que gera Top N), imprimimos diretamente
    if (result && strlen((char*)result) > 0) {
        fprintf(f, "%s", (char*)result);
    } 
    else {
        // Para submissão oficial, se não houver resultados, o ficheiro deve ficar vazio
        // ou conter apenas um newline, dependendo da especificação da sua fase.
        // fprintf(f, ""); 
    }
}

void free_query(void* result, int query_id) {
    // Como usamos GString para a Query 5 e g_strdup para outras, 
    // o ponteiro retornado é memória heap simples que pode ser libertada com free()
    if (result) {
        g_free(result);
    }
}