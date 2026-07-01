/**
 * @file reservation.h
 * @brief Interface da entidade RESERVATION e funções associadas.
 *
 * Este módulo define a estrutura opaca RESERVATION, responsável por armazenar
 * toda a informação relativa a reservas: identificador da reserva, voos
 * associados e passageiro correspondente. A implementação é encapsulada de
 * forma a garantir segurança e integridade dos dados.
 */

#ifndef RESERVATION_H
#define RESERVATION_H

#include <glib.h>

/* ================================
 *        Estrutura Opaca
 * ================================ */

/**
 * @brief Estrutura opaca que representa uma reserva.
 *
 * O conteúdo interno é ocultado para reforçar o encapsulamento e evitar
 * dependências externas indesejadas.
 */
typedef struct reservation *RESERVATION;

/* ================================
 *        Funções Base
 * ================================ */

/**
 * @brief Cria uma reserva vazia com campos inicializados a NULL.
 *
 * @return Ponteiro para um objeto RESERVATION criado dinamicamente.
 */
RESERVATION create_reservation(void);

/**
 * @brief Liberta a memória associada a uma reserva.
 *
 * Compatível com destrutores usados em GHashTables.
 *
 * @param data Ponteiro genérico para a reserva a destruir.
 */
void free_reservation(void *data);

/* ================================
 *              Setters
 * ================================ */

/**
 * @brief Define o identificador único da reserva.
 *
 * @param r Ponteiro para a reserva.
 * @param id Identificador da reserva.
 */
void set_reservation_id(RESERVATION r, const gchar *id);

/**
 * @brief Define a lista de voos associados à reserva.
 *
 * A string contém vários IDs separados por vírgula (ex.: "FL123,FL456").
 * A função converte essa string para uma lista GList de strings.
 *
 * @param r Ponteiro para a reserva.
 * @param flight_ids_str String com os IDs dos voos.
 */
void set_reservation_flight_ids(RESERVATION r, const gchar *flight_ids_str);

/**
 * @brief Define o número de documento do passageiro associado à reserva.
 *
 * @param r Ponteiro para a reserva.
 * @param document_number String com o documento.
 */
void set_reservation_document_number(RESERVATION r, const gchar *document_number);

/*
 * Setters opcionais caso pretendas ativar campos adicionais:
 *
 * void set_reservation_seat(RESERVATION r, const gchar *seat);
 * void set_reservation_price(RESERVATION r, gdouble price);
 * void set_reservation_extra_luggage(RESERVATION r, gboolean extra_luggage);
 * void set_reservation_priority_boarding(RESERVATION r, gboolean priority_boarding);
 */

/* ================================
 *              Getters
 * ================================ */

/**
 * @brief Obtém o identificador da reserva.
 *
 * @param r Ponteiro para a reserva.
 * @return String com o ID da reserva.
 */
const gchar* get_reservation_id(const RESERVATION r);

/**
 * @brief Obtém a lista de voos associados à reserva.
 *
 * @param r Ponteiro para a reserva.
 * @return GList* contendo strings com IDs dos voos.
 */
GList* get_reservation_flight_ids(const RESERVATION r);

/**
 * @brief Obtém o número de documento do passageiro.
 *
 * @param r Ponteiro para a reserva.
 * @return String com o número do documento.
 */
const gchar* get_reservation_document_number(const RESERVATION r);

/*
 * Getters opcionais:
 *
 * const gchar* get_reservation_seat(const RESERVATION r);
 * gdouble get_reservation_price(const RESERVATION r);
 * gboolean get_reservation_extra_luggage(const RESERVATION r);
 * gboolean get_reservation_priority_boarding(const RESERVATION r);
 */

/* ================================
 *               Util
 * ================================ */

/**
 * @brief Constrói uma reserva a partir dos campos de uma linha do CSV.
 *
 * Função utilizada pelo parser para criar dinamicamente a entidade com base
 * nos valores lidos diretamente do dataset.
 *
 * @param fields Array de strings contendo os atributos da reserva.
 * @return Estrutura RESERVATION completamente preenchida.
 */
RESERVATION build_reservation(char **fields);

/**
 * @brief Imprime os campos da reserva no stdout.
 *
 * Útil para debugging durante o desenvolvimento.
 *
 * @param r Ponteiro para a reserva.
 */
void print_reservation(RESERVATION r);

#endif
