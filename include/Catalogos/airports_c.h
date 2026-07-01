/**
 * @file airports_c.h
 * @brief Módulo responsável pelo catálogo de aeroportos.
 *
 * Este módulo gere a estrutura que armazena todos os aeroportos presentes no
 * sistema, permitindo a sua inserção, consulta, remoção e listagem. A estrutura
 * baseia-se numa hash table da GLib para garantir eficiência e acessos rápidos
 * utilizando códigos IATA.
 */

#ifndef AIRPORTS_C_H
#define AIRPORTS_C_H

#include <glib.h>
#include "Entidades/airport.h"

/**
 * @brief Estrutura opaca que representa o catálogo de aeroportos.
 *
 * O catálogo armazena:
 * - uma hash table que mapeia códigos IATA → AIRPORT
 */
typedef struct airports_catalog *AIRPORTS_C;

/* ============================
 *      Funções Base
 * ============================ */

/**
 * @brief Cria um novo catálogo de aeroportos vazio.
 *
 * Inicializa internamente uma hash table da GLib que associa
 * códigos IATA (string) a estruturas AIRPORT.
 *
 * @return Catálogo AIRPORTS_C recém-criado.
 */
AIRPORTS_C create_airport_catalog(void);

/**
 * @brief Liberta toda a memória associada ao catálogo de aeroportos.
 *
 * Remove todas as entradas da hash table, liberta cada estrutura AIRPORT
 * e destrói o catálogo.
 *
 * @param catalog Catálogo a libertar.
 */
void free_airport_c(AIRPORTS_C catalog);

/**
 * @brief Obtém a hash table interna do catálogo.
 *
 * Função útil para módulos que necessitem de interação direta com os dados.
 * O conteúdo deve ser manipulado com cuidado para preservar o encapsulamento.
 *
 * @param catalog Catálogo de aeroportos.
 * @return Hash table que armazena os aeroportos (ID → AIRPORT).
 */
GHashTable* get_hash_table_airport(AIRPORTS_C catalog);

/**
 * @brief Insere um aeroporto no catálogo.
 *
 * @param airport Estrutura AIRPORT a inserir.
 * @param catalog Catálogo onde será guardado.
 */
void insert_airport_c(AIRPORT airport, AIRPORTS_C catalog);

/**
 * @brief Procura um aeroporto pelo seu código IATA.
 *
 * @param catalog Catálogo de aeroportos.
 * @param code Código IATA a procurar (string de 3 letras).
 * @return Ponteiro AIRPORT correspondente ou NULL se não existir.
 */
AIRPORT get_airport_by_code(AIRPORTS_C catalog, const gchar *code);

/**
 * @brief Remove um aeroporto do catálogo utilizando o seu código IATA.
 *
 * @param catalog Catálogo de aeroportos.
 * @param code Código IATA do aeroporto a remover.
 */
void remove_airport_from_catalog(AIRPORTS_C catalog, const gchar *code);

/**
 * @brief Imprime todos os aeroportos contidos no catálogo.
 *
 * Função útil para debugging ou verificação manual dos dados carregados.
 *
 * @param catalog Catálogo de aeroportos.
 */
void print_all_airports(AIRPORTS_C catalog);

#endif
