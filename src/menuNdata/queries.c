#include "menuNdata/queries.h"
#include "Utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <math.h>

// Includes das Entidades
#include "Entidades/airport.h"
#include "Entidades/aircraft.h"
#include "Entidades/reservation.h"
#include "Entidades/flight.h"

// Includes dos Catálogos
#include "Catalogos/airports_c.h"
#include "Catalogos/aircrafts_c.h"
#include "Catalogos/manager_c.h"

// ============================================================================
// QUERY 1: Resumo de Aeroporto
// Output: code;name;city;country;type
// ============================================================================
void* query1(MANAGER_C catalog, char** args, int format) {
    if (!catalog || !args || !args[0]) return NULL;

    // 1. Define o separador: Se format == 1 (1S), usa '=', senão usa ';'
    char sep = (format == 1) ? '=' : ';';

    // 2. Normalizar ID para Maiúsculas (CRUCIAL para encontrar "opo" como "OPO")
    char *airport_id = g_ascii_strup(args[0], -1);

    AIRPORTS_C airports_catalog = get_airports_c(catalog);

    // Usa get_airport_by_code (nome corrigido)
    AIRPORT a = get_airport_by_code(airports_catalog, airport_id);
    g_free(airport_id); // Liberta a string temporária

    if (!a) {
        // MANTIDO: Retorna linha vazia se não encontrar
        char *output = malloc(2);
        if (!output) return NULL;
        output[0] = '\n';
        output[1] = '\0';
        return output;
    }

    const gchar *code    = get_airport_code(a);
    const gchar *name    = get_airport_name(a);
    const gchar *city    = get_airport_city(a);
    const gchar *country = get_airport_country(a);
    AirportType type     = get_airport_type(a);
    
    // NOVOS DADOS
    int arrivals = get_airport_total_arrivals(a);
    int departures = get_airport_total_departures(a);

    const char *type_str =
        (type == SMALL_AIRPORT)  ? "small_airport"  :
        (type == MEDIUM_AIRPORT) ? "medium_airport" :
        (type == LARGE_AIRPORT)  ? "large_airport"  :
        (type == HELIPORT)       ? "heliport"       :
        (type == SEAPLANE_BASE)  ? "seaplane_base"  : "unknown";

    char *output = malloc(1024);
    if (!output) return NULL;

    // Formatação dinâmica com 'sep' e os NOVOS CONTADORES no fim
    snprintf(output, 1024, "%s%c%s%c%s%c%s%c%s%c%d%c%d\n",
             code    ? code    : "", sep,
             name    ? name    : "", sep,
             city    ? city    : "", sep,
             country ? country : "", sep,
             type_str, sep,
             arrivals, sep,
             departures);

    return output;
}

// ============================================================================
// QUERY 2: Top N Voos
// Output: code;manufacturer;model;flights
// ============================================================================
void* query2(MANAGER_C catalog, char** args, int format) {
    if (!catalog || !args || !args[0]) return NULL;

    // 1. Define o separador
    char sep = (format == 1) ? '=' : ';';

    int top_n = atoi(args[0]);
    const gchar *filter_manufacturer = (args[1] && strlen(args[1]) > 0) ? args[1] : NULL;

    AIRCRAFTS_C aircrafts_catalog = get_aircrafts_c(catalog);
    if (!aircrafts_catalog) return NULL;

    GList *ranking = get_sorted_flight_ranking(aircrafts_catalog);
    if (!ranking) return NULL;

    GString *result = g_string_new("");

    int count = 0;
    for (GList *it = ranking; it && count < top_n; it = it->next) {
        AircraftCount *ac = (AircraftCount *)it->data;
        const gchar *aircraft_id = get_aircraft_count_id(ac);
        guint flight_count = get_aircraft_count_value(ac);
    
        AIRCRAFT aircraft = get_aircraft_by_id(aircrafts_catalog, (gchar *)aircraft_id);
        if (!aircraft) continue;
    
        const gchar *manufacturer = get_aircraft_manufacturer(aircraft);
        const gchar *model = get_aircraft_model(aircraft);
    
        // Filtro
        if (filter_manufacturer) {
            if (g_ascii_strcasecmp(filter_manufacturer, manufacturer) != 0)
                continue;
        }
    
        // 2. Append com separador dinâmico
        g_string_append_printf(result, "%s%c%s%c%s%c%u\n",
                               aircraft_id, sep,
                               manufacturer ? manufacturer : "", sep,
                               model ? model : "", sep,
                               flight_count);
        count++;
    }
    
    // Se não houver resultados
    if (result->len == 0) g_string_append_c(result, '\n');

    return g_string_free(result, FALSE);
}

