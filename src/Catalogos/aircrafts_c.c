#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "Catalogos/aircrafts_c.h"
#include "Entidades/aircraft.h"


struct aircraft_count {
    gchar *id;
    guint count;
};

struct aircrafts_catalog {
    GHashTable *aircrafts;      // id → AIRCRAFT
    GHashTable *flight_counts;  // id → guint (número de voos dessa aeronave)
    GList *sorted_flight_ranking;
};

/* ======== Criação e acesso ======== */

AIRCRAFTS_C create_aircraft_catalog(void) {
    AIRCRAFTS_C new_catalog = malloc(sizeof(struct aircrafts_catalog));
    if (!new_catalog) return NULL;

    new_catalog->aircrafts =
        g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)free_aircraft);

    new_catalog->flight_counts =
        g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    new_catalog->sorted_flight_ranking = NULL;

    return new_catalog;
}

GHashTable* get_hash_table_aircraft(AIRCRAFTS_C catalog) {
    return catalog ? catalog->aircrafts : NULL;
}


GHashTable* get_hash_table_flight_counts(AIRCRAFTS_C catalog) {
    return catalog ? catalog->flight_counts : NULL;
}

GList *get_sorted_flight_ranking(AIRCRAFTS_C catalog) {
    return catalog ? catalog->sorted_flight_ranking : NULL;
}


void free_aircraft_c(AIRCRAFTS_C catalog) {
    if (!catalog) return;

    g_hash_table_destroy(catalog->aircrafts);
    g_hash_table_destroy(catalog->flight_counts);

    if (catalog->sorted_flight_ranking)
        g_list_free_full(catalog->sorted_flight_ranking, (GDestroyNotify)free_aircraft_count);

    free(catalog);
}

void free_aircraft_count(AircraftCount *ac) {
    if (!ac) return;
    g_free(ac->id);
    free(ac);
}


/* ======== Inserir, obter e remover ======== */

void insert_aircraft_c(AIRCRAFT aircraft, AIRCRAFTS_C catalog) {
    if (!aircraft || !catalog) return;

    const gchar *id = get_aircraft_identifier(aircraft);
    gchar *key = g_strdup(id);

    g_hash_table_insert(catalog->aircrafts, key, aircraft);
}

AIRCRAFT get_aircraft_by_id(AIRCRAFTS_C catalog, gchar *id) {
    if (!catalog || !id) return NULL;
    return g_hash_table_lookup(catalog->aircrafts, id);
}

const gchar *get_aircraft_count_id(const AircraftCount *ac) {
    return ac ? ac->id : NULL;
}

guint get_aircraft_count_value(const AircraftCount *ac) {
    return ac ? ac->count : 0;
}


void remove_aircraft_from_hash_table(AIRCRAFTS_C catalog, gchar *aircraft_id) {
    if (!catalog || !aircraft_id) return;
    g_hash_table_remove(catalog->aircrafts, aircraft_id);
}

// /* ======== Util ======== */

void print_all_aircrafts(AIRCRAFTS_C catalog) {
    if (!catalog) return;

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, catalog->aircrafts);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        print_aircraft((AIRCRAFT)value);
    }
}


gint compare_aircraft_count(gconstpointer a, gconstpointer b) {
    const AircraftCount *ac1 = a;
    const AircraftCount *ac2 = b;

    // Primeiro, comparar pelo número de voos (maior primeiro)
    if (ac1->count > ac2->count) return -1;
    if (ac1->count < ac2->count) return 1;

    // Se empatar, comparar alfabeticamente pelo ID
    return g_strcmp0(ac1->id, ac2->id);
}


void update_flight_count(AIRCRAFTS_C catalog, const gchar *aircraft_id, const gchar *status) {
    if (!catalog || !aircraft_id || !status)
        return;

    // ❌ Ignora voos cancelados
    if (g_ascii_strcasecmp(status, "CANCELLED") == 0)
        return;

    GHashTable *counts = get_hash_table_flight_counts(catalog);
    if (!counts) {
        printf("[DEBUG] counts table is NULL!\n");
        return;
    }

    // 🔹 Atualiza o contador
    gpointer value = g_hash_table_lookup(counts, aircraft_id);
    guint count = value ? GPOINTER_TO_UINT(value) : 0;
    count++;

    g_hash_table_replace(counts, g_strdup(aircraft_id), GUINT_TO_POINTER(count));
}


void build_aircraft_ranking(AIRCRAFTS_C catalog) {
    if (!catalog || !catalog->flight_counts)
        return;

    // Se já existir ranking antigo, liberta antes de recriar
    if (catalog->sorted_flight_ranking) {
        g_list_free_full(catalog->sorted_flight_ranking, (GDestroyNotify)free_aircraft_count);
        catalog->sorted_flight_ranking = NULL;
    }

    GList *list = NULL;
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, catalog->flight_counts);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        AircraftCount *ac = malloc(sizeof(AircraftCount));
        ac->id = g_strdup((gchar*) key);
        ac->count = GPOINTER_TO_UINT(value);
        list = g_list_append(list, ac);
    }

    catalog->sorted_flight_ranking = g_list_sort(list, compare_aircraft_count);
}



void print_flight_counts_table(GHashTable *flight_counts) {
    if (!flight_counts) {
        printf("[DEBUG] Flight counts table is NULL.\n");
        return;
    }

    printf("=== FLIGHT COUNTS TABLE ===\n");
    printf("%-15s | %s\n", "Aircraft ID", "Flight Count");
    printf("-----------------|--------------\n");

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, flight_counts);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        const gchar *aircraft_id = key;
        guint count = GPOINTER_TO_UINT(value);
        printf("%-15s | %u\n", aircraft_id, count);
    }

    printf("============================\n");
}


void print_aircraft_ranking(AIRCRAFTS_C catalog) {
    if (!catalog || !catalog->sorted_flight_ranking) {
        printf("[DEBUG] Ranking de aeronaves inexistente ou vazio.\n");
        return;
    }

    GList *ranking = catalog->sorted_flight_ranking;
    guint total = g_list_length(ranking);

    printf("=== RANKING COMPLETO DE AERONAVES ===\n");
    printf("%-15s | %-10s\n", "Aircraft ID", "Flight Count");
    printf("-----------------|--------------\n");

    for (GList *it = ranking; it != NULL; it = it->next) {
        AircraftCount *ac = (AircraftCount *)it->data;
        printf("%-15s | %u\n", ac->id, ac->count);
    }

    printf("=====================================\n");
    printf("Total de aeronaves: %u\n", total);
}
