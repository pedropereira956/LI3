#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Catalogos/manager_c.h"
#include "menuNdata/queries.h"   // Módulo onde se encontram implementadas as queries
#include "IO/output.h"
#include <glib.h>

#define MAX_ARGS 7  /**< Número máximo de argumentos permitidos numa query */

/**
 * @file interpreter.h
 * @brief Módulo responsável por interpretar e executar queries.
 *
 * Este módulo faz a ponte entre o ficheiro de input e o sistema interno.
 * Lê cada linha do ficheiro de queries, interpreta o comando,
 * identifica a função de query correspondente e executa-a.
 *
 * Atua como o "motor" do sistema: recebe texto, chama a lógica de negócio
 * adequada (queries) e produz o respetivo output.
 */


/**
 * @typedef queries_func
 * @brief Tipo de função apontadora para funções de query.
 *
 * Cada função de query segue esta assinatura:
 * @code
 * void* queryN(MANAGER_C catalog, char** query_args, int format);
 * @endcode
 *
 * O retorno é genérico (void*), permitindo diferentes formatos de resultados
 * consoante a query executada.
 *
 * @param format Inteiro que define o formato de output (0 para ';', 1 para '=').
 */
typedef void* (*queries_func)(MANAGER_C catalog, char** query_args, int format);


/**
 * @brief Interpreta e executa uma única query.
 *
 * Esta função:
 * - faz o parsing da linha recebida (divide em tokens);
 * - identifica o número da query (query1, query2, ...);
 * - valida e extrai os argumentos;
 * - chama a função correspondente;
 * - devolve o resultado produzido pela query.
 *
 * @param catalog Estrutura principal que agrega todos os catálogos.
 * @param line Linha contendo a query (ex: "3 LIS 2023-10-10").
 * @return Ponteiro para o resultado da query (a ser libertado posteriormente).
 */
void* parser_query(MANAGER_C catalog, char* line);


/**
 * @brief Executa todas as queries presentes num ficheiro de input.
 *
 * Esta função controla o ciclo completo:
 * - abre o ficheiro indicado por `path2`;
 * - lê linha a linha;
 * - executa cada query através de parser_query();
 * - chama o módulo de output para escrever commandN_output.txt;
 * - opcionalmente mede tempos de execução (flag == 1);
 * - liberta a memória associada após cada query.
 *
 * @param manager_catalog Catálogo principal contendo todas as estruturas de dados.
 * @param path2 Caminho para o ficheiro de input com as queries.
 * @param flag Ativa a medição de tempos (1 = medir tempo).
 * @return 0 em caso de sucesso, -1 caso ocorra algum erro.
 */
int execute_queries(MANAGER_C manager_catalog, char* path2, int flag);

#endif