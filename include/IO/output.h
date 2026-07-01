#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>

/**
 * @file output.h
 * @brief Módulo responsável pela criação e escrita dos ficheiros de output.
 *
 * Cada query executada gera um ficheiro do tipo:
 *    Resultados/commandN_output.txt
 * onde N corresponde ao número da linha da query no ficheiro de input.
 *
 * Este módulo centraliza a criação desses ficheiros, a escrita dos resultados
 * e a libertação da memória associada aos mesmos.
 */


/**
 * @brief Cria o ficheiro correspondente à query atual.
 *
 * O ficheiro é criado dentro da pasta "Resultados/" com o formato:
 *      command<N>_output.txt
 *
 * @param cmd_n Número da query (ex.: 1, 2, 3...).
 * @return Ponteiro para o ficheiro criado, ou NULL em caso de falha.
 */
FILE* create_output_file(int cmd_n);


/**
 * @brief Escreve o resultado de uma query no ficheiro fornecido.
 *
 * A função recebe o resultado devolvido pela query (void*) e trata da
 * escrita deste no formato adequado. O formato poderá variar entre
 * queries — esta função serve como ponto central de adaptação.
 *
 * @param f Ficheiro onde o resultado será escrito.
 * @param result Ponteiro genérico para o resultado da query.
 * @param query_id Identificador da query executada.
 */
void output_query(FILE* f, void* result, int query_id);


/**
 * @brief Liberta a memória associada ao resultado de uma query.
 *
 * Cada query pode devolver tipos de dados distintos (strings,
 * estruturas complexas, listas, etc.). Esta função centraliza o processo
 * de libertação, garantindo que não existem memory leaks.
 *
 * @param result Ponteiro para o resultado devolvido pela query.
 * @param query_id Identificador da query executada.
 */
void free_query(void* result, int query_id);

#endif
