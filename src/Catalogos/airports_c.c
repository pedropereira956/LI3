#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "Catalogos/airports_c.h"
#include "Entidades/airport.h"


struct airports_catalog {
    GHashTable *airports;  // chave: código IATA, valor: ponteiro para AIRPORT
};

AIRPORTS_C create_airport_catalog(void) {
    AIRPORTS_C new_catalog = malloc(sizeof(struct airports_catalog));
    if (!new_catalog) return NULL;

    new_catalog->airports = g_hash_table_new_full(
        g_str_hash, g_str_equal,
        g_free, (GDestroyNotify)free_airport
    );

    return new_catalog;
}

void free_airport_c(AIRPORTS_C catalog) {
    if (!catalog) return;

    if (catalog->airports)
        g_hash_table_destroy(catalog->airports);

    free(catalog);
}


/**
 * Retorna o apontador para a hash table interna.
 */
GHashTable* get_hash_table_airport(AIRPORTS_C catalog) {
    return catalog ? catalog->airports : NULL;
}

/**
 * Insere um aeroporto no catálogo.
 */
void insert_airport_c(AIRPORT airport, AIRPORTS_C catalog) {
    if (!airport || !catalog) return;

    const gchar *key_str = get_airport_code(airport);
    if (!key_str) return;

    gchar *key = g_strdup(key_str);  // duplicar a chave
    g_hash_table_insert(catalog->airports, key, airport);
}

/**
 * Procura um aeroporto pelo código IATA.
 */
AIRPORT get_airport_by_code(AIRPORTS_C catalog, const gchar *code) {
    if (!catalog || !code) return NULL;
    return g_hash_table_lookup(catalog->airports, code);
}

/**
 * Remove um aeroporto do catálogo pelo código IATA.
 */
void remove_airport_from_catalog(AIRPORTS_C catalog, const gchar *code) {
    if (!catalog || !code) return;
    g_hash_table_remove(catalog->airports, code);
}

/**
 * Imprime todos os aeroportos no catálogo.
 */
void print_all_airports(AIRPORTS_C catalog) {
    if (!catalog) return;

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, catalog->airports);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        print_airport((AIRPORT)value);
    }
}




