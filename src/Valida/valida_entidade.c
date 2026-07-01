#include <stdlib.h>
#include <stdio.h>
#include "Valida/validates.h"
#include "Utils/utils.h"
#include "Valida/valida_entidade.h"


int verify_passenger(char **fields) {
    if (!fields) return 0;

    
    for (int i = 0; i <= 8; i++) {
        if (!fields[i] || strlen(fields[i]) == 0)
            return 0;
    }

    if (!validate_document_number(fields[0]))
        return 0;

    if (!validate_date(fields[3]))
        return 0;

    if (!validate_gender(fields[5]))
        return 0;

    if (!validate_email(fields[6]))
        return 0;

    return 1; 
}


int verify_airport(char **fields) {
    if (!fields)
        return 0;

    // Verifica que todos os campos obrigatórios existem e não estão vazios
    for (int i = 0; i <= 7; i++) {
        if (!fields[i] || strlen(fields[i]) == 0)
            return 0;
    }

    const gchar *code = fields[0];
    const gchar *name = fields[1];
    const gchar *city = fields[2];
    const gchar *country = fields[3];
    const gchar *latitude_str = fields[4];
    const gchar *longitude_str = fields[5];
    const gchar *type = fields[7];

    if (!validate_airport_code(code)) {
        return 0 ;
    }


    if (strlen(name) == 0 || strlen(city) == 0 || strlen(country) == 0) {
        return 0;
    }

    if (!validate_latitude(latitude_str)) {
        return 0;
    }

    if (!validate_longitude(longitude_str)) {
        return 0;
    }


    if (!validate_airport_type(type)) {
        return 0;
    }

    return 1; 
}



int verify_aircraft(char **fields) {
    if (!fields) return 0;

    // Verifica se os 6 campos existem e não estão vazios
    for (int i = 0; i < 6; i++) {
        if (!fields[i] || strlen(fields[i]) == 0)
            return 0;
    }

    // O campo 3 (índice 3) é o ano
    if (!validate_year(fields[3])) {
        return 0;
    }

    return 1;
}

int verify_flight(char **fields) {
    if (!fields) return 0;

    // Campos obrigatórios
    for (int i = 0; i < 12; i++)
        if (!fields[i] || !*fields[i])
            return 0;

    // Status válido
    FlightStatus status = parse_flight_status(fields[6]);
    if (status != ON_TIME && status != DELAYED && status != CANCELLED) {
        return 0;
    }

    // 3️⃣ ID e formato das datas
    if (!validate_flight_id(fields[0]))  {
        return 0;
    }
    if (!validate_datetime(fields[1]) || !validate_datetime(fields[3])) {
        return 0;
    } 

    if (status != CANCELLED && (!validate_datetime(fields[2]) || !validate_datetime(fields[4]))) {
        return 0 ;
    }

    // 4️⃣ Origem e destino
    if (!validate_origin_destination(fields[7], fields[8])) {
        return 0;
    }

    // 5️⃣ Ordem temporal básica (planeada)
    if (!validate_flight_time_order_str(fields[1], fields[3])) {
        return 0;
    }

    // 6️⃣ Estado CANCELLED → deve ter N/A
    if (status == CANCELLED) {
        if (strcmp(fields[2], "N/A") != 0) {
            return 0;
        }
        if (strcmp(fields[4], "N/A") != 0) {
            return 0;
        }
    }

    // 7️⃣ Estado DELAYED → coerência temporal
    if (!validate_delayed_status_str(status, fields[1], fields[3], fields[2], fields[4])) {
        return 0;
    }

    return 1; 
}


/*int verify_reservation(char **fields) {
    if (!fields)
        return 0;

    // 1️⃣ Campos obrigatórios (7 esperados)
    for (int i = 0; i < 7; i++) {
        if (!fields[i] || !*fields[i])
            return 0;
    }

    // 2️⃣ Reservation ID — formato sintático
    if (!validate_reservation_id(fields[0]))
        return 0;

    // 4️⃣ Document number — formato básico (ex: letras+números)
    if (!validate_document_number(fields[2]))
        return 0;



    return 1; // ✅ Tudo sintaticamente válido
}*/

int verify_reservation(char **fields) {
    if (!fields) return 0;

    // 1️⃣ Verificar se as colunas essenciais existem e não estão vazias
    for (int i = 0; i < 3; i++) {
        if (!fields[i] || !*fields[i]) return 0;
    }

    // 2️⃣ Validar ID da Reserva (R + 9 dígitos) [cite: 396]
    if (!validate_reservation_id(fields[0])) return 0;

    // 3️⃣ Validar Formato de Lista nos flight_ids 
    // A string deve começar com '[' e terminar com ']'
    size_t len = strlen(fields[1]);
    if (fields[1][0] != '[' || fields[1][len-1] != ']') return 0;

    // 4️⃣ Validar Document Number (9 dígitos) [cite: 399]
    if (!validate_document_number(fields[2])) return 0;

    return 1; 
}