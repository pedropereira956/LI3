#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "Catalogos/flights_c.h"
#include "Entidades/flight.h"


struct flights_catalog {
    GHashTable* flights; // hashtable de voos com id de voo como chave
};

FLIGHTS_C create_flight_catalog (void) {
    FLIGHTS_C new_catalog = malloc(sizeof(struct flights_catalog)) ;

    new_catalog-> flights = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify) free_flight);

    return new_catalog;
}   

GHashTable* get_hash_table_flight(FLIGHTS_C catalog){
    return catalog->flights;
}

void insert_flight_c(FLIGHT flight,FLIGHTS_C catalog) {

    const gchar* id = get_flight_id(flight); // para nao causar warnings a compilar
    gchar *key = g_strdup(id);
    g_hash_table_insert(catalog->flights,key,flight) ;
}

FLIGHT get_flight_by_id(FLIGHTS_C catalog,gchar* id) {
    return g_hash_table_lookup(catalog->flights,id) ;
}

void remove_flight_from_hash_table(FLIGHTS_C flights, gchar* flight_id) {
    g_hash_table_remove(flights->flights, flight_id);
}

void free_flight_c(FLIGHTS_C catalog) {
    if (!catalog) return;
    g_hash_table_destroy(catalog->flights);
    free(catalog);
}


void print_all_flights(FLIGHTS_C catalog) {
    if (!catalog) return;

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, catalog->flights);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        print_flight((FLIGHT)value);
    }
}

