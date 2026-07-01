#include "Executavel/programaTestes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include <ctype.h>
#include <glib.h> 

#include "Catalogos/manager_c.h"
#include "menuNdata/queries.h"
#include "IO/output.h"

// ===================================================
// CORES PARA O TERMINAL
// ===================================================
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// ===================================================
// CONTADORES GLOBAIS (Atualizados para Q5 e Q6)
// ===================================================
static int total_Q1 = 0, ok_Q1 = 0;
static int total_Q2 = 0, ok_Q2 = 0;
static int total_Q3 = 0, ok_Q3 = 0;
static int total_Q5 = 0, ok_Q5 = 0;
static int total_Q6 = 0, ok_Q6 = 0;

static double time_Q1 = 0, time_Q2 = 0, time_Q3 = 0, time_Q5 = 0, time_Q6 = 0;
static double time_total = 0;

typedef struct {
    int query_n;
    int line_n;
} DIFF;

static DIFF diffs[4096];
static int diff_count = 0;

// ===================================================
// UTILITÁRIO DE TEMPO
// ===================================================
static inline double time_diff_ms(struct timespec a, struct timespec b) {
    return (b.tv_sec - a.tv_sec) * 1000.0 +
           (b.tv_nsec - a.tv_nsec) / 1e6;
}

// ===================================================
// COMPARAÇÃO DE FICHEIROS
// ===================================================
int compare_files_and_find_line(const char *out, const char *exp, int cmd_n) {
    FILE *A = fopen(out, "r");
    FILE *B = fopen(exp, "r");

    if (!A || !B) {
        if (A) fclose(A);
        if (B) fclose(B);
        return -1;
    }

    char la[4096], lb[4096];
    int line = 1;
    int erro = 0;

    while (1) {
        char *ra = fgets(la, sizeof(la), A);
        char *rb = fgets(lb, sizeof(lb), B);

        if (ra) la[strcspn(la, "\r\n")] = '\0';
        if (rb) lb[strcspn(lb, "\r\n")] = '\0';

        if (!ra && !rb) break;

        if (!ra || !rb || strcmp(la, lb) != 0) {
            printf(ANSI_COLOR_RED "[FALHA] Query %d (Linha %d):\n" ANSI_COLOR_RESET, cmd_n, line);
            printf("  Obtido:   \"%s\"\n", ra ? la : "<FIM>");
            printf("  Esperado: \"%s\"\n", rb ? lb : "<FIM>");
            erro = 1;
            break;
        }
        line++;
    }

    fclose(A);
    fclose(B);
    return (erro) ? line : 0;
}

// ===================================================
// TESTAR UMA LINHA
// ===================================================
void testar_linha(MANAGER_C manager, int cmd_n, int qID, char **args, int format, const char *expected_dir) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    void *r = NULL;

    // Encaminhamento para a Query correta
    if (qID == 1)      { total_Q1++; r = query1(manager, args, format); }
    else if (qID == 2) { total_Q2++; r = query2(manager, args, format); }
    else if (qID == 3) { total_Q3++; r = query3(manager, args, format); }
    else if (qID == 5) { total_Q5++; r = query5(manager, args, format); }
    else if (qID == 6) { total_Q6++; r = query6(manager, args, format); } 

    // Escrita do Output
    char out_path[256];
    snprintf(out_path, sizeof(out_path), "resultados/command%d_output.txt", cmd_n);

    FILE *f = fopen(out_path, "w");
    if (f) {
        output_query(f, r, qID); 
        fclose(f);
    }

    // Comparação com o diretório de resultados esperados
    char exp_path[256];
    snprintf(exp_path, sizeof(exp_path), "%s/command%d_output.txt", expected_dir, cmd_n);

    int diff_line = compare_files_and_find_line(out_path, exp_path, cmd_n);

    if (diff_line == 0) {
        if (qID == 1) ok_Q1++;
        else if (qID == 2) ok_Q2++;
        else if (qID == 3) ok_Q3++;
        else if (qID == 5) ok_Q5++;
        else if (qID == 6) ok_Q6++;
    } else {
        diffs[diff_count].query_n = cmd_n;
        diffs[diff_count].line_n = diff_line;
        diff_count++;
    }

    free_query(r, qID);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double ms = time_diff_ms(start, end);
    time_total += ms/1000.0;

    if (qID == 1) time_Q1 += ms;
    else if (qID == 2) time_Q2 += ms;
    else if (qID == 3) time_Q3 += ms;
    else if (qID == 5) time_Q5 += ms;
    else if (qID == 6) time_Q6 += ms;
}

