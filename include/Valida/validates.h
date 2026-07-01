#ifndef VALIDATES_H
#define VALIDATES_H

#include <glib.h>
#include "Entidades/flight.h"
#include "Catalogos/aircrafts_c.h"
#include "Catalogos/flights_c.h"
#include "Entidades/reservation.h"
#include "Catalogos/passengers_c.h"

/**
 * @file validates.h
 * @brief Módulo responsável pela validação sintática e lógica de campos individuais.
 *
 * Este módulo contém funções dedicadas à verificação da correção dos campos
 * presentes nas entidades (voos, passageiros, reservas, aeroportos, aeronaves).
 *
 * As funções aqui presentes validam:
 *  - formatos de data e hora,
 *  - integridade de identificadores,
 *  - coerência entre horas e estados de voo,
 *  - formatos textuais (email, género, códigos),
 *  - valores numéricos (latitudes, longitudes),
 *  - regras internas do domínio aeronáutico.
 *
 * Estas funções são usadas pelos módulos de parsing e validação de entidades.
 */


/* ============================================================
 * Validação de datas
 * ============================================================ */

/**
 * @brief Valida uma data no formato YYYY-MM-DD.
 *
 * Verifica formato, tamanho e limites dos valores.
 *
 * @param str String da data.
 * @return 1 se válido, 0 se inválido.
 */
int validate_date(const gchar *str);

/**
 * @brief Valida um ano isolado.
 *
 * Aceita apenas valores numéricos com quatro dígitos.
 *
 * @param string String contendo um ano.
 * @return 1 se válido, 0 se inválido.
 */
int validate_year(const char *string);

/**
 * @brief Valida uma data com hora no formato YYYY-MM-DD HH:MM:SS.
 *
 * Verifica formato, campos numéricos e intervalos aceitáveis.
 *
 * @param str String da data-hora.
 * @return 1 se válido, 0 se inválido.
 */
int validate_datetime(const gchar *str);


/* ============================================================
 * Validação de voos
 * ============================================================ */

/**
 * @brief Valida o identificador de um voo.
 *
 * Ex.: "TP1234", "LH98".
 *
 * @param str ID do voo.
 * @return 1 se válido, 0 se inválido.
 */
int validate_flight_id(const gchar *str);

/**
 * @brief Verifica se a origem e o destino são diferentes.
 *
 * @param origin Código IATA de origem.
 * @param destination Código IATA de destino.
 * @return 1 se válido, 0 se inválido.
 */
int validate_origin_destination(const gchar *origin, const gchar *destination);

/**
 * @brief Verifica coerência temporal entre partida e chegada.
 *
 * @param departure Data/hora de partida prevista.
 * @param arrival Data/hora de chegada prevista.
 * @return 1 se válido, 0 se inválido.
 */
int validate_flight_time_order_str(const gchar *departure, const gchar *arrival);

/**
 * @brief Valida coerência de um voo cancelado.
 *
 * Um voo CANCELLED **não pode** conter actual_departure ou actual_arrival.
 *
 * @param status Estado do voo.
 * @param actual_departure Partida real.
 * @param actual_arrival Chegada real.
 * @return 1 se válido, 0 se inválido.
 */
int validate_cancelled_status(FlightStatus status,
                              const gchar *actual_departure,
                              const gchar *actual_arrival);

/**
 * @brief Valida coerência de um voo com estado DELAYED.
 *
 * - Partida real deve ser posterior à prevista
 * - Chegada real deve ser posterior à prevista
 * (quando aplicável)
 *
 * @param status Estado do voo.
 * @param dep Partida prevista.
 * @param arr Chegada prevista.
 * @param act_dep Partida real.
 * @param act_arr Chegada real.
 * @return 1 se válido, 0 se inválido.
 */
int validate_delayed_status_str(FlightStatus status,
                                const gchar *dep, const gchar *arr,
                                const gchar *act_dep, const gchar *act_arr);


/* ============================================================
 * Validação textual
 * ============================================================ */

/**
 * @brief Valida o formato de um email.
 *
 * Regras mínimas:
 *  - contém '@'
 *  - contém '.'
 *  - TLD com pelo menos 2 caracteres
 *
 * @param email String do email.
 * @return 1 se válido, 0 se inválido.
 */
int validate_email(const gchar *email);

/**
 * @brief Valida o número de documento de um passageiro.
 *
 * Aceita apenas caracteres alfanuméricos.
 *
 * @param doc Número do documento.
 * @return 1 se válido, 0 se inválido.
 */
int validate_document_number(const gchar *doc);

/**
 * @brief Valida o género de um passageiro.
 *
 * Normalmente aceita "M", "F" ou strings equivalentes.
 *
 * @param gender String do género.
 * @return 1 se válido, 0 se inválido.
 */
int validate_gender(const gchar *gender);

/**
 * @brief Valida o ID de uma reserva.
 *
 * Ex.: "R12345"
 *
 * @param id Identificador da reserva.
 * @return 1 se válido, 0 se inválido.
 */
int validate_reservation_id(const gchar *id);


/* ============================================================
 * Validação de aeroportos e coordenadas
 * ============================================================ */

/**
 * @brief Valida o tipo textual de aeroporto.
 *
 * Aceita valores do domínio definido:
 *  - small_airport, medium_airport, large_airport, heliport, seaplane_base
 *
 * @param type String do tipo.
 * @return 1 se válido, 0 se inválido.
 */
int validate_airport_type(const gchar *type);

/**
 * @brief Verifica se uma coordenada está num formato válido.
 *
 * Aceita números reais, positivos ou negativos.
 *
 * @param str String representando a coordenada.
 * @return 1 se válido, 0 se inválido.
 */
int is_valid_coordinate_format(const gchar *str);

/**
 * @brief Valida a latitude.
 *
 * Intervalo permitido: [-90, 90].
 *
 * @param lat_str Latitude.
 * @return 1 se válido, 0 se inválido.
 */
int validate_latitude(const gchar *lat_str);

/**
 * @brief Valida a longitude.
 *
 * Intervalo permitido: [-180, 180].
 *
 * @param lon_str Longitude.
 * @return 1 se válido, 0 se inválido.
 */
int validate_longitude(const gchar *lon_str);

/**
 * @brief Valida um código IATA (3 letras).
 *
 * @param code Código do aeroporto.
 * @return 1 se válido, 0 se inválido.
 */
int validate_airport_code(const gchar *code);

#endif
