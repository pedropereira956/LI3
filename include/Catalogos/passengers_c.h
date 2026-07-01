/**
 * @file passengers_c.h
 * @brief Módulo responsável pelo catálogo de passageiros.
 *
 * Este módulo gere a estrutura que armazena todos os passageiros presentes
 * no sistema. Suporta operações de inserção, consulta, remoção e listagem.
 * A implementação baseia-se numa hash table da GLib, garantindo acessos rápidos
 * através do número de documento do passageiro.
 */

#ifndef PASSENGERS_C_H
#define PASSENGERS_C_H

#include <glib.h>
#include "Entidades/passenger.h"

/**
 * @brief Estrutura opaca que representa o catálogo de passageiros.
 *
 * O catálogo armazena internamente:
 * - uma hash table que mapeia números de documento → PASSENGER
 */
typedef struct passengers_catalog *PASSENGERS_C;

/* ============================
 *        Criação e Memória
 * ============================ */

/**
 * @brief Cria um novo catálogo de passageiros vazio.
 *
 * A hash table criada utiliza o número de documento do passageiro como chave.
 *
 * @return Um objeto PASSENGERS_C recém-criado.
 */
PASSENGERS_C create_passenger_catalog(void);

/**
 * @brief Liberta toda a memória associada ao catálogo de passageiros.
 *
 * Remove todas as entradas, liberta cada estrutura PASSENGER
 * e destrói a hash table interna.
 *
 * @param catalog Catálogo a libertar.
 */
void free_passenger_catalog(PASSENGERS_C catalog);

/**
 * @brief Função redundante — libertação completa do catálogo.
 *
 * Esta função deve comportar-se de forma equivalente a free_passenger_catalog(),
 * dependendo da implementação no ficheiro .c.
 *
 * @param catalog Catálogo de passageiros.
 */
void free_passenger_c(PASSENGERS_C catalog);

/* ============================
 *          Operações
 * ============================ */

/**
 * @brief Insere um passageiro no catálogo.
 *
 * @param passenger Estrutura PASSENGER a inserir.
 * @param catalog Catálogo onde será guardado.
 */
void insert_passenger_c(PASSENGER passenger, PASSENGERS_C catalog);

/**
 * @brief Procura um passageiro através do seu número de documento.
 *
 * @param catalog Catálogo de passageiros.
 * @param doc_number Número de documento (string).
 * @return Estrutura PASSENGER correspondente ou NULL se não existir.
 */
PASSENGER get_passenger_by_id(PASSENGERS_C catalog, const gchar *doc_number);

/**
 * @brief Remove um passageiro do catálogo através do número de documento.
 *
 * @param catalog Catálogo de passageiros.
 * @param doc_number Número de documento do passageiro a remover.
 */
void remove_passenger_from_catalog(PASSENGERS_C catalog, const gchar *doc_number);

/**
 * @brief Obtém a hash table interna do catálogo de passageiros.
 *
 * Deve ser usada com cuidado para manter o encapsulamento.
 *
 * @param catalog Catálogo de passageiros.
 * @return GHashTable* que armazena os passageiros.
 */
GHashTable* get_hash_table_passenger(PASSENGERS_C catalog);

/* ============================
 *           Impressão
 * ============================ */

/**
 * @brief Imprime todos os passageiros presentes no catálogo.
 *
 * Função útil para debugging e inspeção manual.
 *
 * @param catalog Catálogo de passageiros.
 */
void print_all_passengers(PASSENGERS_C catalog);

#endif
