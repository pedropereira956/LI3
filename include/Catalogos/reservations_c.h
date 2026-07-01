/**
 * @file reservations_c.h
 * @brief Módulo responsável pelo catálogo de reservas.
 *
 * Este módulo gere a estrutura que armazena todas as reservas do sistema.
 * As reservas são organizadas numa hash table da GLib, permitindo inserções
 * e pesquisas eficientes através do respetivo identificador.
 */

#ifndef RESERVATIONS_C_H
#define RESERVATIONS_C_H

#include <glib.h>
#include "Entidades/reservation.h"

/**
 * @brief Estrutura opaca que representa o catálogo de reservas.
 *
 * Internamente, o catálogo contém:
 * - uma hash table que mapeia IDs de reserva → RESERVATION
 */
typedef struct reservations_catalog *RESERVATIONS_C;

/* ============================
 *      Criação e Acesso
 * ============================ */

/**
 * @brief Cria um novo catálogo de reservas vazio.
 *
 * @return Um objeto RESERVATIONS_C inicializado.
 */
RESERVATIONS_C create_reservation_catalog(void);

/**
 * @brief Obtém a hash table interna que armazena as reservas.
 *
 * Deve ser usada com cuidado para preservar o encapsulamento.
 *
 * @param catalog Catálogo de reservas.
 * @return GHashTable* que contém todos os objetos RESERVATION.
 */
GHashTable* get_hash_table_reservation(RESERVATIONS_C catalog);

/**
 * @brief Liberta toda a memória associada ao catálogo de reservas.
 *
 * Esta função:
 *  - remove todas as entradas da hash table,
 *  - liberta cada estrutura RESERVATION,
 *  - destrói o catálogo.
 *
 * @param catalog Catálogo a libertar.
 */
void free_reservation_c(RESERVATIONS_C catalog);

/* ============================
 *          Operações
 * ============================ */

/**
 * @brief Insere uma reserva no catálogo.
 *
 * @param reservation Estrutura RESERVATION a inserir.
 * @param catalog Catálogo onde será armazenada.
 */
void insert_reservation_c(RESERVATION reservation, RESERVATIONS_C catalog);

/**
 * @brief Procura uma reserva pelo seu identificador.
 *
 * @param catalog Catálogo de reservas.
 * @param id ID da reserva a procurar.
 * @return A reserva correspondente ou NULL caso não exista.
 */
RESERVATION get_reservation_by_id(RESERVATIONS_C catalog, const gchar *id);

/**
 * @brief Remove uma reserva do catálogo através do seu ID.
 *
 * @param catalog Catálogo de reservas.
 * @param id Identificador da reserva a remover.
 */
void remove_reservation_from_hash_table(RESERVATIONS_C catalog, const gchar *id);

/* ============================
 *            Util
 * ============================ */

/**
 * @brief Imprime todas as reservas presentes no catálogo.
 *
 * Função útil para debugging e inspeção manual.
 *
 * @param catalog Catálogo de reservas.
 */
void print_all_reservations(RESERVATIONS_C catalog);

#endif
