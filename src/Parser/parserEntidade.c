#include <stdlib.h>
#include <stdio.h>
#include "Parser/parserEntidade.h"
#include "Entidades/aircraft.h"
#include "Entidades/passenger.h"
#include "Entidades/airport.h"
#include "Catalogos/airports_c.h"
#include "Catalogos/passengers_c.h"
#include "Catalogos/aircrafts_c.h"
#include "Valida/valida_entidade.h"
#include "Valida/validates.h"
#include "Utils/utils.h"
#include "Catalogos/manager_c.h"

static void write_error_line(FILE *error_f, char **fields) {
    for (int i = 0; fields[i] != NULL; i++) {
        fprintf(error_f, "\"%s\"%s", fields[i], (fields[i + 1] ? "," : "\n"));
    }
}


int process_passenger(char **fields, PASSENGERS_C catalog, FILE *error_f) {
    if (!fields || !catalog) return 0;

    if (!verify_passenger(fields)) {
        write_error_line(error_f, fields);
        return 0;
    }

    PASSENGER p = build_passenger(fields);
    if (!p) {
        fprintf(stderr, "Erro ao criar objeto PASSENGER\n");
        return 0;
    }

    insert_passenger_c(p, catalog);
    return 1; // Sucesso
}


int process_aircraft(char **fields, AIRCRAFTS_C catalog, FILE *error_f) {
    if (!fields || !catalog) return 0;

    if (!verify_aircraft(fields)) {
        write_error_line(error_f, fields);
        return 0;
    }

    AIRCRAFT a = build_aircraft(fields);
    if (!a) {
        fprintf(stderr, "Erro ao criar objeto AIRCRAFT\n");
        return 0;
    }

    insert_aircraft_c(a, catalog);
    return 1; // Sucesso
}


int process_airport(char **fields, AIRPORTS_C catalog, FILE *error_f) {
    if (!fields || !catalog) return 0;

    if (!verify_airport(fields)) {
        write_error_line(error_f, fields);
        return 0;
    }

    AIRPORT a = build_airport(fields);
    if (!a) {
        fprintf(stderr, "Erro ao criar objeto AIRPORT\n");
        return 0;
    }

    insert_airport_c(a, catalog);
    return 1; // Sucesso
}




int process_flight(char **fields, MANAGER_C manager_catalog, FILE *error_f) {
    if (!fields || !manager_catalog) return 0;

    // 1. Validação estrutural do voo (formatos, datas, etc.)
    if (!verify_flight(fields)) {
        write_error_line(error_f, fields);
        return 0;
    }

    // 2. Validação lógica: Verificar se a aeronave existe no catálogo
    AIRCRAFTS_C aircrafts_catalog = get_aircrafts_c(manager_catalog);
    GHashTable *table_air = get_hash_table_aircraft(aircrafts_catalog);
    
    // O ID da aeronave está na coluna J (índice 9)
    char *id_upper = g_ascii_strup(fields[9], -1);
    if (!table_air || !g_hash_table_lookup(table_air, id_upper)) {
        g_free(id_upper);
        write_error_line(error_f, fields);
        return 0;
    }
    g_free(id_upper);

    // 3. Determinar o estado do voo (Coluna G - Índice 6)
    FlightStatus status = parse_flight_status(fields[6]);
    
    // 4. Criar o objeto FLIGHT (Internamente deve ler a Airline do Índice 10)
    FLIGHT f = build_flight(fields, status);
    if (!f) return 0;

    // 5. Inserção no catálogo principal de voos
    FLIGHTS_C flights_catalog = get_flights_c(manager_catalog);
    insert_flight_c(f, flights_catalog);

    // 6. --- LÓGICA OTIMIZADA PARA Q5 ---
    // Agregação imediata para evitar lentidão nas queries
    if (status == DELAYED) {
        const char *airline_name = get_flight_airline(f);
        
        // Calcula a diferença entre partida estimada (Col B) e real (Col C)
        int delay = calculate_delay_minutes(get_flight_departure(f), get_flight_actual_departure(f));
        
        // Apenas contabiliza se houver um atraso real (> 0)
        // Isto corrige a contagem de 50883 para 34398
        if (delay > 0) {
            update_airline_stats(manager_catalog, airline_name, delay);
        }
    } // Fecho do bloco DELAYED corrigido

    // 7. Atualização dos contadores de voos da aeronave específica
    update_flight_count(get_aircrafts_c(manager_catalog), get_flight_aircraft(f), fields[6]);

    return 1;
}


