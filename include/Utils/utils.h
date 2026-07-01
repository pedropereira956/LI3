#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <glib.h>
#include "Entidades/flight.h"
#include "Entidades/airport.h"
#include "Catalogos/manager_c.h"

/**
 * @file utils.h
 * @brief Conjunto de funções auxiliares usadas em vários módulos do sistema.
 *
 * Este módulo contém utilitários gerais, incluindo:
 *  - carregamento inicial dos catálogos;
 *  - manipulação de strings;
 *  - interpretação de enums (FlightStatus, AirportType);
 *  - conversão de listas de voos.
 *
 * Estas funções promovem a reutilização e evitam duplicação de código.
 */


/* ============================================================
 * Carregamento de dados
 * ============================================================ */

/**
 * @brief Faz o parsing de todos os ficheiros CSV do dataset.
 *
 * Esta função:
 *  - abre os ficheiros passengers, flights, airports, aircrafts e reservations;
 *  - chama os módulos de parsing específicos para cada tipo de entidade;
 *  - cria automaticamente os ficheiros *_errors.csv previstos no enunciado;
 *  - insere entidades válidas nos respetivos catálogos.
 *
 * @param manager_catalog Estrutura principal com todos os catálogos.
 * @param path1 Caminho para a pasta dos datasets.
 * @return 0 se sucesso, 1 em caso de falha.
 */
int set_catalogs(MANAGER_C manager_catalog, char* path1);


/* ============================================================
 * Manipulação de strings
 * ============================================================ */

/**
 * @brief Concatena duas strings numa nova string alocada dinamicamente.
 *
 * @param s1 Primeira string.
 * @param s2 Segunda string.
 * @return Nova string resultante de s1 + s2 (o utilizador deve libertá-la).
 */
char* concat(char *s1, char *s2);

/**
 * @brief Remove aspas iniciais e finais de uma string, caso existam.
 *
 * Exemplo: `"LIS"` → `LIS`
 *
 * @param s String a ser modificada.
 */
void trim_quotes(char *s);


/* ============================================================
 * Parsers de enums
 * ============================================================ */

/**
 * @brief Converte uma string num valor FlightStatus.
 *
 * Strings suportadas:
 *  - "on_time"     → ON_TIME
 *  - "delayed"     → DELAYED
 *  - "cancelled"   → CANCELLED
 *
 * @param str String com o estado.
 * @return Valor correspondente da enumeração FlightStatus.
 */
FlightStatus parse_flight_status(const gchar *str);

/**
 * @brief Converte uma string num valor AirportType.
 *
 * Strings suportadas incluem:
 *  - "small_airport"
 *  - "medium_airport"
 *  - "large_airport"
 *  - "heliport"
 *  - "seaplane_base"
 *
 * @param str String com o tipo do aeroporto.
 * @return Valor correspondente da enumeração AirportType.
 */
AirportType parse_airport_type(const gchar *str);


/* ============================================================
 * Conversão de listas
 * ============================================================ */

/**
 * @brief Converte uma string de IDs de voos numa GList.
 *
 * Exemplo:
 *    "AB123,CD456,EF789"
 * converte para uma lista com três strings já duplicadas.
 *
 * A lista devolvida deve ser libertada após o uso.
 *
 * @param flight_ids_str String contendo IDs de voos separados por vírgulas.
 * @return Lista ligada de strings, ou NULL se vazia/inválida.
 */
GList* parse_flight_ids_to_list(const gchar *flight_ids_str);

/**
 * @brief Calcula a diferença em minutos entre duas datas/horas.
 * * @param departure Data estimada (YYYY-MM-DD hh:mm:ss).
 * @param actual_departure Data real (YYYY-MM-DD hh:mm:ss).
 * @return Diferença em minutos (actual - estimado).
 */
int calculate_delay_minutes(const char *departure, const char *actual_departure);

#endif
