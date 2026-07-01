/**
 * @file airport.h
 * @brief Definição da estrutura e funções associadas a um aeroporto.
 *
 * Este módulo define a entidade AIRPORT, responsável por armazenar
 * informação relativa a aeroportos, incluindo código IATA, nome,
 * localização, tipo e estatísticas de passageiros.
 */

#ifndef AIRPORT_H
#define AIRPORT_H

#include <glib.h>

/* ============================
 * Enum Tipo de Aeroporto
 * ============================ */

/**
 * @brief Enumeração que representa o tipo de aeroporto.
 *
 * A classificação é derivada diretamente do dataset, permitindo distinguir
 * diferentes infraestruturas aeroportuárias.
 */
typedef enum airport_type {
    SMALL_AIRPORT,     /**< Aeroporto pequeno */
    MEDIUM_AIRPORT,    /**< Aeroporto médio */
    LARGE_AIRPORT,     /**< Aeroporto grande */
    HELIPORT,          /**< Heliporto */
    SEAPLANE_BASE      /**< Base para hidroaviões */
} AirportType;

/* ============================
 * Estrutura Opaca
 * ============================ */

/**
 * @brief Estrutura opaca que representa um aeroporto.
 *
 * O conteúdo interno é ocultado para garantir encapsulamento.
 */
typedef struct airport *AIRPORT;

/* ============================
 * Funções Base
 * ============================ */

/**
 * @brief Cria um novo aeroporto com campos inicializados a NULL/Zero.
 *
 * @return Ponteiro AIRPORT criado dinamicamente.
 */
AIRPORT create_airport(void);

/**
 * @brief Liberta toda a memória associada a um aeroporto.
 *
 * Compatível com destrutores utilizados em GHashTables.
 *
 * @param data Ponteiro genérico para o aeroporto a libertar.
 */
void free_airport(void *data);

/* ============================
 * Setters
 * ============================ */

/**
 * @brief Define o código IATA do aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 * @param code Código IATA (3 letras).
 */
void set_airport_code(AIRPORT a, const gchar *code);

/**
 * @brief Define o nome oficial do aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 * @param name Nome do aeroporto.
 */
void set_airport_name(AIRPORT a, const gchar *name);

/**
 * @brief Define a cidade onde o aeroporto se encontra.
 *
 * @param a Ponteiro para o aeroporto.
 * @param city Nome da cidade.
 */
void set_airport_city(AIRPORT a, const gchar *city);

/**
 * @brief Define o país onde o aeroporto se localiza.
 *
 * @param a Ponteiro para o aeroporto.
 * @param country Nome do país.
 */
void set_airport_country(AIRPORT a, const gchar *country);

/**
 * @brief Define o tipo de aeroporto a partir de uma string do dataset.
 *
 * A função transforma a string em enum AirportType.
 *
 * @param a Ponteiro para o aeroporto.
 * @param type_str String do tipo (ex.: "large_airport").
 */
void set_airport_type(AIRPORT a, const gchar *type_str);

/* ============================
 * Estatísticas (Q1)
 * ============================ */

/**
 * @brief Incrementa o número total de passageiros que chegaram a este aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 */
void inc_airport_arrivals(AIRPORT a);

/**
 * @brief Incrementa o número total de passageiros que partiram deste aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 */
void inc_airport_departures(AIRPORT a);

/* ============================
 * Getters
 * ============================ */

/**
 * @brief Obtém o código IATA do aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 * @return String constante com o código.
 */
const gchar* get_airport_code(const AIRPORT a);

/**
 * @brief Obtém o nome oficial do aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 * @return String constante com o nome.
 */
const gchar* get_airport_name(const AIRPORT a);

/**
 * @brief Obtém a cidade associada ao aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 * @return String com a cidade.
 */
const gchar* get_airport_city(const AIRPORT a);

/**
 * @brief Obtém o país onde o aeroporto está localizado.
 *
 * @param a Ponteiro para o aeroporto.
 * @return String com o país.
 */
const gchar* get_airport_country(const AIRPORT a);

/**
 * @brief Obtém o tipo de aeroporto (Enum).
 *
 * @param a Ponteiro para o aeroporto.
 * @return Um valor do enum AirportType.
 */
AirportType get_airport_type(const AIRPORT a);

/**
 * @brief Obtém o número total de passageiros que chegaram (Chegadas).
 *
 * @param a Ponteiro para o aeroporto.
 * @return Número de passageiros (inteiro).
 */
int get_airport_total_arrivals(const AIRPORT a);

/**
 * @brief Obtém o número total de passageiros que partiram (Partidas).
 *
 * @param a Ponteiro para o aeroporto.
 * @return Número de passageiros (inteiro).
 */
int get_airport_total_departures(const AIRPORT a);

/**
 * @brief Converte o tipo do aeroporto numa string legível (ex: "large_airport").
 *
 * Importante para o output da Query 1.
 *
 * @param a Ponteiro para o aeroporto.
 * @return String alocada (deve ser libertada com g_free) correspondente ao tipo.
 */
char *get_airport_type_string(const AIRPORT a);

/* ============================
 * Util
 * ============================ */

/**
 * @brief Constrói um aeroporto a partir de um array de campos.
 *
 * Usado no parser para criar a entidade a partir do ficheiro CSV.
 *
 * @param fields Array de strings contendo os atributos do aeroporto.
 * @return AIRPORT inicializado com os valores fornecidos.
 */
AIRPORT build_airport(char **fields);

/**
 * @brief Imprime no stdout todos os campos do aeroporto.
 *
 * Útil para debugging e verificação dos dados carregados.
 *
 * @param a Ponteiro para o aeroporto.
 */
void print_airport(AIRPORT a);

#endif