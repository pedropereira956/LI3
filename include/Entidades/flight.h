/**
 * @file flight.h
 * @brief Interface da entidade FLIGHT e funções associadas.
 *
 * Este módulo define a estrutura opaca FLIGHT, responsável por armazenar
 * toda a informação referente a um voo: horários previstos e reais,
 * origem, destino, aeronave utilizada e estado do voo. A implementação
 * interna é encapsulada, garantindo segurança e integridade dos dados.
 */

#ifndef FLIGHT_H
#define FLIGHT_H

#include <glib.h>

/* ======================
   ENUMS E ESTRUTURAS
   ====================== */

/**
 * @brief Enum que representa o estado de um voo.
 *
 * Os estados possíveis são:
 *  - ON_TIME: voo dentro do horário previsto
 *  - DELAYED: voo atrasado
 *  - CANCELLED: voo cancelado
 */
typedef enum {
    ON_TIME,
    DELAYED,
    CANCELLED
} FlightStatus;

/**
 * @brief Estrutura opaca que representa um voo.
 *
 * Os detalhes internos são ocultados para reforçar o encapsulamento.
 */
typedef struct flight *FLIGHT;

/* ======================
   CONSTRUTOR / DESTRUTOR
   ====================== */

/**
 * @brief Cria um voo vazio, inicializando todos os campos a NULL.
 *
 * @return Ponteiro para um objeto FLIGHT criado dinamicamente.
 */
FLIGHT create_flight(void);

/**
 * @brief Liberta toda a memória associada a um voo.
 *
 * Compatível com funções destrutoras usadas em GHashTables.
 *
 * @param data Ponteiro genérico para o voo a destruir.
 */
void free_flight(void *data);

/* ======================
   SETTERS
   ====================== */

/**
 * @brief Define o identificador único do voo.
 *
 * @param f Ponteiro para o voo.
 * @param id Código identificador do voo.
 */
void set_flight_id(FLIGHT f, const gchar *id);

/**
 * @brief Define a data/hora prevista de partida.
 *
 * @param f Ponteiro para o voo.
 * @param dep String da data prevista (YYYY/MM/DD hh:mm:ss).
 */
void set_flight_departure(FLIGHT f, const gchar *dep);

/**
 * @brief Define a data/hora real de partida.
 *
 * @param f Ponteiro para o voo.
 * @param act_dep String da data real.
 */
void set_flight_actual_departure(FLIGHT f, const gchar *act_dep);

/**
 * @brief Define a data/hora prevista de chegada.
 *
 * @param f Ponteiro para o voo.
 * @param arr String da data prevista.
 */
void set_flight_arrival(FLIGHT f, const gchar *arr);

/**
 * @brief Define a data/hora real de chegada.
 *
 * @param f Ponteiro para o voo.
 * @param act_arr String da data real.
 */
void set_flight_actual_arrival(FLIGHT f, const gchar *act_arr);

/**
 * @brief Define o estado do voo (on time, delayed, cancelled).
 *
 * @param f Ponteiro para o voo.
 * @param status Estado do voo.
 */
void set_flight_status(FLIGHT f, FlightStatus status);

/**
 * @brief Define o aeroporto de origem.
 *
 * @param f Ponteiro para o voo.
 * @param origin Código IATA do aeroporto de origem.
 */
void set_flight_origin(FLIGHT f, const gchar *origin);

/**
 * @brief Define o aeroporto de destino.
 *
 * @param f Ponteiro para o voo.
 * @param destination Código IATA do aeroporto de destino.
 */
void set_flight_destination(FLIGHT f, const gchar *destination);

/**
 * @brief Define o identificador da aeronave usada no voo.
 *
 * @param f Ponteiro para o voo.
 * @param aircraft ID da aeronave.
 */
void set_flight_aircraft(FLIGHT f, const gchar *aircraft);

void set_flight_airline(FLIGHT f, const gchar *airline);

/* ======================
   GETTERS
   ====================== */

/**
 * @brief Obtém o ID do voo.
 *
 * @param f Ponteiro para o voo.
 * @return String com o identificador.
 */
const gchar *get_flight_id(const FLIGHT f);

/**
 * @brief Obtém a data/hora prevista de partida.
 */
const gchar *get_flight_departure(const FLIGHT f);

/**
 * @brief Obtém a data/hora real de partida.
 */
const gchar *get_flight_actual_departure(const FLIGHT f);

/**
 * @brief Obtém a data/hora prevista de chegada.
 */
const gchar *get_flight_arrival(const FLIGHT f);

/**
 * @brief Obtém a data/hora real de chegada.
 */
const gchar *get_flight_actual_arrival(const FLIGHT f);

/**
 * @brief Obtém o estado atual do voo.
 */
FlightStatus get_flight_status(const FLIGHT f);

/**
 * @brief Obtém o código IATA do aeroporto de origem.
 */
const gchar *get_flight_origin(const FLIGHT f);

/**
 * @brief Obtém o código IATA do aeroporto de destino.
 */
const gchar *get_flight_destination(const FLIGHT f);

/**
 * @brief Obtém o identificador da aeronave usada neste voo.
 */
const gchar *get_flight_aircraft(const FLIGHT f);

const gchar *get_flight_airline(const FLIGHT f);

/* ======================
   OUTRAS FUNÇÕES
   ====================== */

/**
 * @brief Constrói um voo a partir dos campos lidos no CSV.
 *
 * Função usada no parser para transformar os campos em entidade FLIGHT.
 *
 * @param fields Array de strings com os atributos do voo.
 * @param status Estado inicial do voo.
 * @return Objeto FLIGHT preenchido.
 */
FLIGHT build_flight(char **fields, FlightStatus status);

/**
 * @brief Imprime toda a informação do voo no stdout.
 *
 * Útil para debugging.
 *
 * @param f Ponteiro para o voo.
 */
void print_flight(FLIGHT f);

#endif // FLIGHT_H
