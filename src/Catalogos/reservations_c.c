#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "Catalogos/reservations_c.h"
#include "Entidades/reservation.h"

struct reservations_catalog {
    GHashTable *reservations; // chave: reservation_id, valor: RESERVATION
};



RESERVATIONS_C create_reservation_catalog(void) {
    RESERVATIONS_C new_catalog = malloc(sizeof(struct reservations_catalog));
    if (!new_catalog) return NULL;

    new_catalog->reservations =
        g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)free_reservation);

    return new_catalog;
}

GHashTable* get_hash_table_reservation(RESERVATIONS_C catalog) {
    if (!catalog) return NULL;
    return catalog->reservations;
}

void free_reservation_c(RESERVATIONS_C catalog) {
    if (!catalog) return;
    g_hash_table_destroy(catalog->reservations);
    free(catalog);
}

void insert_reservation_c(RESERVATION reservation, RESERVATIONS_C catalog) {
    if (!reservation || !catalog) return;

    const gchar *id = get_reservation_id(reservation);
    if (!id) return;

    gchar *key = g_strdup(id);
    g_hash_table_insert(catalog->reservations, key, reservation);
}

RESERVATION get_reservation_by_id(RESERVATIONS_C catalog, const gchar *id) {
    if (!catalog || !id) return NULL;
    return g_hash_table_lookup(catalog->reservations, id);
}

void remove_reservation_from_hash_table(RESERVATIONS_C catalog, const gchar *id) {
    if (!catalog || !id) return;
    g_hash_table_remove(catalog->reservations, id);
}


void print_all_reservations(RESERVATIONS_C catalog) {
    if (!catalog) return;

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, catalog->reservations);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        print_reservation((RESERVATION)value);
    }
}
