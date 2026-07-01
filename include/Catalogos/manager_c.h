/**
 * @file manager_c.h
 * @brief Módulo agregador responsável por gerir todos os catálogos do sistema.
 *
 * Este módulo representa o "gestor de catálogos", uma estrutura central que
 * contém e disponibiliza acesso aos diferentes catálogos do sistema:
 * - Voos
 * - Passageiros
 * - Aeronaves
 * - Reservas
 * - Aeroportos
 *
 * A sua existência permite que outros módulos (como as queries) acedam de forma
 * unificada a todos os catálogos, promovendo organização, encapsulamento e
 * simplificação do código.
 */

#ifndef MANAGER_C_H
#define MANAGER_C_H

#include <glib.h>
#include "Catalogos/flights_c.h"
#include "Catalogos/passengers_c.h"
#include "Catalogos/aircrafts_c.h"
#include "Catalogos/reservations_c.h"
#include "Catalogos/airports_c.h"

/**
 * @brief Estrutura que armazena estatísticas acumuladas por companhia aérea.
 * * Mover esta definição para aqui resolve o erro de "unknown type name" no queries.c.
 * É usada para calcular o atraso médio na Query 5.
 */
typedef struct airline_stats {
    char *airline;      /**< Nome da companhia aérea */
    double total_delay; /**< Soma acumulada de atrasos em minutos */
    int count;          /**< Total de voos com estado 'Delayed' */
} *AIRLINE_STATS;

/**
 * @brief Estrutura opaca que representa o gestor de catálogos.
 *
 * Esta estrutura contém internamente todos os catálogos principais,
 * bem como tabelas auxiliares utilizadas em queries específicas.
 */
typedef struct manager_catalog *MANAGER_C;

/* ============================
 * Criação
 * ============================ */

/**
 * @brief Cria o gestor de catálogos, armazenando todos os catálogos recebidos.
 *
 * @param flights Catálogo de voos.
 * @param passengers Catálogo de passageiros.
 * @param aircrafts Catálogo de aeronaves.
 * @param reservations Catálogo de reservas.
 * @param airports Catálogo de aeroportos.
 * @return MANAGER_C recém-criado.
 */
MANAGER_C create_manager_c(FLIGHTS_C flights,
                           PASSENGERS_C passengers,
                           AIRCRAFTS_C aircrafts,
                           RESERVATIONS_C reservations,
                           AIRPORTS_C airports);
                           

/* ============================
 * Acesso aos Catálogos
 * ============================ */

/**
 * @brief Obtém o catálogo de voos.
 * @param manager Gestor de catálogos.
 * @return Catálogo de voos.
 */
FLIGHTS_C get_flights_c(MANAGER_C manager);

/**
 * @brief Obtém o catálogo de passageiros.
 * @param manager Gestor de catálogos.
 * @return Catálogo de passageiros.
 */
PASSENGERS_C get_passengers_c(MANAGER_C manager);

/**
 * @brief Obtém o catálogo de aeronaves.
 * @param manager Gestor de catálogos.
 * @return Catálogo de aeronaves.
 */
AIRCRAFTS_C get_aircrafts_c(MANAGER_C manager);

/**
 * @brief Obtém o catálogo de reservas.
 * @param manager Gestor de catálogos.
 * @return Catálogo de reservas.
 */
RESERVATIONS_C get_reservations_c(MANAGER_C manager);

/**
 * @brief Obtém o catálogo de aeroportos.
 * @param manager Gestor de catálogos.
 * @return Catálogo de aeroportos.
 */
AIRPORTS_C get_airports_c(MANAGER_C manager);

/* ============================
 * Tabelas Auxiliares
 * ============================ */

/**
 * @brief Define a hash table que mapeia datas → listas de partidas.
 * @param manager Gestor de catálogos.
 * @param table Hash table a armazenar.
 */
void set_departures_by_date(MANAGER_C manager, GHashTable *table);

/**
 * @brief Obtém a tabela que mapeia datas para voos que partem nesse dia.
 * @param manager Gestor de catálogos.
 * @return Hash table previamente definida, ou NULL se não existir.
 */
GHashTable* get_departures_by_date(MANAGER_C manager);

/**
 * @brief Atualiza ou insere as estatísticas de atraso de uma companhia aérea.
 * * Invocada durante o parsing de voos para acumular dados de voos 'Delayed'.
 *
 * @param manager Gestor de catálogos.
 * @param airline Nome da companhia aérea.
 * @param delay Tempo de atraso em minutos.
 */
void update_airline_stats(MANAGER_C manager, const char *airline, int delay);

/**
 * @brief Obtém a tabela hash contendo as estatísticas acumuladas das companhias.
 * @param manager Gestor de catálogos.
 * @return GHashTable mapeando o nome da companhia para a estrutura AIRLINE_STATS.
 */
GHashTable* get_airline_stats_table(MANAGER_C manager);

/**
 * @brief Atualiza as estatísticas de nacionalidade por aeroporto (Query 6).
 * @param manager Gestor de catálogos.
 * @param nationality Nacionalidade do passageiro.
 * @param airport_iata Código IATA do aeroporto de destino.
 */
void update_nationality_stats(MANAGER_C manager, const char *nationality, const char *airport_iata);

/**
 * @brief Obtém a tabela de estatísticas de nacionalidades.
 * @return GHashTable (Nacionalidade -> GHashTable(Aeroporto -> Contador)).
 */
GHashTable* get_nationality_stats_table(MANAGER_C manager);



/* ============================
 * Gestão de Memória
 * ============================ */

 /**
 * @brief Liberta a memória de uma estrutura de estatísticas de companhia aérea.
 *
 * Esta função é utilizada como destruidor (callback) na tabela hash para limpar
 * tanto a estrutura interna como a string duplicada do nome da companhia.
 * * @param data Apontador para a estrutura AIRLINE_STATS (gpointer).
 */
void free_airline_stats_value(gpointer data);

/**
 * @brief Liberta toda a memória associada ao gestor de catálogos.
 * @param manager Gestor a libertar.
 */
void free_manager_c(MANAGER_C manager);

#endif