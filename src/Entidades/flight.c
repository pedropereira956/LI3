#include "Entidades/flight.h"
#include "Utils/utils.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <glib.h>

struct flight {
    gchar *id;
    gchar *departure;         // "YYYY-MM-DD hh:mm"
    gchar *actual_departure;  // pode ser NULL ou "N/A"
    gchar *arrival;
    gchar *actual_arrival;
    FlightStatus status;
    gchar origin[4];          // código IATA (3 letras + '\0')
    gchar destination[4];
    gchar *aircraft;
    gchar *airline;
};

/* ======== Construtor / Destrutor ======== */

FLIGHT create_flight(void) {
    FLIGHT f = malloc(sizeof(struct flight));
    if (!f) return NULL;
    memset(f, 0, sizeof(struct flight));
    return f;
}

void free_flight(void *data) {
    if (!data) return;
    FLIGHT f = data;
    free(f->id);
    free(f->departure);
    free(f->actual_departure);
    free(f->arrival);
    free(f->actual_arrival);
    free(f->aircraft);
    free(f->airline);
    free(f);
}

/* ======== Setters ======== */
void set_flight_id(FLIGHT f, const gchar *id) {
    if (!f) return;
    free(f->id);
    f->id = g_strdup(g_strstrip((gchar*)id)); // remove espaços/newlines
}


void set_flight_departure(FLIGHT f, const gchar *dep) {
    if (!f) return;
    free(f->departure);
    f->departure = g_strdup(dep);
}

void set_flight_actual_departure(FLIGHT f, const gchar *act_dep) {
    if (!f) return;
    free(f->actual_departure);
    f->actual_departure = g_strdup(act_dep);
}

void set_flight_arrival(FLIGHT f, const gchar *arr) {
    if (!f) return;
    free(f->arrival);
    f->arrival = g_strdup(arr);
}

void set_flight_actual_arrival(FLIGHT f, const gchar *act_arr) {
    if (!f) return;
    free(f->actual_arrival);
    f->actual_arrival = g_strdup(act_arr);
}

void set_flight_status(FLIGHT f, FlightStatus status) {
    if (f) f->status = status;
}

void set_flight_origin(FLIGHT f, const gchar *origin) {
    if (!f || !origin) return;
    strncpy(f->origin, origin, 3);
    f->origin[3] = '\0';
}

void set_flight_destination(FLIGHT f, const gchar *destination) {
    if (!f || !destination) return;
    strncpy(f->destination, destination, 3);
    f->destination[3] = '\0';
}

void set_flight_aircraft(FLIGHT f, const gchar *aircraft) {
    if (!f) return;
    free(f->aircraft);
    f->aircraft = g_strdup(aircraft);
}

void set_flight_airline(FLIGHT f, const char *airline) {
    if (f->airline) free(f->airline);
    f->airline = strdup(airline); 
}

/* ======== Getters ======== */

const gchar* get_flight_id(const FLIGHT f)             { return f ? f->id : NULL; }
const gchar* get_flight_departure(const FLIGHT f)      { return f ? f->departure : NULL; }
const gchar* get_flight_actual_departure(const FLIGHT f){ return f ? f->actual_departure : NULL; }
const gchar* get_flight_arrival(const FLIGHT f)        { return f ? f->arrival : NULL; }
const gchar* get_flight_actual_arrival(const FLIGHT f) { return f ? f->actual_arrival : NULL; }
FlightStatus get_flight_status(const FLIGHT f)         { return f ? f->status : CANCELLED; }
const gchar* get_flight_origin(const FLIGHT f)         { return f ? f->origin : NULL; }
const gchar* get_flight_destination(const FLIGHT f)    { return f ? f->destination : NULL; }
const gchar* get_flight_aircraft(const FLIGHT f)       { return f ? f->aircraft : NULL; }
const gchar* get_flight_airline(const FLIGHT f)        { return f ? f->airline : NULL; }

/* ======== Build ======== */

FLIGHT build_flight(char **fields, FlightStatus status) {
    if (!fields) return NULL;

    FLIGHT f = create_flight();
    if (!f) return NULL;

    set_flight_id(f, fields[0]);
    set_flight_departure(f, fields[1]);
    set_flight_actual_departure(f, fields[2]);
    set_flight_arrival(f, fields[3]);
    set_flight_actual_arrival(f, fields[4]);
    set_flight_status(f, status);
    set_flight_origin(f, fields[7]);
    set_flight_destination(f, fields[8]);
    set_flight_aircraft(f, fields[9]);
    set_flight_airline(f, fields[10]);

    return f;
}

/* ======== Print ======== */

void print_flight(FLIGHT f) {
    if (!f) return;

    // const char *status_str = NULL;
    // switch (get_flight_status(f)) {
    //     case ON_TIME:   status_str = "ON_TIME"; break;
    //     case DELAYED:   status_str = "DELAYED"; break;
    //     case CANCELLED: status_str = "CANCELLED"; break;
    //     default:        status_str = "UNKNOWN"; break;
    // }

    // printf("=== FLIGHT INFO ===\n");
    // printf("ID: %s\n", get_flight_id(f));
    // // printf("Status: %s\n", status_str);
    // // printf("Origin: %s\n", get_flight_origin(f));
    // // printf("Destination: %s\n", get_flight_destination(f));
    // printf("Departure: %s\n", get_flight_departure(f));
    // printf("Actual Departure: %s\n", get_flight_actual_departure(f));
    // printf("Arrival: %s\n", get_flight_arrival(f));
    // printf("Actual Arrival: %s\n", get_flight_actual_arrival(f));
    // // printf("Aircraft: %s\n", get_flight_aircraft(f));
    // printf("====================\n\n");
}