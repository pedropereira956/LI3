#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>
#include "Utils/utils.h"
#include "Valida/validates.h"
#include "Entidades/flight.h"
#include "Catalogos/aircrafts_c.h"
#include "Catalogos/passengers_c.h"
#include "Entidades/passenger.h"
#include "Catalogos/flights_c.h"
#include "Entidades/reservation.h"
/**
 * Valida se uma string representa uma data válida no formato "aaaa-mm-dd".
 */
int validate_date(const gchar *str) {
    if (!str || strlen(str) != 10)
        return 0;

    // formato esperado: YYYY-MM-DD
    if (str[4] != '-' || str[7] != '-')
        return 0;

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit((unsigned char)str[i]))
            return 0;
    }

    gint month = atoi(str + 5);
    gint day = atoi(str + 8);

    if (month < 1 || month > 12)
        return 0;

    if (day < 1 || day > 31)
        return 0;

    return 1;
}

int validate_year(const char *string) {
    if (!string || *string == '\0')
        return 0;

    // O ano deve ter exatamente 4 dígitos
    if (strlen(string) != 4)
        return 0;

    // Todos os caracteres têm de ser dígitos
    for (int i = 0; i < 4; i++) {
        if (!isdigit((unsigned char)string[i]))
            return 0;
    }

    int year = atoi(string);

    // Validação de intervalo razoável
    if (year < 1900 || year > 2100)
        return 0;

    return 1;
}

/**
 * Valida se uma string representa uma data e hora válidas no formato "aaaa-mm-dd hh:mm".
 */
int validate_datetime(const gchar *str) {
    if (!str || strlen(str) != 16)
        return 0;

    // formato esperado: YYYY-MM-DD hh:mm
    if (str[4] != '-' || str[7] != '-' || str[10] != ' ' || str[13] != ':')
        return 0;

    for (int i = 0; i < 16; i++) {
        if (i == 4 || i == 7 || i == 10 || i == 13) continue;
        if (!isdigit((unsigned char)str[i]))
            return 0;
    }

    gint year = atoi(str);
    gint month = atoi(str + 5);
    gint day = atoi(str + 8);
    gint hour = atoi(str + 11);
    gint minute = atoi(str + 14);

    if (year < 1900)
        return 0;

    if (month < 1 || month > 12)
        return 0;

    if (day < 1 || day > 31)
        return 0;

    if (hour < 0 || hour > 23)
        return 0;

    if (minute < 0 || minute > 59)
        return 0;

    return 1;
}

/**
 * Valida o identificador de voo (formato: ccddddd).
 */

int validate_flight_id(const gchar *str) {
    if (!str) return 0;

    // FIX: O dataset da Fase 2 tem IDs com 8 caracteres (ex: RB197746).
    // O enunciado diz 7, mas o dataset manda. Aceitamos entre 1 a 10 para segurança,
    // ou especificamente 7 e 8. Vamos aceitar se tiver tamanho razoável.
    size_t len = strlen(str);
    if (len < 1) return 0; 
    
    // Verificamos se não tem espaços
    if (strchr(str, ' ') != NULL) return 0;

    // Verifica se os dois primeiros são letras maiúsculas
    if (!isupper((unsigned char)str[0]) || !isupper((unsigned char)str[1])) {
        return 0;
    }

    // Verifica se o resto são apenas dígitos
    // Começa no índice 2 e vai até ao fim da string (seja ela tam 7, 8 ou 9)
    for (size_t i = 2; i < len; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return 0;
        }
    }

    return 1;
}
/**
 * Verifica se origem e destino são diferentes (e válidos).
 */
int validate_origin_destination(const gchar *origin, const gchar *destination) {
    if (!origin || !destination) return 0;
    if (strlen(origin) != 3 || strlen(destination) != 3) return 0;
    if (strcmp(origin, destination) == 0) return 0;
    return 1;
}



/**
 * arrival não pode ser antes do departure
 */
int validate_flight_time_order_str(const gchar *departure, const gchar *arrival) {
    if (!departure || !arrival) return 0;
    return strcmp(arrival, departure) >= 0;
}

// se o voa foi cancea«lado nao deve haver departure nem arrival 
int validate_cancelled_status(FlightStatus status, const gchar *actual_departure, const gchar *actual_arrival) {
    if (status == CANCELLED) {
        if (!actual_departure || !actual_arrival) return 0;
        if (g_strcmp0(actual_departure, "N/A") != 0) return 0;
        if (g_strcmp0(actual_arrival, "N/A") != 0) return 0;
    }
    return 1;
}

