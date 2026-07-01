/**
 * @file passenger.h
 * @brief Interface da entidade PASSENGER e operações associadas.
 *
 * Este módulo define a estrutura opaca PASSENGER, responsável por armazenar
 * informação relativa a passageiros, incluindo número de documento e nome.
 * A implementação interna é totalmente encapsulada, promovendo segurança
 * e integridade dos dados.
 */

#ifndef PASSENGER_H
#define PASSENGER_H

#include <glib.h>

/* ================================
 *        Estrutura Opaca
 * ================================ */

/**
 * @brief Estrutura opaca que representa um passageiro.
 *
 * Os detalhes internos são ocultados para reforçar o encapsulamento.
 */
typedef struct passenger *PASSENGER;

/* ================================
 *        Funções Base
 * ================================ */

/**
 * @brief Cria um novo passageiro com campos inicializados a NULL.
 *
 * @return Ponteiro para um PASSENGER criado dinamicamente.
 */
PASSENGER create_passenger(void);

/**
 * @brief Liberta a memória associada a um passageiro.
 *
 * Adequada para ser utilizada como destrutor em GHashTables.
 *
 * @param data Ponteiro genérico para o passageiro a destruir.
 */
void free_passenger(void *data);

/* ================================
 *              Setters
 * ================================ */

/**
 * @brief Define o número de documento do passageiro.
 *
 * @param p Ponteiro para o passageiro.
 * @param doc Número do documento (string).
 */
void set_passenger_document_number(PASSENGER p, const gchar *doc);

/**
 * @brief Define o primeiro nome do passageiro.
 *
 * @param p Ponteiro para o passageiro.
 * @param first Primeiro nome.
 */
void set_passenger_first_name(PASSENGER p, const gchar *first);

/**
 * @brief Define a nacionalidade do passageiro.
 * @param p Ponteiro para o passageiro.
 * @param nat Nacionalidade.
 */
void set_passenger_nationality(PASSENGER p, const gchar *nat);

/*
 * Setters opcionais caso pretendas ativar mais campos:
 *
 * void set_passenger_last_name(PASSENGER p, const gchar *last);
 * void set_passenger_data_nascimento(PASSENGER p, const gchar *data_nascimento);
 * void set_passenger_nationality(PASSENGER p, const gchar *nat);
 * void set_passenger_gender(PASSENGER p, const gchar *gender);
 * void set_passenger_email(PASSENGER p, const gchar *email);
 * void set_passenger_phone(PASSENGER p, const gchar *phone);
 * void set_passenger_address(PASSENGER p, const gchar *addr);
 */

/* ================================
 *              Getters
 * ================================ */

/**
 * @brief Obtém o número de documento do passageiro.
 *
 * @param p Ponteiro para o passageiro.
 * @return String com o número do documento.
 */
const gchar* get_passenger_document_number(const PASSENGER p);

/**
 * @brief Obtém o primeiro nome do passageiro.
 *
 * @param p Ponteiro para o passageiro.
 * @return String com o primeiro nome.
 */
const gchar* get_passenger_first_name(const PASSENGER p);

/**
 * @brief Obtém a nacionalidade do passageiro.
 * @param p Ponteiro para o passageiro.
 * @return String com a nacionalidade.
 */
const gchar* get_passenger_nationality(const PASSENGER p);

/*
 * Getters opcionais para campos adicionais:
 *
 * const gchar* get_passenger_last_name(const PASSENGER p);
 * const gchar* get_passenger_data_nascimento(const PASSENGER p);
 * const gchar* get_passenger_nationality(const PASSENGER p);
 * const gchar* get_passenger_gender(const PASSENGER p);
 * const gchar* get_passenger_email(const PASSENGER p);
 * const gchar* get_passenger_phone(const PASSENGER p);
 * const gchar* get_passenger_address(const PASSENGER p);
 */

/* ================================
 *               Util
 * ================================ */

/**
 * @brief Constrói um passageiro a partir dos campos de uma linha do CSV.
 *
 * Função utilizada pelo parser para transformar os valores lidos num objeto PASSENGER.
 *
 * @param fields Array de strings representando os atributos do passageiro.
 * @return Objeto PASSENGER preenchido.
 */
PASSENGER build_passenger(char **fields);

/**
 * @brief Imprime todos os campos do passageiro.
 *
 * Útil para debugging.
 *
 * @param p Ponteiro para o passageiro.
 */
void print_passenger(PASSENGER p);

#endif
