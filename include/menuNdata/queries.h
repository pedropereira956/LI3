#ifndef QUERIES_H
#define QUERIES_H

#include "Catalogos/manager_c.h"

/**
 * @file queries.h
 * @brief Declaração das funções responsáveis pela execução das queries.
 *
 * Cada função de query recebe o catálogo principal e um array de argumentos já
 * processados pelo módulo `interpreter`. O retorno é genérico (void*), permitindo
 * que cada query devolva estruturas distintas (strings, listas, structs, etc.),
 * posteriormente tratadas pelo módulo `output`.
 */


/**
 * @brief Executa a Query 1.
 *
 * Esta query processa os argumentos fornecidos e acede aos catálogos necessários
 * através do `MANAGER_C`. O resultado é devolvido num formato genérico
 * (void*), sendo o módulo `output` responsável pela formatação final.
 *
 * @param catalog Catálogo principal que contém todos os dados carregados.
 * @param args Array de argumentos já tokenizados pelo interpreter.
 * @param format Inteiro que define o formato de output (0 para separador ';', 1 para separador '=').
 * @return Ponteiro genérico contendo o resultado da query.
 */
void* query1(MANAGER_C catalog, char** args, int format);


/**
 * @brief Executa a Query 2.
 *
 * Tal como as restantes, esta função valida os argumentos fornecidos, acede
 * às estruturas necessárias e produz um resultado formatado internamente,
 * mas ainda genérico para ser interpretado pelo sistema de output.
 *
 * @param catalog Catálogo principal.
 * @param args Argumentos recebidos a partir do ficheiro de input.
 * @param format Inteiro que define o formato de output (0 para separador ';', 1 para separador '=').
 * @return Ponteiro com o resultado da query.
 */
void* query2(MANAGER_C catalog, char** args, int format);


/**
 * @brief Executa a Query 3.
 *
 * Esta query pode envolver filtragens, ordenações ou cálculos adicionais a
 * partir dos catálogos presentes no `manager`. O retorno segue o mesmo padrão
 * das outras queries.
 *
 * @param catalog Catálogo principal.
 * @param args Argumentos da query.
 * @param format Inteiro que define o formato de output (0 para separador ';', 1 para separador '=').
 * @return Resultado genérico (void*), dependente do tipo de output esperado.
 */
void* query3(MANAGER_C catalog, char** args, int format);

/**
 * @brief Executa a Query 5 (Top N companhias aéreas com maior atraso médio).
 * * @param catalog Catálogo principal (MANAGER_C).
 * @param args Argumentos da query (N).
 * @param format Formato de output (0 para ;, 1 para =).
 * @return String com o resultado formatado ou NULL.
 */
void* query5(MANAGER_C catalog, char** args, int format);


/**
 * @brief Executa a Query 6 (Aeroporto mais comum para uma nacionalidade).
 * @param catalog Catálogo principal.
 * @param args Argumentos da query (Nacionalidade).
 * @param format Formato de output (0 para ;, 1 para =).
 * @return String com o resultado (code;arrival_count) ou linha vazia.
 */
void* query6(MANAGER_C catalog, char** args, int format);



#endif