// ============================================================================
// QUERY 3: Voos por Data (Aeroporto com mais partidas)
// Output: code;name;city;country;total_flights
// ============================================================================
void* query3(MANAGER_C manager, char** args, int format) {
    if (!manager || !args || !args[0] || !args[1]) return NULL;

    // 1. Define o separador
    char sep = (format == 1) ? '=' : ';';

    const gchar *start_date = args[0];
    const gchar *end_date   = args[1];

    GHashTable *by_date = get_departures_by_date(manager);
    if (!by_date) return NULL;

    // Acumulador
    GHashTable *accumulator = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    GList *dates = g_hash_table_get_keys(by_date);
    dates = g_list_sort(dates, (GCompareFunc)g_strcmp0);

    for (GList *d = dates; d; d = d->next) {
        const gchar *date = d->data;
        if (g_strcmp0(date, start_date) < 0 || g_strcmp0(date, end_date) > 0)
            continue;

        GHashTable *daily = g_hash_table_lookup(by_date, date);
        if (!daily) continue;

        GHashTableIter inner;
        gpointer akey, aval;
        g_hash_table_iter_init(&inner, daily);

        while (g_hash_table_iter_next(&inner, &akey, &aval)) {
            const gchar *airport = akey;
            guint count = GPOINTER_TO_UINT(aval);

            gpointer val = g_hash_table_lookup(accumulator, airport);
            guint total = val ? GPOINTER_TO_UINT(val) : 0;
            g_hash_table_replace(accumulator, g_strdup(airport), GUINT_TO_POINTER(total + count));
        }
    }

    gchar *best_airport = NULL;
    guint best_count = 0;

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, accumulator);
    
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        const gchar *airport = key;
        guint count = GPOINTER_TO_UINT(value);
        // Desempate: Maior contagem ou ordem alfabética do código
        if (count > best_count || (count == best_count && (!best_airport || g_strcmp0(airport, best_airport) < 0))) {
            best_airport = (gchar*)airport;
            best_count = count;
        }
    }

    // 2. Output
    char *output = malloc(1024);
    if (!output) {
        g_list_free(dates);
        g_hash_table_destroy(accumulator);
        return NULL;
    }

    if (best_airport) {
        AIRPORTS_C airports = get_airports_c(manager);
        AIRPORT a = get_airport_by_code(airports, best_airport);

        const gchar *name = a ? get_airport_name(a) : "";
        const gchar *city = a ? get_airport_city(a) : "";
        const gchar *country = a ? get_airport_country(a) : "";

        snprintf(output, 1024, "%s%c%s%c%s%c%s%c%u\n",
                 best_airport, sep,
                 name, sep,
                 city, sep,
                 country, sep,
                 best_count);
    } else {
        snprintf(output, 1024, "\n");
    }

    g_list_free(dates);
    g_hash_table_destroy(accumulator);
    return output;
}


typedef struct {
    char *airline;
    double total_delay;
    int count;
} AirlineStats;