// ===================================================
// FUNÇÃO PRINCIPAL DE TESTES
// ===================================================
void programa_testes(char *dataset, char *input_file, char *expected_dir) {
    printf(ANSI_COLOR_YELLOW "A carregar Dataset: %s ...\n" ANSI_COLOR_RESET, dataset);
    
    // Inicialização do Manager
    PASSENGERS_C pc = create_passenger_catalog();
    FLIGHTS_C fc = create_flight_catalog();
    RESERVATIONS_C rc = create_reservation_catalog();
    AIRCRAFTS_C ac = create_aircraft_catalog();
    AIRPORTS_C apc = create_airport_catalog();

    MANAGER_C manager = create_manager_c(fc, pc, ac, rc, apc);

    if (!set_catalogs(manager, dataset)) {
        printf(ANSI_COLOR_RED "Erro no carregamento dos catálogos.\n" ANSI_COLOR_RESET);
        return;
    }

    FILE *fin = fopen(input_file, "r");
    if (!fin) {
        printf(ANSI_COLOR_RED "Erro ao abrir ficheiro de input: %s\n" ANSI_COLOR_RESET, input_file);
        free_manager_c(manager);
        return;
    }

    printf(ANSI_COLOR_GREEN "Testes iniciados...\n" ANSI_COLOR_RESET);

    char line[1024];
    int cmd_n = 1;

    while (fgets(line, sizeof(line), fin)) {
        line[strcspn(line, "\r\n")] = '\0';
        if (!line[0] || line[0] == '#') continue;

        char buffer[1024];
        strcpy(buffer, line);

        char *command = strtok(buffer, " ");
        if (!command) continue;

        int format = 0;
        int len = strlen(command);
        if (len > 0 && toupper(command[len - 1]) == 'S') {
            format = 1;              
            command[len - 1] = '\0'; 
        }

        int qID = atoi(command);

        // Parsing de argumentos específico por Query
        if (qID == 1) {
            char *arg = strtok(NULL, ""); 
            while(arg && *arg == ' ') arg++;
            char *args_array[2] = { arg, NULL };
            testar_linha(manager, cmd_n, 1, args_array, format, expected_dir);
        }
        else if (qID == 2) {
            char *a1 = strtok(NULL, " ");
            char *a2 = strtok(NULL, ""); 
            char *args_array[3] = { a1, a2, NULL };
            testar_linha(manager, cmd_n, 2, args_array, format, expected_dir);
        }
        else if (qID == 3) {
            char *d1 = strtok(NULL, " ");
            char *d2 = strtok(NULL, " ");
            char *args_array[3] = { d1, d2, NULL };
            testar_linha(manager, cmd_n, 3, args_array, format, expected_dir);
        }
        else if (qID == 5) {
            char *n_val = strtok(NULL, " ");
            char *args_array[2] = { n_val, NULL };
            testar_linha(manager, cmd_n, 5, args_array, format, expected_dir);
        }
        else if (qID == 6) {
            // A Q6 recebe a nacionalidade (pode conter espaços em alguns datasets)
            char *nat = strtok(NULL, ""); 
            if (nat) {
                while(*nat == ' ') nat++; // Trim inicial
            }
            char *args_array[2] = { nat, NULL };
            testar_linha(manager, cmd_n, 6, args_array, format, expected_dir);
        }

        cmd_n++;
    }

    fclose(fin);

    // Relatório Final formatado para o terminal
    printf("\n" ANSI_COLOR_YELLOW "=== RELATÓRIO DE DESEMPENHO E PRECISÃO ===" ANSI_COLOR_RESET "\n");
    printf("Q1: %d/%d OK (Tempo médio: %.4f ms)\n", ok_Q1, total_Q1, total_Q1 ? time_Q1/total_Q1 : 0);
    printf("Q2: %d/%d OK (Tempo médio: %.4f ms)\n", ok_Q2, total_Q2, total_Q2 ? time_Q2/total_Q2 : 0);
    printf("Q3: %d/%d OK (Tempo médio: %.4f ms)\n", ok_Q3, total_Q3, total_Q3 ? time_Q3/total_Q3 : 0);
    printf("Q5: %d/%d OK (Tempo médio: %.4f ms)\n", ok_Q5, total_Q5, total_Q5 ? time_Q5/total_Q5 : 0);
    printf("Q6: %d/%d OK (Tempo médio: %.4f ms)\n", ok_Q6, total_Q6, total_Q6 ? time_Q6/total_Q6 : 0);
    printf("-------------------------------------------\n");
    printf("Tempo total de execução de queries: %.4f segundos\n", time_total);

    if (diff_count == 0) {
        printf(ANSI_COLOR_GREEN "\nSUCESSO TOTAL: Todos os outputs coincidem com o esperado.\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "\nERROS DETETADOS: %d comandos falharam.\n" ANSI_COLOR_RESET, diff_count);
    }

    free_manager_c(manager); // Libertação total
}