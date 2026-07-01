/**
 * @file flights_c.h
 * @brief Módulo responsável pelo catálogo de voos.
 *
 * Este módulo gere o catálogo de voos presentes no sistema,
 * permitindo a sua inserção, consulta, remoção e listagem.
 * O catálogo baseia-se numa hash table da GLib, garantindo
 * acesso eficiente aos voos através do respetivo ID.
 */

#ifndef FLIGHTS_C_H
#define FLIGHTS_C_H

#include <glib.h>
#include "Entidades/flight.h"

/**
 * @brief Estrutura opaca que representa o catálogo de voos.
 *
 * O catálogo contém:
 * - uma hash table que mapeia IDs de voo (string) → FLIGHT
 */
typedef struct flights_catalog *FLIGHTS_C;

/* ============================
 *      Funções Base
 * ============================ */

/**
 * @brief Cria um novo catálogo de voos vazio.
 *
 * Inicializa internamente uma hash table da GLib onde os voos
 * serão armazenados de forma indexada pelo seu ID.
 *
 * @return Novo objeto FLIGHTS_C.
 */
FLIGHTS_C create_flight_catalog(void);

/**
 * @brief Obtém a hash table interna do catálogo de voos.
 *
 * @param catalog Catálogo de voos.
 * @return Ponteiro para a hash table que contém os voos.
 */
GHashTable* get_hash_table_flight(FLIGHTS_C catalog);

/**
 * @brief Insere um voo no catálogo.
 *
 * @param flight Estrutura FLIGHT a inserir.
 * @param catalog Catálogo onde será guardado.
 */
void insert_flight_c(FLIGHT flight, FLIGHTS_C catalog);

/**
 * @brief Procura um voo através do seu ID.
 *
 * @param catalog Catálogo de voos.
 * @param id ID do voo a procurar.
 * @return FLIGHT correspondente ou NULL caso não exista.
 */
FLIGHT get_flight_by_id(FLIGHTS_C catalog, gchar* id);

/**
 * @brief Remove um voo do catálogo utilizando o seu ID.
 *
 * @param flights Catálogo de voos.
 * @param flight_id ID do voo a remover.
 */
void remove_flight_from_hash_table(FLIGHTS_C flights, gchar* flight_id);

/* ============================
 *         Impressão
 * ============================ */

/**
 * @brief Imprime todos os voos presentes no catálogo.
 *
 * Função útil para debugging e verificação manual.
 *
 * @param catalog Catálogo de voos.
 */
void print_all_flights(FLIGHTS_C catalog);

/* ============================
 *      Gestão de Memória
 * ============================ */

/**
 * @brief Liberta toda a memória associada ao catálogo de voos.
 *
 * Remove todas as entradas, liberta cada estrutura FLIGHT
 * e destrói a hash table interna.
 *
 * @param catalog Catálogo a libertar.
 */
void free_flight_c(FLIGHTS_C catalog);

#endif
