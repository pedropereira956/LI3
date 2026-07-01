#include "Parser/genericParser.h"
#include "Valida/valida_entidade.h"
#include "Utils/utils.h"
#include "Parser/parserEntidade.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



void parseF(FILE* f, gchar* nome, int max_fields, void* catalog, FILE* error_f) {
    char* line = NULL;
    size_t lsize = 0;

    if (f == NULL) {
        fprintf(stderr, "Error opening file\n");
        return;
    }

    // Cabeçalho
    if (getline(&line, &lsize, f) != -1) {
        fprintf(error_f, "%s", line);
    } else {
        fprintf(stderr, "Error reading first line from file\n");
        free(line);
        return;
    }

    int tipo = -1;
    if (strcmp(nome, "flights") == 0) tipo = 0;
    else if (strcmp(nome, "passengers") == 0) tipo = 1;
    else if (strcmp(nome, "aircrafts") == 0) tipo = 2;
    else if (strcmp(nome, "reservations") == 0) tipo = 3;
    else if (strcmp(nome, "airports") == 0) tipo = 4;

    while (getline(&line, &lsize, f) != -1) {
        line[strcspn(line, "\r\n")] = '\0'; // remove newline

        char** fields = parseL(line, max_fields);
        if (!fields) continue;

        switch (tipo) {
            case 0: process_flight(fields, catalog, error_f); break;
            case 1: process_passenger(fields, (PASSENGERS_C)catalog, error_f); break;
            case 2: process_aircraft(fields, (AIRCRAFTS_C)catalog, error_f); break;
            case 3: process_reservation(fields, catalog, error_f);  break;
            case 4: process_airport(fields, (AIRPORTS_C)catalog, error_f); break;
        }

        // ✅ libertar os campos
        for (int i = 0; fields[i] != NULL; i++)
            free(fields[i]);
        free(fields);
    }

    free(line);
}


char** parseL(char* line, int max_fields) {
    if (!line) return NULL;

    // Copia para poder manipular
    char *copy = g_strdup(line);
    copy[strcspn(copy, "\r\n")] = '\0'; // remove newline

    GPtrArray *fields = g_ptr_array_new();
    char *ptr = copy;
    char *start = ptr;
    gboolean in_quotes = FALSE;

    while (*ptr) {
        if (*ptr == '"') {
            in_quotes = !in_quotes;
        } else if (*ptr == ',' && !in_quotes) {
            *ptr = '\0';
            trim_quotes(start);
            g_ptr_array_add(fields, g_strdup(start));
            start = ptr + 1;
        }
        ptr++;
    }

    // último campo
    trim_quotes(start);
    g_ptr_array_add(fields, g_strdup(start));

    // corta ao máximo permitido
    if (max_fields > 0 && fields->len > (guint) max_fields)
        fields->len = max_fields;

    // fecha com NULL
    g_ptr_array_add(fields, NULL);

    g_free(copy);
    return (char **)g_ptr_array_free(fields, FALSE);
}

