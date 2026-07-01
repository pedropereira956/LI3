#include "IO/interpreter.h" // Ou "Executavel/interpreter.h" conforme a tua estrutura
#include "menuNdata/queries.h" // Para ter acesso às funções query1, query2, etc.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <glib.h>

// Definição do tipo de função para o array (deve corresponder ao header das queries)
typedef void* (*queries_func)(MANAGER_C, char**, int);

/**
 * @brief Analisa uma linha de texto e executa a query correspondente.
 *
 * Esta função:
 * 1. Divide a linha em tokens.
 * 2. Verifica se o primeiro token tem 'S' (ex: "1S") -> define format=1.
 * 3. Chama a função da query passando o catálogo, os argumentos e o formato.
 *
 * @param manager_catalog Catálogo principal.
 * @param line Linha de texto da query.
 * @return Resultado devolvido pela função da query.
 */
void* parser_query(MANAGER_C manager_catalog, char* line) {
    if (!line || !manager_catalog) return NULL;

    // Divide a linha por espaços
    gchar **tokens = g_strsplit_set(line, " \r\n", -1);
    if (!tokens || !tokens[0]) {
        g_strfreev(tokens);
        return NULL;
    }

    char *command = tokens[0];
    int format = 0; // 0 = default (;), 1 = custom (=)

    // 1. Deteção do 'S' (Formato Especial)
    int len = strlen(command);
    if (len > 0 && toupper(command[len - 1]) == 'S') {
        format = 1;
        command[len - 1] = '\0'; // Remove o 'S' para ficar apenas o número (ex: "1S" -> "1")
    }

    // 2. Converter para ID numérico
    int query_id = atoi(command);
    if (query_id < 1 || query_id > 10) { 
        g_strfreev(tokens);
        return NULL;
    }

    // 3. Tabela de funções (Atualizada para aceitar o 3º argumento 'format')
    // Nota: Certifica-te que queries.h tem as assinaturas certas
    static queries_func queries[] = {
        query1, query2, query3, 
        NULL, query5, query6, NULL, NULL, NULL, NULL // Placeholders para q4-q10
    };

    void *result = NULL;

    // 4. Executar Query
    if (queries[query_id - 1]) {
        // Passamos tokens + 1 (ignorando o comando "1" ou "1S") e o format
        result = queries[query_id - 1](manager_catalog, tokens + 1, format);
    }

    g_strfreev(tokens);
    return result;
}


/**
 * @brief Lê todas as queries de um ficheiro, executa-as e grava o resultado.
 *
 * @param manager_catalog Catálogo principal do sistema.
 * @param path2 Caminho do ficheiro de queries.
 * @param flag Se 1, mede o tempo de execução de cada query.
 * @return 0 se sucesso, -1 se erro.
 */
int execute_queries(MANAGER_C manager_catalog, char* path2, int flag) {
    if (!manager_catalog || !path2) return -1;

    FILE *queries_file = fopen(path2, "r");
    if (!queries_file) {
        perror("Erro a abrir ficheiro de queries");
        return -1;
    }

    FILE *analysis_file = NULL;
    if (flag == 1) {
        analysis_file = fopen("resultados/analysis.txt", "w"); // Garante que a pasta resultados existe
        if (!analysis_file) {
            perror("Erro a criar analysis.txt");
            // Não faz return -1 aqui para não parar a execução principal, apenas avisa
        }
    }

    char *line = NULL;
    size_t len = 0;
    int cmd_n = 1; // Contador para o nome dos ficheiros (command1_output.txt, etc.)
    struct timespec start, end;
    double elapsed;

    while (getline(&line, &len, queries_file) != -1) {
        // Remove newline no fim da linha
        line[strcspn(line, "\r\n")] = '\0'; 
        
        if (strlen(line) == 0 || line[0] == '#') continue; // Ignora vazias ou comentários

        // Medição de tempo (Início)
        if (flag == 1) clock_gettime(CLOCK_REALTIME, &start);

        // >>> Execução da Query <<<
        void *result = parser_query(manager_catalog, line);

        // Medição de tempo (Fim)
        if (flag == 1 && analysis_file) {
            clock_gettime(CLOCK_REALTIME, &end);
            elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
            fprintf(analysis_file, "Query %d: %.6f segundos\n", cmd_n, elapsed);
        }

        // Criar ficheiro de output (commandX_output.txt)
        FILE *output_file = create_output_file(cmd_n);
        if (output_file) {
            // Nota: atoi("1S") devolve 1, por isso funciona para saber qual o ID base
            int query_id = atoi(line); 
            
            if (result != NULL) {
                // Assume-se que output_query sabe imprimir o void* retornado
                output_query(output_file, result, query_id);
                free_query(result, query_id);
            }
            fclose(output_file);
        } else {
            fprintf(stderr, "Erro ao criar output para comando %d\n", cmd_n);
        }

        cmd_n++;
    }

    free(line);
    fclose(queries_file);
    if (analysis_file) fclose(analysis_file);

    return 1;
}