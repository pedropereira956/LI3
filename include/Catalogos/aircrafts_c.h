/**
 * @file aircrafts_c.h
 * @brief Módulo responsável pelo catálogo de aeronaves e pela contagem de voos associados.
 *
 * Este módulo gere o catálogo de aeronaves (aircrafts), permitindo inserir,
 * consultar e remover aeronaves, bem como acompanhar o número de voos associados
 * a cada uma. A estrutura também suporta a criação de rankings de utilização
 * com base no número de voos.
 */

#ifndef AIRCRAFTS_C_H
#define AIRCRAFTS_C_H

#include <glib.h>
#include "Entidades/aircraft.h"

/**
 * @brief Estrutura opaca que representa o catálogo de aeronaves.
 */
typedef struct aircrafts_catalog *AIRCRAFTS_C;

/**
 * @brief Estrutura que associa uma aeronave ao número de voos em que participou.
 */
typedef struct aircraft_count AircraftCount;

/* ============================
 *      Funções Base
 * ============================ */

/**
 * @brief Cria um catálogo de aeronaves vazio.
 *
 * O catálogo contém:
 * - uma hash table que mapeia IDs de aeronaves para estruturas AIRCRAFT
 * - uma segunda hash table que armazena a contagem de voos por aeronave
 *
 * @return Novo objeto AIRCRAFTS_C.
 */
AIRCRAFTS_C create_aircraft_catalog(void);

/**
 * @brief Obtém a hash table que armazena as aeronaves.
 *
 * @param catalog Catálogo de aeronaves.
 * @return Ponteiro para uma GHashTable com mapeamento ID → AIRCRAFT.
 */
GHashTable* get_hash_table_aircraft(AIRCRAFTS_C catalog);

/**
 * @brief Obtém a hash table que armazena a contagem de voos por aeronave.
 *
 * @param catalog Catálogo de aeronaves.
 * @return Ponteiro para uma GHashTable com mapeamento ID → AircraftCount.
 */
GHashTable* get_hash_table_flight_counts(AIRCRAFTS_C catalog);

/**
 * @brief Obtém uma lista ordenada com o ranking das aeronaves por número de voos.
 *
 * A lista é ordenada usando compare_aircraft_count().
 *
 * @param catalog Catálogo de aeronaves.
 * @return Lista GList* ordenada contendo elementos AircraftCount.
 */
GList *get_sorted_flight_ranking(AIRCRAFTS_C catalog);

/**
 * @brief Insere uma nova aeronave no catálogo.
 *
 * @param aircraft Estrutura AIRCRAFT a inserir.
 * @param catalog Catálogo onde será inserida.
 */
void insert_aircraft_c(AIRCRAFT aircraft, AIRCRAFTS_C catalog);

/**
 * @brief Procura uma aeronave pelo seu ID.
 *
 * @param catalog Catálogo de aeronaves.
 * @param id Identificador da aeronave.
 * @return Ponteiro AIRCRAFT ou NULL se não existir.
 */
AIRCRAFT get_aircraft_by_id(AIRCRAFTS_C catalog, gchar *id);

/**
 * @brief Remove uma aeronave do catálogo utilizando o seu ID.
 *
 * @param catalog Catálogo de aeronaves.
 * @param aircraft_id Identificador da aeronave a remover.
 */
void remove_aircraft_from_hash_table(AIRCRAFTS_C catalog, gchar *aircraft_id);

/* ============================
 *     Funções AircraftCount
 * ============================ */

/**
 * @brief Obtém o ID associado a uma estrutura AircraftCount.
 *
 * @param ac Estrutura AircraftCount.
 * @return ID da aeronave.
 */
const gchar *get_aircraft_count_id(const AircraftCount *ac);

/**
 * @brief Obtém o número de voos registados para a aeronave.
 *
 * @param ac Estrutura AircraftCount.
 * @return Número de voos.
 */
guint get_aircraft_count_value(const AircraftCount *ac);

/* ============================
 *      Gestão de Memória
 * ============================ */

/**
 * @brief Liberta toda a memória associada ao catálogo.
 *
 * Remove e destrói:
 * - todas as aeronaves
 * - todas as estruturas AircraftCount
 * - todas as hash tables internas
 *
 * @param catalog Catálogo a libertar.
 */
void free_aircraft_c(AIRCRAFTS_C catalog);

/**
 * @brief Liberta uma estrutura AircraftCount.
 *
 * @param ac Estrutura AircraftCount a libertar.
 */
void free_aircraft_count(AircraftCount *ac);

/* ============================
 *          Impressão
 * ============================ */

/**
 * @brief Imprime todas as aeronaves presentes no catálogo.
 *
 * Função útil para debugging.
 *
 * @param catalog Catálogo de aeronaves.
 */
void print_all_aircrafts(AIRCRAFTS_C catalog);

/**
 * @brief Função de comparação usada no ordenamento de rankings.
 *
 * @param a Primeiro elemento (AircraftCount).
 * @param b Segundo elemento (AircraftCount).
 * @return <0 se a < b, >0 se a > b, 0 se iguais.
 */
gint compare_aircraft_count(gconstpointer a, gconstpointer b);

/* ============================
 *      Contagem de Voos
 * ============================ */

/**
 * @brief Atualiza a contagem de voos para um dado ID de aeronave.
 *
 * Apenas voos com estado válido devem contar para a estatística.
 *
 * @param catalog Catálogo de aeronaves.
 * @param aircraft_id ID da aeronave.
 * @param status Estado do voo ("scheduled", "cancelled", etc.).
 */
void update_flight_count(AIRCRAFTS_C catalog, const gchar *aircraft_id, const gchar *status);

/**
 * @brief Constrói (ou reconstrói) o ranking das aeronaves com base no número de voos.
 *
 * @param catalog Catálogo de aeronaves.
 */
void build_aircraft_ranking(AIRCRAFTS_C catalog);

/**
 * @brief Imprime a tabela de contagem de voos (hash table).
 *
 * @param flight_counts Hash table com AircraftCount.
 */
void print_flight_counts_table(GHashTable *flight_counts);

/**
 * @brief Imprime o ranking das aeronaves.
 *
 * @param catalog Catálogo de aeronaves.
 */
void print_aircraft_ranking(AIRCRAFTS_C catalog);

#endif
