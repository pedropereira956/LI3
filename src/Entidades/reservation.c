#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "Entidades/reservation.h"

struct reservation {
    gchar *id;
    GList *flight_ids;           // lista de strings (IDs dos voos)
    gchar *document_number;
    // gchar *seat;
    // gdouble price;
    // gboolean extra_luggage;
    // gboolean priority_boarding;
};

/* ======== Funções base ======== */

RESERVATION create_reservation(void) {
    RESERVATION r = malloc(sizeof(struct reservation));
    if (!r) return NULL;
    memset(r, 0, sizeof(struct reservation));
    return r;
}

void free_reservation(void *data) {
    if (!data) return;
    RESERVATION r = data;

    free(r->id);
    g_list_free_full(r->flight_ids, g_free);
    free(r->document_number);
    //free(r->seat);
    free(r);
}

/* ======== Setters ======== */

void set_reservation_id(RESERVATION r, const gchar *id) {
    if (!r) return;
    free(r->id);
    r->id = g_strdup(id);
}

void set_reservation_flight_ids(RESERVATION r, const gchar *flight_ids_str) {
    if (!r) return;

    if (r->flight_ids) {
        g_list_free_full(r->flight_ids, g_free);
        r->flight_ids = NULL;
    }

    if (!flight_ids_str || strlen(flight_ids_str) == 0)
        return;

    gchar **tokens = g_strsplit(flight_ids_str, ",", -1);
    for (int i = 0; tokens[i] != NULL; i++) {
        gchar *id = g_strdup(g_strstrip(tokens[i]));
        r->flight_ids = g_list_append(r->flight_ids, id);
    }
    g_strfreev(tokens);
}

void set_reservation_document_number(RESERVATION r, const gchar *document_number) {
    if (!r) return;
    free(r->document_number);
    r->document_number = g_strdup(document_number);
}
/*
void set_reservation_seat(RESERVATION r, const gchar *seat) {
    if (!r) return;
    free(r->seat);
    r->seat = g_strdup(seat);
}

void set_reservation_price(RESERVATION r, gdouble price) {
    if (!r) return;
    r->price = price;
}

void set_reservation_extra_luggage(RESERVATION r, gboolean extra_luggage) {
    if (!r) return;
    r->extra_luggage = extra_luggage;
}

void set_reservation_priority_boarding(RESERVATION r, gboolean priority_boarding) {
    if (!r) return;
    r->priority_boarding = priority_boarding;
}
*/

/* ======== Getters ======== */

const gchar* get_reservation_id(const RESERVATION r) {
    return r ? r->id : NULL;
}

GList* get_reservation_flight_ids(const RESERVATION r) {
    return r ? r->flight_ids : NULL;
}

const gchar* get_reservation_document_number(const RESERVATION r) {
    return r ? r->document_number : NULL;
}
/*
const gchar* get_reservation_seat(const RESERVATION r) {
    return r ? r->seat : NULL;
}

gdouble get_reservation_price(const RESERVATION r) {
    return r ? r->price : 0.0;
}

gboolean get_reservation_extra_luggage(const RESERVATION r) {
    return r ? r->extra_luggage : FALSE;
}

gboolean get_reservation_priority_boarding(const RESERVATION r) {
    return r ? r->priority_boarding : FALSE;
}
*/


RESERVATION build_reservation(char **fields) {
    if (!fields)
        return NULL;

    RESERVATION r = create_reservation();
    if (!r)
        return NULL;

    // Preencher os campos diretamente
    set_reservation_id(r, fields[0]);
    set_reservation_flight_ids(r, fields[1]);
    set_reservation_document_number(r, fields[2]);

    return r; 
}


void print_reservation(RESERVATION r) {
    if (!r) {
        printf("(Reserva nula)\n");
        return;
    }

    printf("===== RESERVATION INFO =====\n");
    printf("ID: %s\n", get_reservation_id(r));
    printf("Document: %s\n", get_reservation_document_number(r));


    // Lista de flight IDs
    printf("Flights: ");
    GList *flights = get_reservation_flight_ids(r);
    if (!flights) {
        printf("(none)\n");
    } else {
        for (GList *l = flights; l != NULL; l = l->next) {
            printf("%s", (gchar *)l->data);
            if (l->next) printf(", ");
        }
        printf("\n");
    }

    printf("=============================\n\n");
}