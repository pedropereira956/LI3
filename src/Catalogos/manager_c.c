#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "Catalogos/manager_c.h"
#include "Catalogos/flights_c.h"
#include "Catalogos/passengers_c.h"
#include "Catalogos/aircrafts_c.h"
#include "Catalogos/reservations_c.h"
#include "Catalogos/airports_c.h"

/**
 * Estrutura que guarda todos os catálogos principais e estruturas auxiliares.
 * A struct AIRLINE_STATS foi removida daqui pois agora é pública no manager_c.h.
 */
struct manager_catalog {
    FLIGHTS_C flights;
    PASSENGERS_C passengers;
    AIRCRAFTS_C aircrafts;
    RESERVATIONS_C reservations;
    AIRPORTS_C airports;

    GHashTable *departures_by_date;

    /** 🔹 Tabela Hash para a Query 5: Mapeia Nome (String) -> AIRLINE_STATS */
    GHashTable *airline_stats;

    GHashTable *nationality_stats;
};

void free_airline_stats_value(gpointer data) {
    AIRLINE_STATS stats = (AIRLINE_STATS)data;
    if (stats) {
        g_free(stats->airline); // Liberta o g_strdup feito no update_airline_stats
        free(stats);            // Liberta a memória alocada com malloc
    }
}

static void free_nested_hash_table(gpointer data) {
    if (data) {
        g_hash_table_destroy((GHashTable *)data);
    }
}

/**
 * Cria o gestor de catálogos com os apontadores fornecidos.
 */
MANAGER_C create_manager_c(FLIGHTS_C flights,
                           PASSENGERS_C passengers,
                           AIRCRAFTS_C aircrafts,
                           RESERVATIONS_C reservations,
                           AIRPORTS_C airports) {
    MANAGER_C new_manager = malloc(sizeof(struct manager_catalog));
    if (!new_manager) return NULL;

    new_manager->flights = flights;
    new_manager->passengers = passengers;
    new_manager->aircrafts = aircrafts;
    new_manager->reservations = reservations;
    new_manager->airports = airports;

    new_manager->departures_by_date = NULL; 

    /** 🔹 Inicialização da tabela de estatísticas com destruidores automáticos */
    new_manager->airline_stats = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_airline_stats_value);

    new_manager->nationality_stats = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_nested_hash_table);

    return new_manager;
} 

/**
 * Getters de Catálogos
 */
FLIGHTS_C get_flights_c(MANAGER_C manager)      { return manager ? manager->flights : NULL; }
PASSENGERS_C get_passengers_c(MANAGER_C manager) { return manager ? manager->passengers : NULL; }
AIRCRAFTS_C get_aircrafts_c(MANAGER_C manager)   { return manager ? manager->aircrafts : NULL; }
RESERVATIONS_C get_reservations_c(MANAGER_C manager) { return manager ? manager->reservations : NULL; }
AIRPORTS_C get_airports_c(MANAGER_C manager)    { return manager ? manager->airports : NULL; }

/**
 * Setters e getters para Tabelas Auxiliares
 */
void set_departures_by_date(MANAGER_C manager, GHashTable *table) {
    if (manager) manager->departures_by_date = table;
}

GHashTable* get_departures_by_date(MANAGER_C manager) {
    return manager ? manager->departures_by_date : NULL;
}

/**
 * 🔹 Implementação da lógica de atualização para a Query 5
 */
void update_airline_stats(MANAGER_C manager, const char *airline, int delay) {
    if (!manager || !airline) return;

    AIRLINE_STATS stats = g_hash_table_lookup(manager->airline_stats, airline);
    if (!stats) {
        stats = malloc(sizeof(struct airline_stats));
        stats->airline = g_strdup(airline); /** Preserva o nome real (ex: "Everest Air") */
        stats->total_delay = 0;
        stats->count = 0;
        g_hash_table_insert(manager->airline_stats, g_strdup(airline), stats);
    }
    stats->total_delay += (double)delay;
    stats->count++;
}

GHashTable* get_airline_stats_table(MANAGER_C manager) {
    return manager ? manager->airline_stats : NULL;
}

void update_nationality_stats(MANAGER_C manager, const char *nationality, const char *airport_iata) {
    if (!manager || !nationality || !airport_iata) return;

    // 1. Procurar a tabela de aeroportos desta nacionalidade
    GHashTable *airport_counts = g_hash_table_lookup(manager->nationality_stats, nationality);
    
    if (!airport_counts) {
        // Criar sub-tabela: Chave=IATA (g_free), Valor=Int* (g_free)
        airport_counts = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
        g_hash_table_insert(manager->nationality_stats, g_strdup(nationality), airport_counts);
    }

    // 2. Atualizar o contador do aeroporto
    int *count = g_hash_table_lookup(airport_counts, airport_iata);
    if (!count) {
        count = malloc(sizeof(int));
        *count = 0;
        g_hash_table_insert(airport_counts, g_strdup(airport_iata), count);
    }
    (*count)++;
}

GHashTable* get_nationality_stats_table(MANAGER_C manager) {
    return manager ? manager->nationality_stats : NULL;
}


/**
 * Liberta toda a memória associada.
 */
void free_manager_c(MANAGER_C manager) {
    if (!manager) return;

    free_flight_c(manager->flights);
    free_passenger_c(manager->passengers);
    free_aircraft_c(manager->aircrafts);
    free_reservation_c(manager->reservations);
    free_airport_c(manager->airports);

    if (manager->departures_by_date)
        g_hash_table_destroy(manager->departures_by_date);

    /** 🔹 Liberta a tabela de estatísticas da Q5 */
    if (manager->airline_stats)
        g_hash_table_destroy(manager->airline_stats);

    if (manager->nationality_stats)
        g_hash_table_destroy(manager->nationality_stats);

    free(manager);
}