#include "menuNdata/departureStats.h"
#include "Catalogos/flights_c.h"
#include "Entidades/flight.h"
#include "Catalogos/manager_c.h"
#include <string.h>
#include <stdio.h>

void build_departures_by_date(MANAGER_C manager) {
    if (!manager) return;

    FLIGHTS_C flights_catalog = get_flights_c(manager);
    GHashTable *flights = get_hash_table_flight(flights_catalog);

    if (!flights) return;

    // data -> (hash aeroporto -> count)
    GHashTable *departures_by_date = g_hash_table_new_full(
        g_str_hash, g_str_equal,
        g_free, (GDestroyNotify) g_hash_table_destroy
    );

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, flights);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        FLIGHT f = (FLIGHT)value;

        FlightStatus status = get_flight_status(f);
        if (status == CANCELLED)
            continue;

        const gchar *date = get_flight_actual_departure(f);
        const gchar *origin = get_flight_origin(f);

        if (!date || !origin) continue;

        gchar date_key[11];
        strncpy(date_key, date, 10);
        date_key[10] = '\0';

        GHashTable *daily = g_hash_table_lookup(departures_by_date, date_key);
        if (!daily) {
            daily = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
            g_hash_table_insert(departures_by_date, g_strdup(date_key), daily);
        }

        gpointer val = g_hash_table_lookup(daily, origin);
        guint count = val ? GPOINTER_TO_UINT(val) : 0;
        g_hash_table_replace(daily, g_strdup(origin), GUINT_TO_POINTER(count + 1));
    }

    set_departures_by_date(manager, departures_by_date);
}

void print_departures_by_date(MANAGER_C manager) {
    GHashTable *main_table = get_departures_by_date(manager);
    if (!main_table) {
        printf("[DEBUG] departures_by_date está vazia.\n");
        return;
    }

    // Número total de dias = número de chaves na hash principal
    guint total_days = g_hash_table_size(main_table);

    // Para contar aeroportos únicos em todos os dias
    GHashTable *unique_airports = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    // Ordena as datas
    GList *dates = g_hash_table_get_keys(main_table);
    dates = g_list_sort(dates, (GCompareFunc)g_strcmp0);

    printf("\n==================== DEPARTURES BY DATE ====================\n");

    for (GList *d = dates; d; d = d->next) {
        const gchar *date = d->data;
        GHashTable *daily = g_hash_table_lookup(main_table, date);
        if (!daily) continue;

        printf("\n📅 Data: %s\n", date);
        printf("-----------------------------------------\n");
        printf("%-6s | %s\n", "IATA", "Departures");
        printf("---------------------------\n");

        // Ordena aeroportos
        GList *airports = g_hash_table_get_keys(daily);
        airports = g_list_sort(airports, (GCompareFunc)g_strcmp0);

        for (GList *a = airports; a; a = a->next) {
            const gchar *airport_code = a->data;
            guint count = GPOINTER_TO_UINT(g_hash_table_lookup(daily, airport_code));

            printf("%-6s | %u\n", airport_code, count);

            // adiciona ao set de aeroportos únicos
            if (!g_hash_table_contains(unique_airports, airport_code))
                g_hash_table_insert(unique_airports, g_strdup(airport_code), NULL);
        }

        g_list_free(airports);
    }

    guint total_airports = g_hash_table_size(unique_airports);

    printf("============================================================\n");
    printf("📊 Total de dias registados: %u\n", total_days);
    printf("🛫 Total de aeroportos únicos: %u\n", total_airports);
    printf("============================================================\n\n");

    // Liberta memória auxiliar
    g_list_free(dates);
    g_hash_table_destroy(unique_airports);
}
