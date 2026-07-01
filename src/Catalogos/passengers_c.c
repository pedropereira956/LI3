#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "Catalogos/passengers_c.h"
#include "Entidades/passenger.h"

/* Estrutura interna */
struct passengers_catalog {
    GHashTable *passengers; // chave: document number, valor: PASSENGER
};

/* ======== Funções base ======== */
PASSENGERS_C create_passenger_catalog(void) {
    PASSENGERS_C catalog = malloc(sizeof(struct passengers_catalog));
    if (!catalog) return NULL;

    catalog->passengers = g_hash_table_new_full(
        g_str_hash, g_str_equal,
        free,              // liberta a chave
        (GDestroyNotify) free_passenger  // liberta o valor
    );

    return catalog;
}

void free_passenger_catalog(PASSENGERS_C catalog) {
    if (!catalog) return;
    g_hash_table_destroy(catalog->passengers);
    free(catalog);
}

/* ======== Operações ======== */
void insert_passenger_c(PASSENGER passenger, PASSENGERS_C catalog) {
    if (!catalog || !passenger) return;

    const gchar *id = get_passenger_document_number(passenger);
    gchar *key = g_strdup(id);
    g_hash_table_insert(catalog->passengers, key, passenger);
}

PASSENGER get_passenger_by_id(PASSENGERS_C catalog, const gchar *doc_number) {
    if (!catalog || !doc_number) return NULL;
    return g_hash_table_lookup(catalog->passengers, doc_number);
}

void remove_passenger_from_catalog(PASSENGERS_C catalog, const gchar *doc_number) {
    if (!catalog || !doc_number) return;
    g_hash_table_remove(catalog->passengers, doc_number);
}

GHashTable* get_hash_table_passenger(PASSENGERS_C catalog) {
    return catalog ? catalog->passengers : NULL;
}



void free_passenger_c(PASSENGERS_C catalog) {
    if (!catalog) return;
    g_hash_table_destroy(catalog->passengers);
    free(catalog);
}


// /* ======== Util ======== */
void print_all_passengers(PASSENGERS_C catalog) {
    if (!catalog) return;

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, catalog->passengers);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        print_passenger((PASSENGER)value);
    }
}


