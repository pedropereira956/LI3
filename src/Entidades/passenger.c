#include "Entidades/passenger.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

struct passenger {
    gchar *document_number;
    gchar *first_name;
    gchar *nationality;
    //gchar *last_name;
    //gchar *data_nascimento;           // formato "aaaa-mm-dd"
    
    //gchar gender[2];      // 1 char + '\0'
    //gchar *email;
    //gchar *phone;
    //gchar *address;
};


PASSENGER create_passenger() {
    PASSENGER p = malloc(sizeof(struct passenger));
    if (!p) return NULL;
    // inicializar campos a NULL
    p->document_number = NULL;
    p->first_name = NULL;
    p->nationality = NULL;
    return p;
}


void free_passenger(void *data) {
    if (!data) return;
    PASSENGER p = data;
    free(p->document_number);
    free(p->first_name);
    free(p->nationality);
    // free(p->last_name);
    // free(p->data_nascimento);
    // free(p->email);
    // free(p->phone);
    // free(p->address);
    free(p);
}

/* ======== Setters ======== */
void set_passenger_document_number(PASSENGER p, const gchar *doc) {
    free(p->document_number);
    p->document_number = g_strdup(doc);
}

void set_passenger_first_name(PASSENGER p, const gchar *first) {
    free(p->first_name);
    p->first_name = g_strdup(first);
}

void set_passenger_nationality(PASSENGER p, const gchar *nat) {
    free(p->nationality);
    p->nationality = g_strdup(nat);
}


/*
void set_passenger_last_name(PASSENGER p, const gchar *last) {
    free(p->last_name);
    p->last_name = g_strdup(last);
}

void set_passenger_data_nascimento(PASSENGER p, const gchar *data_nascimento) {
    free(p->data_nascimento);
    p->data_nascimento = g_strdup(data_nascimento);
}

void set_passenger_nationality(PASSENGER p, const gchar *nat) {
    free(p->nationality);
    p->nationality = g_strdup(nat);
}

void set_passenger_gender(PASSENGER p, const gchar *gender) {
    if (!gender) return;
    p->gender[0] = gender[0];
    p->gender[1] = '\0';
}

void set_passenger_email(PASSENGER p, const gchar *email) {
    free(p->email);
    p->email = g_strdup(email);
}

void set_passenger_phone(PASSENGER p, const gchar *phone) {
    free(p->phone);
    p->phone = g_strdup(phone);
}

void set_passenger_address(PASSENGER p, const gchar *addr) {
    free(p->address);
    p->address = g_strdup(addr);
}
*/
/* ======== Getters ======== */
const gchar* get_passenger_document_number(const PASSENGER p) { return p->document_number; }
const gchar* get_passenger_first_name(const PASSENGER p) { return p->first_name; }
const gchar* get_passenger_nationality(const PASSENGER p) { 
    return p ? p->nationality : NULL; 
}
/*
const gchar* get_passenger_last_name(const PASSENGER p) { return p->last_name; }
const gchar* get_passenger_data_nascimento(const PASSENGER p) { return p->data_nascimento; }
const gchar* get_passenger_nationality(const PASSENGER p) { return p->nationality; }
const gchar* get_passenger_gender(const PASSENGER p) { return p->gender; }
const gchar* get_passenger_email(const PASSENGER p) { return p->email; }
const gchar* get_passenger_phone(const PASSENGER p) { return p->phone; }
const gchar* get_passenger_address(const PASSENGER p) { return p->address; }
*/



PASSENGER build_passenger(char **fields) {
    if (!fields) return 0;
    // if (!verify_passenger(fields)) return 0;  
    PASSENGER p = create_passenger();
    if (!p) return 0;

    // Preencher os dados
    set_passenger_document_number(p, fields[0]);
    set_passenger_first_name(p, fields[1]);
    set_passenger_nationality(p, fields[4]);

    return p; 
}


void print_passenger(PASSENGER p) {
    if (!p) return;

    const gchar *doc_number = get_passenger_document_number(p);
    const gchar *first_name = get_passenger_first_name(p);
    const gchar *nationality = get_passenger_nationality(p);

    printf("Passenger:\n");
    printf("  Document Number: %s\n", doc_number ? doc_number : "(null)");
    printf("  First Name: %s\n", first_name ? first_name : "(null)");
    printf("  Nationality: %s\n", nationality ? nationality : "(null)");
    printf("----------------------------------------\n");
}