int process_reservation(char **fields, MANAGER_C manager_catalog, FILE *error_f) {
    if (!fields || !manager_catalog) return 0;

    // 1. Obter o catálogo de reservas
    RESERVATIONS_C res_catalog = get_reservations_c(manager_catalog);

    // 2. MUDANÇA CRÍTICA: Verificar se o ID da reserva já existe para evitar duplicados
    if (get_reservation_by_id(res_catalog, fields[0]) != NULL) {
        return 0; 
    }

    // 3. Validação da estrutura da reserva
    if (!verify_reservation(fields)) { 
        write_error_line(error_f, fields); 
        return 0; 
    }
    
    FLIGHTS_C flights_catalog = get_flights_c(manager_catalog);
    PASSENGERS_C passengers_catalog = get_passengers_c(manager_catalog);
    AIRPORTS_C airports_catalog = get_airports_c(manager_catalog);

    // 4. Parse da lista de voos associados à reserva
    GList *flight_ids = parse_flight_ids_to_list(fields[1]);
    if (!flight_ids) { 
        write_error_line(error_f, fields); 
        return 0; 
    }

    // 5. Validações lógicas (existência de voos, conexões e passageiro)
    if (!validate_reservation_flights_list(flight_ids, flights_catalog)) { 
        write_error_line(error_f, fields); 
        g_list_free_full(flight_ids, g_free); 
        return 0; 
    }
    if (!validate_connected_flights_list(flight_ids, flights_catalog)) { 
        write_error_line(error_f, fields); 
        g_list_free_full(flight_ids, g_free); 
        return 0; 
    }
    if (!validate_reservation_document(fields[2], passengers_catalog)) { 
        write_error_line(error_f, fields); 
        g_list_free_full(flight_ids, g_free); 
        return 0; 
    }

    // 6. Criar e inserir o objeto reserva no catálogo
    RESERVATION r = build_reservation(fields);
    if (!r) { 
        g_list_free_full(flight_ids, g_free); 
        return 0; 
    }
    insert_reservation_c(r, res_catalog);

    // 🔹 PREPARAÇÃO PARA QUERY 6: Obter nacionalidade do passageiro 🔹
    // fields[2] contém o ID do passageiro associado à reserva
    PASSENGER p = get_passenger_by_id(passengers_catalog, fields[2]);
    const char *nat = p ? get_passenger_nationality(p) : NULL;

    // 7. Atualizar as estatísticas dos aeroportos e nacionalidades
    for (GList *l = flight_ids; l != NULL; l = l->next) {
        char *flight_id = (char *)l->data;
        FLIGHT flight = get_flight_by_id(flights_catalog, flight_id);

        if (flight) {
            FlightStatus status = get_flight_status(flight); 

            // Apenas considera voos com estado diferente de cancelado
            if (status != CANCELLED) {
                const char *origin_code = get_flight_origin(flight);
                const char *dest_code = get_flight_destination(flight);
                
                // Atualização para Query 1 (Contagem Geral)
                AIRPORT origin_apt = get_airport_by_code(airports_catalog, origin_code);
                AIRPORT dest_apt = get_airport_by_code(airports_catalog, dest_code);

                if (origin_apt) inc_airport_departures(origin_apt);
                if (dest_apt) inc_airport_arrivals(dest_apt);

                // 🔹 ATUALIZAÇÃO PARA QUERY 6: Nacionalidade -> Destino 🔹
                if (nat && dest_code) {
                    update_nationality_stats(manager_catalog, nat, dest_code);
                }
            }
        }
    }

    g_list_free_full(flight_ids, g_free);
    return 1;
}


int validate_reservation_flights_list(GList *flights, FLIGHTS_C flights_catalog) {
    if (!flights || !flights_catalog){
        return 0;
    }

    guint count = g_list_length(flights);
    if (count < 1 || count > 2) {
        return 0;
    }

    GHashTable *flights_table = get_hash_table_flight(flights_catalog);
    if (!flights_table){
        return 0;
    }


    for (GList *l = flights; l != NULL; l = l->next) {
        const gchar *flight_id = (const gchar *)l->data;
        if (!flight_id || !*flight_id) {
            return 0;
        }
        if (!g_hash_table_contains(flights_table, flight_id)) {
            return 0;
        } // voo inexistente
    }

    return 1; // ✅ todos os voos existem
}



int validate_connected_flights_list(GList *flights, FLIGHTS_C flights_catalog) {
    if (!flights || !flights_catalog)
        return 0;

    guint count = g_list_length(flights);
    if (count != 2)
        return 1; // ✅ válido se só tiver 1 voo (não há ligação para verificar)

    gchar *first_id  = (gchar *)flights->data;
    gchar *second_id = (gchar *)flights->next->data;

    if (!first_id || !second_id)
        return 0;

    FLIGHT first_flight  = get_flight_by_id(flights_catalog, first_id);
    FLIGHT second_flight = get_flight_by_id(flights_catalog, second_id);

    if (!first_flight || !second_flight)
        return 0;

    const gchar *dest1 = get_flight_destination(first_flight);
    const gchar *orig2 = get_flight_origin(second_flight);

    return (dest1 && orig2 && g_strcmp0(dest1, orig2) == 0);
}


int validate_reservation_document(const gchar *document_number, PASSENGERS_C passengers_catalog) {
    if (!document_number || !passengers_catalog)
        return 0;

    // Primeiro valida formato do número do documento
    if (!validate_document_number(document_number))
        return 0;

    // Acede à tabela hash de passageiros
    GHashTable *passengers_table = get_hash_table_passenger(passengers_catalog);
    if (!passengers_table)
        return 0;

    // Verifica se o passageiro existe
    gpointer found = g_hash_table_lookup(passengers_table, document_number);
    return (found != NULL);
}