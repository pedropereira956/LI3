#include "Entidades/aircraft.h"
#include "Valida/valida_entidade.h"
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <stdio.h>

struct aircraft {
    gchar *identifier;      // tail number
    gchar *manufacturer;    // fabricante
    gchar *model;           // modelo
    //gint year;              // ano de fabrico
    //gint capacity;          // capacidade máxima
    //gint range;             // alcance máximo (km)
};

/* ======== Funções base ======== */

AIRCRAFT create_aircraft(void) {
    AIRCRAFT a = malloc(sizeof(struct aircraft));
    if (!a) return NULL;
    memset(a, 0, sizeof(struct aircraft));
    return a;
}

void free_aircraft(void *data) {
    if (!data) return;
    AIRCRAFT a = data;
    free(a->identifier);
    free(a->manufacturer);
    free(a->model);
    free(a);
}

/* ======== Setters ======== */

void set_aircraft_identifier(AIRCRAFT a, const gchar *identifier) {
    free(a->identifier);
    a->identifier = g_strdup(identifier);
}

void set_aircraft_manufacturer(AIRCRAFT a, const gchar *manufacturer) {
    free(a->manufacturer);
    a->manufacturer = g_strdup(manufacturer);
}

void set_aircraft_model(AIRCRAFT a, const gchar *model) {
    free(a->model);
    a->model = g_strdup(model);
}
/*
void set_aircraft_year(AIRCRAFT a, gint year) {
    a->year = year;
}
 
void set_aircraft_capacity(AIRCRAFT a, gint capacity) {
    a->capacity = capacity;
}

void set_aircraft_range(AIRCRAFT a, gint range) {
    a->range = range;
}
*/

/* ======== Getters ======== */

const gchar* get_aircraft_identifier(const AIRCRAFT a) {
    return a ? a->identifier : NULL;
}

const gchar* get_aircraft_manufacturer(const AIRCRAFT a) {
    return a ? a->manufacturer : NULL;
}

const gchar* get_aircraft_model(const AIRCRAFT a) {
    return a ? a->model : NULL;
}
/*
gint get_aircraft_year(const AIRCRAFT a) {
    return a ? a->year : -1;
}

gint get_aircraft_capacity(const AIRCRAFT a) {
    return a ? a->capacity : -1;
}

gint get_aircraft_range(const AIRCRAFT a) {
    return a ? a->range : -1;
}
*/

// cria o objeto aircraft
AIRCRAFT  build_aircraft(char **fields) {
    if (!fields) return 0;
    if (!verify_aircraft(fields)) return 0;

    AIRCRAFT a = create_aircraft();
    if (!a) return 0;

    // Preencher os dados
    set_aircraft_identifier(a, fields[0]);
    set_aircraft_manufacturer(a, fields[1]);
    set_aircraft_model(a, fields[2]);

    return a;
}

void print_aircraft(AIRCRAFT a) {
    if (!a) return;

    const gchar *identifier   = get_aircraft_identifier(a);
    const gchar *manufacturer = get_aircraft_manufacturer(a);
    const gchar *model        = get_aircraft_model(a);

    printf("Aircraft:\n");
    printf("  Identifier:   %s\n", identifier   ? identifier   : "(null)");
    printf("  Manufacturer: %s\n", manufacturer ? manufacturer : "(null)");
    printf("  Model:        %s\n", model        ? model        : "(null)");
    printf("----------------------------------------\n");
}

