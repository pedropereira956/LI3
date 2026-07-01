#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <glib.h>
#include <ctype.h>
#include "Utils/utils.h"
#include <string.h>
#include "Entidades/flight.h"
#include "Entidades/airport.h"
#include "Parser/genericParser.h"
#include "Catalogos/manager_c.h"
#include "Catalogos/airports_c.h"
#include "menuNdata/departureStats.h"
#include <time.h>
#include <stdio.h>




FlightStatus parse_flight_status(const gchar *str) {
    if (!str) return CANCELLED;

    if (g_ascii_strcasecmp(str, "ON TIME") == 0)
        return ON_TIME;
    else if (g_ascii_strcasecmp(str, "DELAYED") == 0)
        return DELAYED;
    else if (g_ascii_strcasecmp(str, "CANCELLED") == 0)
        return CANCELLED;


    return CANCELLED;
}


GList* parse_flight_ids_to_list(const gchar *flight_ids_str) {
    if (!flight_ids_str || !*flight_ids_str)
        return NULL;

    GList *list = NULL;
    gchar **tokens = g_strsplit(flight_ids_str, ",", -1);

    for (gint i = 0; tokens[i] != NULL; i++) {
        gchar *clean = g_strdup(tokens[i]);
        clean = g_strstrip(clean);

        // Remove colchetes e aspas simples
        for (char *p = clean; *p; p++) {
            if (*p == '[' || *p == ']' || *p == '\'' || *p == '"')
                *p = ' ';
        }
        g_strstrip(clean); // tira espaços deixados pela remoção

        if (*clean)
            list = g_list_append(list, g_strdup(clean));
        g_free(clean);
    }

    g_strfreev(tokens);
    return list;
}


AirportType parse_airport_type(const gchar *str) {
    if (!str) return SMALL_AIRPORT;

    if (g_ascii_strcasecmp(str, "small_airport") == 0) return SMALL_AIRPORT;
    if (g_ascii_strcasecmp(str, "medium_airport") == 0) return MEDIUM_AIRPORT;
    if (g_ascii_strcasecmp(str, "large_airport") == 0) return LARGE_AIRPORT;
    if (g_ascii_strcasecmp(str, "heliport") == 0) return HELIPORT;
    if (g_ascii_strcasecmp(str, "seaplane_base") == 0) return SEAPLANE_BASE;

    return SMALL_AIRPORT; // default
}



char* concat(char *s1, char *s2) {

    char *result = malloc(sizeof(char) * (strlen(s1) + strlen(s2) + 1));
    strcpy(result, s1);
    strcat(result, s2);

    return result;
}


void trim_quotes(char *s) {
    if (!s) return;
    size_t len = strlen(s);
    if (len >= 2 && s[0] == '"' && s[len - 1] == '"') {
        s[len - 1] = '\0';
        memmove(s, s + 1, len - 1);
    }
}



int set_catalogs(MANAGER_C manager_catalog, char *path1) {
    // Caminhos completos
    char *flight_path      = concat(path1, "/flights.csv");
    char *passenger_path   = concat(path1, "/passengers.csv");
    char *aircraft_path    = concat(path1, "/aircrafts.csv");
    char *reservation_path = concat(path1, "/reservations.csv");
    char *airport_path     = concat(path1, "/airports.csv");

    // Ficheiros normais
    FILE *flights_file      = fopen(flight_path, "r");
    FILE *passengers_file   = fopen(passenger_path, "r");
    FILE *aircrafts_file    = fopen(aircraft_path, "r");
    FILE *reservations_file = fopen(reservation_path, "r");
    FILE *airports_file     = fopen(airport_path, "r");

    // Ficheiros de erros (atenção: "resultados", não "Resultados")
    FILE *flights_error_file      = fopen("resultados/flights_errors.csv", "w");
    FILE *passengers_error_file   = fopen("resultados/passengers_errors.csv", "w");
    FILE *aircrafts_error_file    = fopen("resultados/aircrafts_errors.csv", "w");
    FILE *reservations_error_file = fopen("resultados/reservations_errors.csv", "w");
    FILE *airports_error_file     = fopen("resultados/airports_errors.csv", "w");

    // Verifica aberturas
    if (!flights_file || !passengers_file || !aircrafts_file ||
        !reservations_file || !airports_file) {
        fprintf(stderr, "Error opening file\n");
        // Liberta memória e fecha os que abriram
        if (flights_file) fclose(flights_file);
        if (passengers_file) fclose(passengers_file);
        if (aircrafts_file) fclose(aircrafts_file);
        if (reservations_file) fclose(reservations_file);
        if (airports_file) fclose(airports_file);
        free(flight_path);
        free(passenger_path);
        free(aircraft_path);
        free(reservation_path);
        free(airport_path);
        return 1; // erro
    }

    // Parseia cada entidade
    PASSENGERS_C passengers_catalog = get_passengers_c(manager_catalog);
    parseF(passengers_file, "passengers", 10, passengers_catalog, passengers_error_file);

    AIRCRAFTS_C aircrafts_catalog = get_aircrafts_c(manager_catalog);
    parseF(aircrafts_file, "aircrafts", 6, aircrafts_catalog, aircrafts_error_file);

    AIRPORTS_C airports_catalog = get_airports_c(manager_catalog);
    parseF(airports_file, "airports", 8, airports_catalog, airports_error_file);


    parseF(flights_file,"flights",12,manager_catalog,flights_error_file);

    build_aircraft_ranking(get_aircrafts_c(manager_catalog));

    build_departures_by_date(manager_catalog);
    
    parseF(reservations_file,"reservations",8,manager_catalog,reservations_error_file);

    


    // Fecha ficheiros e liberta paths
    fclose(flights_file);
    fclose(passengers_file);
    fclose(aircrafts_file);
    fclose(reservations_file);
    fclose(airports_file);

    fclose(flights_error_file);
    fclose(passengers_error_file);
    fclose(aircrafts_error_file);
    fclose(reservations_error_file);
    fclose(airports_error_file);

    free(flight_path);
    free(passenger_path);
    free(aircraft_path);
    free(reservation_path);
    free(airport_path);

    return 1; // sucesso
}



/**
 * Calcula a diferença em minutos entre a partida estimada e a real.
 */
int calculate_delay_minutes(const char *departure, const char *actual_departure) {
    if (!departure || !actual_departure || strcmp(actual_departure, "N/A") == 0) 
        return 0;

    struct tm d1 = {0}, d2 = {0};
    
    // Formato: "YYYY-MM-DD hh:mm:ss"
    if (sscanf(departure, "%d-%d-%d %d:%d:%d", 
               &d1.tm_year, &d1.tm_mon, &d1.tm_mday, 
               &d1.tm_hour, &d1.tm_min, &d1.tm_sec) < 5) return 0;
    
    if (sscanf(actual_departure, "%d-%d-%d %d:%d:%d", 
               &d2.tm_year, &d2.tm_mon, &d2.tm_mday, 
               &d2.tm_hour, &d2.tm_min, &d2.tm_sec) < 5) return 0;

    // Ajustes para as structs tm
    d1.tm_year -= 1900; d1.tm_mon -= 1;
    d2.tm_year -= 1900; d2.tm_mon -= 1;

    time_t t1 = mktime(&d1);
    time_t t2 = mktime(&d2);

    if (t1 == -1 || t2 == -1) return 0;

    // Retorna a diferença em minutos (t2 - t1)
    return (int)(difftime(t2, t1) / 60);
}