//valida os horarios dos voos atrasados
int validate_delayed_status_str(FlightStatus status, const gchar *dep, const gchar *arr,const gchar *act_dep, const gchar *act_arr){
    if (status != DELAYED) return 1;

    if (!dep || !arr || !act_dep || !act_arr) return 0;

    // Basta garantir que as “actual” não são menores que as planeadas
    if (strcmp(act_dep, dep) < 0) return 0;
    if (strcmp(act_arr, arr) < 0) return 0;

    return 1;
}

//funçao que valida o email de um user

int validate_email(const gchar *email) {
    if (!email) return 0;

    const gchar *at = strchr(email, '@');
    if (!at) return 0; // tem de ter @
    if (at == email) return 0; // não pode começar com @
    if (strchr(at + 1, '@')) return 0; // não pode ter dois @

    // username (parte antes do @)
    for (const gchar *p = email; p < at; p++) {
        if (!isalnum((unsigned char)*p) && *p != '.')
            return 0;
    }

    // não pode começar ou terminar com ponto
    if (email[0] == '.' || *(at - 1) == '.')
        return 0;

    // domínio (parte depois do @)
    const gchar *dot = strrchr(at + 1, '.');
    if (!dot) return 0;
    if (dot == at + 1) return 0; // nada entre @ e .
    if (*(dot + 1) == '\0') return 0; // nada depois do ponto

    // lstring (entre @ e .)
    for (const gchar *p = at + 1; p < dot; p++) {
        if (!islower((unsigned char)*p))
            return 0;
    }

    // rstring (depois do ponto)
    gint rlen = strlen(dot + 1);
    if (rlen < 2 || rlen > 3) return 0;
    for (const gchar *p = dot + 1; *p; p++) {
        if (!islower((unsigned char)*p))
            return 0;
    }

    return 1; // tudo certo
}


/*valida se o document number tem 9 numeros
int validate_document_number(const gchar *doc) {
    if (!doc) return 0;

    gint len = strlen(doc);
    if (len != 9) return 0;

    for (gint i = 0; i < len; i++) {
        if (!isdigit((unsigned char)doc[i]))
            return 0;
    }

    return 1;
}
*/
int validate_document_number(const gchar *doc) {
    if (!doc) return 0;

    const char *p = doc;
    if (*p == '"') p++; // Pula aspa se existir

    size_t len = 0;
    while(p[len] && p[len] != '"') { // Conta até ao fim ou até à aspa de fecho
        if (!isdigit((unsigned char)p[len])) return 0;
        len++;
    }

    return (len == 9); // Tem de ter 9 dígitos
}
//valida o gender do user
int validate_gender(const gchar *gender) {
    if (!gender) return 0;          // null
    if (strlen(gender) != 1) return 0;  // só pode ter 1 caractere

    gchar g = gender[0];
    return (g == 'M' || g == 'F' || g == 'O');
}

/*valida o id da reserva
int validate_reservation_id(const gchar *id) {
    if (!id) return 0;
    if (strlen(id) != 10) return 0; // 'R' + 9 dígitos

    if (id[0] != 'R') return 0;

    for (int i = 1; i < 10; i++) {
        if (!g_ascii_isdigit(id[i]))
            return 0;
    }

    return 1;
}
    */

int validate_reservation_id(const gchar *id) {
    if (!id) return 0;
    
    // Se vier com aspas, ignoramos os limites para a validação
    const char *p = id;
    if (*p == '"') p++; // Pula aspa inicial
    
    if (p[0] != 'R') return 0;
    
    // Verifica se os próximos 9 são dígitos
    for (int i = 1; i < 10; i++) {
        if (!isdigit((unsigned char)p[i])) return 0;
    }
    return 1;
}

// Valida se o document number de uma reserva corresponde a um passageiro existente,o gchar passado é o document nunumber que esta na reserva

// int validate_reservation_document(const gchar *document_number, PASSENGERS_C passengers_catalog) {
//     if (!document_number || !passengers_catalog)
//         return 0;

//     // Primeiro valida formato do número do documento
//     if (!validate_document_number(document_number))
//         return 0;

//     // Acede à tabela hash de passageiros
//     GHashTable *passengers_table = get_hash_table_passenger(passengers_catalog);
//     if (!passengers_table)
//         return 0;

//     // Verifica se o passageiro existe
//     gpointer found = g_hash_table_lookup(passengers_table, document_number);
//     return (found != NULL);
// }

// valida se os flight_ids existentes na reserva sao mesmo voos que existem
// int validate_reservation_flights(const gchar *flight_ids_str, FLIGHTS_C flights_catalog) {
//     if (!flight_ids_str || !flights_catalog)
//         return 0;

//     // Cria lista temporária de flight_ids
//     GList *flights = parse_flight_ids_to_list(flight_ids_str);
//     if (!flights)
//         return 0;

//     guint count = g_list_length(flights);
//     if (count < 1 || count > 2) {
//         g_list_free_full(flights, g_free);
//         return 0;
//     }