static gint compare_q5(gconstpointer a, gconstpointer b) {
    AIRLINE_STATS sA = (AIRLINE_STATS)a;
    AIRLINE_STATS sB = (AIRLINE_STATS)b;
    
    double avgA = sA->total_delay / sA->count;
    double avgB = sB->total_delay / sB->count;

    // Arredondamento para 3 casas decimais antes da comparação
    // Multiplicamos por 1000, arredondamos e comparamos os inteiros resultantes
    double roundA = round(avgA * 1000.0) / 1000.0;
    double roundB = round(avgB * 1000.0) / 1000.0;

    if (roundA > roundB) return -1;
    if (roundA < roundB) return 1;
    
    // Critério de desempate: Ordem alfabética do nome da companhia (ASC)
    return strcmp(sA->airline, sB->airline);
}

void* query5(MANAGER_C catalog, char** args, int format) {
    if (!catalog || !args[0]) return NULL;
    int N = atoi(args[0]);
    char sep = (format == 1) ? '=' : ';';

    // 1. Obtemos a tabela que já foi preenchida durante o parsing
    GHashTable *stats_table = get_airline_stats_table(catalog);
    if (!stats_table) return NULL;

    // 2. Extraímos os valores para uma lista apenas para ordenar
    // Nota: g_hash_table_get_values cria uma lista nova que precisa de g_list_free
    GList *list = g_hash_table_get_values(stats_table);

    // 3. Ordenamos usando o critério: Atraso Médio (DESC) e Nome (ASC)
    list = g_list_sort(list, compare_q5);

    GString *output = g_string_new("");
    int i = 0;

    // 4. Percorremos apenas os primeiros N elementos
    for (GList *curr = list; curr != NULL && i < N; curr = curr->next, i++) {
        // Usamos a estrutura AIRLINE_STATS que definiste no manager_c.c
        AIRLINE_STATS s = (AIRLINE_STATS)curr->data;
        
        double avg = s->total_delay / s->count;
        
        // Formata com 3 casas decimais conforme o enunciado
        g_string_append_printf(output, "%s%c%d%c%.3f\n", 
                               s->airline, sep, s->count, sep, avg);
    }

    // 5. Libertamos a lista temporária (mas não os dados dentro dela!)
    g_list_free(list);

    // Retornamos a string (o free_query tratará do resto)
    return g_string_free(output, FALSE);
}

// ============================================================================
// QUERY 6: Aeroporto de destino mais comum por nacionalidade
// Output: code;arrival_count
// ============================================================================
void* query6(MANAGER_C catalog, char** args, int format) {
    if (!catalog || !args || !args[0]) return NULL;

    char sep = (format == 1) ? '=' : ';';
    const char *target_nationality = args[0];

    // 1. Obtém a tabela principal de nacionalidades (Nacionalidade -> GHashTable)
    GHashTable *nat_stats = get_nationality_stats_table(catalog);
    if (!nat_stats) return NULL;

    // 2. Procura a sub-tabela específica da nacionalidade fornecida
    GHashTable *airport_counts = g_hash_table_lookup(nat_stats, target_nationality);

    // Se a nacionalidade não existir, retorna uma linha vazia (conforme enunciado)
    if (!airport_counts) {
        char *empty = malloc(2);
        if (empty) { empty[0] = '\n'; empty[1] = '\0'; }
        return empty;
    }

    char *best_airport = NULL;
    int max_count = -1;

    // 3. Itera sobre a sub-tabela (Aeroporto -> Contador) para encontrar o máximo
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, airport_counts);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        char *current_airport = (char *)key;
        int current_count = *(int *)value;

        // Critério de seleção:
        // a) Maior contagem
        // b) Se contagem igual, menor código IATA lexicograficamente
        if (current_count > max_count || 
           (current_count == max_count && g_strcmp0(current_airport, best_airport) < 0)) {
            max_count = current_count;
            best_airport = current_airport;
        }
    }

    // 4. Formata o output final
    char *output = malloc(256);
    if (output && best_airport) {
        snprintf(output, 256, "%s%c%d\n", best_airport, sep, max_count);
    } else if (output) {
        output[0] = '\n'; output[1] = '\0';
    }

    return output;
}