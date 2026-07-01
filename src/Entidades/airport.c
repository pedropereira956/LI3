#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "Entidades/airport.h"
#include "Valida/valida_entidade.h"
#include "Utils/utils.h"


struct airport {
    gchar *code;       // IATA code (3 letras)
    gchar *name;
    gchar *city;
    gchar *country;
    AirportType type;
    int total_arrivals;   // NOVO: Contador de chegadas
    int total_departures; // NOVO: Contador de partidas
    //gdouble latitude;
    //gdouble longitude;
    //gchar *icao;       // código ICAO
};

/* ======== Base ======== */

AIRPORT create_airport(void) {
    AIRPORT a = malloc(sizeof(struct airport));
    if (!a) return NULL;
    memset(a, 0, sizeof(struct airport)); // Isto inicializa os ints a 0
    return a;
}

void free_airport(void *data) {
    if (!data) return;
    AIRPORT a = data;
    free(a->code);
    free(a->name);
    free(a->city);
    free(a->country);
    free(a);
}

/* ======== Setters ======== */

void set_airport_code(AIRPORT a, const gchar *code) {
    if (!a) return;
    free(a->code);
    a->code = g_strdup(code);
}

void set_airport_name(AIRPORT a, const gchar *name) {
    if (!a) return;
    free(a->name);
    a->name = g_strdup(name);
}

void set_airport_city(AIRPORT a, const gchar *city) {
    if (!a) return;
    free(a->city);
    a->city = g_strdup(city);
}

void set_airport_country(AIRPORT a, const gchar *country) {
    if (!a) return;
    free(a->country);
    a->country = g_strdup(country);
}

/*
void set_airport_latitude(AIRPORT a, gdouble latitude) {
    if (!a) return;
    a->latitude = latitude;
}

void set_airport_longitude(AIRPORT a, gdouble longitude) {
    if (!a) return;
    a->longitude = longitude;
}

void set_airport_icao(AIRPORT a, const gchar *icao) {
    if (!a) return;
    free(a->icao);
    a->icao = g_strdup(icao);
}
*/

void set_airport_type(AIRPORT a, const gchar *type_str) {
    if (!a || !type_str) return;

    AirportType type = parse_airport_type(type_str);
    a->type = type;
}

/* ======== NOVOS Incrementadores (Para Q1) ======== */

void inc_airport_arrivals(AIRPORT a) {
    if (a) a->total_arrivals++;
}

void inc_airport_departures(AIRPORT a) {
    if (a) a->total_departures++;
}

/* ======== Getters ======== */

const gchar* get_airport_code(const AIRPORT a) { return a ? a->code : NULL; }
const gchar* get_airport_name(const AIRPORT a) { return a ? a->name : NULL; }
const gchar* get_airport_city(const AIRPORT a) { return a ? a->city : NULL; }
const gchar* get_airport_country(const AIRPORT a) { return a ? a->country : NULL; }
// gdouble get_airport_latitude(const AIRPORT a) { return a ? a->latitude : 0.0; }
// gdouble get_airport_longitude(const AIRPORT a) { return a ? a->longitude : 0.0; }
// const gchar* get_airport_icao(const AIRPORT a) { return a ? a->icao : NULL; }
AirportType get_airport_type(const AIRPORT a) { return a ? a->type : SMALL_AIRPORT; }

// Novos Getters para contagem
int get_airport_total_arrivals(const AIRPORT a) { return a ? a->total_arrivals : 0; }
int get_airport_total_departures(const AIRPORT a) { return a ? a->total_departures : 0; }

// Auxiliar para a Query 1 (Retorna string alocada para facilitar uso no g_string_printf)
char *get_airport_type_string(const AIRPORT a) {
    if (!a) return g_strdup("");
    switch (a->type) {
        case SMALL_AIRPORT: return g_strdup("small_airport");
        case MEDIUM_AIRPORT: return g_strdup("medium_airport");
        case LARGE_AIRPORT: return g_strdup("large_airport");
        case HELIPORT: return g_strdup("heliport");
        case SEAPLANE_BASE: return g_strdup("seaplane_base");
        default: return g_strdup("unknown");
    }
}

AIRPORT build_airport(char **fields) {
    if (!fields) return 0;
    if (!verify_airport(fields)) return 0;

    AIRPORT a = create_airport();
    if (!a) return 0;

    // Preencher dados
    set_airport_code(a, fields[0]);
    set_airport_name(a, fields[1]);
    set_airport_city(a, fields[2]);
    set_airport_country(a, fields[3]);
    set_airport_type(a, fields[7]);

    return a;
}


void print_airport(AIRPORT a) {
    if (!a) return;

    const gchar *code     = get_airport_code(a);     // Código do aeroporto (ex: KOJ)
    const gchar *name     = get_airport_name(a);     // Nome do aeroporto (ex: Kagoshima Airport)
    const gchar *city     = get_airport_city(a);     // Cidade (ex: Kagoshima)
    const gchar *country  = get_airport_country(a);  // País (ex: JP)
    AirportType type      = get_airport_type(a);     // Tipo do aeroporto (enum)

    // Converte o enum para string legível
    const char *type_str = NULL;
    switch (type) {
        case SMALL_AIRPORT:
            type_str = "small_airport";
            break;
        case MEDIUM_AIRPORT:
            type_str = "medium_airport";
            break;
        case LARGE_AIRPORT:
            type_str = "large_airport";
            break;
        case HELIPORT:
            type_str = "heliport";
            break;
        case SEAPLANE_BASE:
            type_str = "seaplane_base";
            break;
        default:
            type_str = "unknown";
            break;
    }

    // Imprime no formato desejado
    printf("%s,%s,%s,%s,%s\n", 
            code ? code : "(null)", 
            name ? name : "(null)", 
            city ? city : "(null)", 
            country ? country : "(null)", 
            type_str);
}