//     GHashTable *flights_table = get_hash_table_flight(flights_catalog);
//     if (!flights_table) {
//         g_list_free_full(flights, g_free);
//         return 0;
//     }

//     // Validar que todos os voos existem
//     for (GList *l = flights; l != NULL; l = l->next) {
//         gchar *flight_id = (gchar *)l->data;
//         if (!flight_id || strlen(flight_id) == 0 || !g_hash_table_contains(flights_table, flight_id)) {
//             g_list_free_full(flights, g_free);
//             return 0;
//         }
//     }

//     g_list_free_full(flights, g_free);
//     return 1;
// }

// //verifica se quando existem 2 voos, o destino do primeiro é igual à origem do segundo
// int validate_connected_flights(const gchar *flight_ids_str, FLIGHTS_C flights_catalog) {
//     if (!flight_ids_str || !flights_catalog)
//         return 0;

//     // Obter lista temporária de IDs
//     GList *flight_ids = parse_flight_ids_to_list(flight_ids_str);
//     if (!flight_ids) 
//         return 0;

//     guint count = g_list_length(flight_ids);
//     if (count != 2) {
//         g_list_free_full(flight_ids, g_free);
//         return 1; // válido se houver 1 voo ou 0 (não há conexão a validar)
//     }

//     gchar *first_id  = (gchar *)flight_ids->data;
//     gchar *second_id = (gchar *)flight_ids->next->data;

//     if (!first_id || !second_id) {
//         g_list_free_full(flight_ids, g_free);
//         return 0;
//     }

//     // vamos ao catalogo buscar os voos para comparar
//     FLIGHT first_flight  = get_flight_by_id(flights_catalog, first_id);
//     FLIGHT second_flight = get_flight_by_id(flights_catalog, second_id);

//     if (!first_flight || !second_flight) {
//         g_list_free_full(flight_ids, g_free);
//         return 0;
//     }

//     const gchar *dest1 = get_flight_destination(first_flight);
//     const gchar *orig2 = get_flight_origin(second_flight);

//     gboolean valid = (dest1 && orig2 && g_strcmp0(dest1, orig2) == 0);

//     g_list_free_full(flight_ids, g_free);
//     return valid;
// }


//valida o tipo de aeroporto
int validate_airport_type(const gchar *type) {
    if (!type) return 0;

    if (g_ascii_strcasecmp(type, "small_airport") == 0) return 1;
    if (g_ascii_strcasecmp(type, "medium_airport") == 0) return 1;
    if (g_ascii_strcasecmp(type, "large_airport") == 0) return 1;
    if (g_ascii_strcasecmp(type, "heliport") == 0) return 1;
    if (g_ascii_strcasecmp(type, "seaplane_base") == 0) return 1;

    return 0;
}


/**
 * Verifica se uma string representa um número decimal válido com formato [-]dd.dddddddd
 * (auxiliar interna).
 */
int is_valid_coordinate_format(const gchar *str) {
    if (!str || *str == '\0') return 0; // apenas verificar se não é vazio

    int i = 0;
    if (str[i] == '-') i++; // sinal opcional

    int digits_before_dot = 0;
    int digits_after_dot = 0;
    gboolean found_dot = FALSE;

    for (; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            if (found_dot) return 0; // só pode haver um ponto
            found_dot = TRUE;
            continue;
        }
        if (!g_ascii_isdigit(str[i])) return 0;

        if (!found_dot) digits_before_dot++;
        else digits_after_dot++;

        if (digits_after_dot > 8) return 0; // máximo 8 casas decimais
    }

    // tem de haver ponto, e pelo menos 1 dígito antes e depois
    if (!found_dot || digits_before_dot < 1 || digits_after_dot < 1)
        return 0;

    return 1;
}

/**
 * Valida latitude: formato e intervalo (-90 a 90)
 */
int validate_latitude(const gchar *lat_str) {
    if (!is_valid_coordinate_format(lat_str)) return 0;

    gdouble lat = g_ascii_strtod(lat_str, NULL);
    if (lat < -90.0 || lat > 90.0)
        return 0;

    return 1;
}

/**
 * Valida longitude: formato e intervalo (-180 a 180)
 */
int validate_longitude(const gchar *lon_str) {
    if (!is_valid_coordinate_format(lon_str)) return 0;

    gdouble lon = g_ascii_strtod(lon_str, NULL);
    if (lon < -180.0 || lon > 180.0)
        return 0;

    return 1;
}


//codigo aeroporto tem que ter 3 caracteres
int validate_airport_code(const gchar *code) {
    if (!code || strlen(code) != 3)
        return 0;

    for (int i = 0; i < 3; i++) {
        if (!g_ascii_isupper(code[i])) // deve ser [A-Z]
            return 0;
    }

    return 1;
}