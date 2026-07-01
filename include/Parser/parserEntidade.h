#ifndef PARSERENTIDADE_H
#define PARSERENTIDADE_H

#include <stdio.h>
#include "Catalogos/passengers_c.h"
#include "Catalogos/airports_c.h"
#include "Catalogos/aircrafts_c.h"
#include "Entidades/flight.h"
#include "Catalogos/manager_c.h"

/**
 * @file parserentidade.h
 * @brief Módulo que processa e valida entidades antes de as inserir nos catálogos.
 *
 * Cada função deste módulo recebe os campos já separados de uma linha do CSV
 * e trata da validação, construção da entidade e inserção no catálogo
 * correspondente. Caso a linha seja inválida, esta é escrita num ficheiro
 * de erros, conforme o comportamento exigido pelo enunciado.
 */


/* ============================================================
 * Processamento de Passageiros
 * ============================================================ */

/**
 * @brief Processa uma linha do ficheiro passengers.csv.
 *
 * Esta função:
 *  - valida todos os campos;
 *  - constrói a entidade PASSENGER;
 *  - insere-a no catálogo caso seja válida;
 *  - regista a linha em error_f caso seja inválida.
 *
 * @param fields Lista de campos já separados.
 * @param catalog Catálogo de passageiros.
 * @param error_f Ficheiro de erros.
 * @return 0 se válida, 1 se inválida.
 */
int process_passenger(char **fields, PASSENGERS_C catalog, FILE *error_f);


/* ============================================================
 * Processamento de Aeronaves
 * ============================================================ */

/**
 * @brief Processa uma linha do ficheiro aircrafts.csv.
 *
 * Efetua validações próprias de aeronaves e insere no catálogo.
 *
 * @param fields Campos da linha.
 * @param catalog Catálogo de aeronaves.
 * @param error_f Ficheiro de erros.
 * @return 0 se válida, 1 se inválida.
 */
int process_aircraft(char **fields, AIRCRAFTS_C catalog, FILE *error_f);


/* ============================================================
 * Processamento de Aeroportos
 * ============================================================ */

/**
 * @brief Processa uma linha do ficheiro airports.csv.
 *
 * Verifica formato do código IATA, país, cidade, tipo, etc.
 *
 * @param fields Campos da linha.
 * @param catalog Catálogo de aeroportos.
 * @param error_f Ficheiro de erros.
 * @return 0 se válida, 1 se inválida.
 */
int process_airport(char **fields, AIRPORTS_C catalog, FILE *error_f);


/* ============================================================
 * Processamento de Voos
 * ============================================================ */

/**
 * @brief Processa uma linha do ficheiro flights.csv.
 *
 * Valida horários, origem, destino, estado do voo e aeronave,
 * constrói a entidade FLIGHT e insere-a nos catálogos (voos +
 * contadores e rankings no aircrafts_c).
 *
 * @param fields Campos da linha.
 * @param manager_catalog Manager que contém todos os catálogos.
 * @param error_f Ficheiro de erros.
 * @return 0 se válida, 1 se inválida.
 */
int process_flight(char **fields, MANAGER_C manager_catalog, FILE *error_f);


/* ============================================================
 * Processamento de Reservas
 * ============================================================ */

/**
 * @brief Processa uma linha do ficheiro reservations.csv.
 *
 * Valida:
 *  - formato da lista de voos;
 *  - existência dos voos referidos;
 *  - encadeamento dos voos (connected flights);
 *  - existência do passageiro;
 *
 * Constrói a entidade RESERVATION e insere-a no catálogo.
 *
 * @param fields Campos da reserva.
 * @param manager_catalog Manager com catálogos de voos e passageiros.
 * @param error_f Ficheiro de erros.
 * @return 0 se válida, 1 se inválida.
 */
int process_reservation(char **fields, MANAGER_C manager_catalog, FILE *error_f);


/* ============================================================
 * Funções de validação específicas de reservas
 * ============================================================ */

/**
 * @brief Verifica se todos os voos numa lista pertencem ao catálogo.
 *
 * @param flights Lista de códigos de voo.
 * @param flights_catalog Catálogo de voos.
 * @return 1 se válido, 0 se inválido.
 */
int validate_reservation_flights_list(GList *flights, FLIGHTS_C flights_catalog);


/**
 * @brief Verifica se uma lista de voos representa voos conectados.
 *
 * Critérios:
 *  - destino do voo anterior = origem do voo seguinte;
 *  - horários compatíveis (voo seguinte parte depois do anterior chegar).
 *
 * @param flights Lista de voos.
 * @param flights_catalog Catálogo de voos.
 * @return 1 se válido, 0 se inválido.
 */
int validate_connected_flights_list(GList *flights, FLIGHTS_C flights_catalog);


/**
 * @brief Verifica se o documento referido na reserva existe no catálogo de passageiros.
 *
 * @param document_number Número de documento.
 * @param passengers_catalog Catálogo de passageiros.
 * @return 1 se válido, 0 se inválido.
 */
int validate_reservation_document(const gchar *document_number, PASSENGERS_C passengers_catalog);

#endif
