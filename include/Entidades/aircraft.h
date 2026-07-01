/**
 * @file aircraft.h
 * @brief Definição da estrutura e funções associadas a uma aeronave.
 *
 * Este módulo define a estrutura AIRCRAFT e as operações de criação,
 * modificação e acesso aos seus campos. As aeronaves representam entidades
 * fundamentais do sistema, contendo informação base como identificador,
 * fabricante e modelo.
 */

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <glib.h>

/**
 * @brief Estrutura opaca que representa uma aeronave.
 *
 * Os detalhes internos são ocultados para garantir encapsulamento.
 */
typedef struct aircraft *AIRCRAFT;

/* ============================
 *        Funções Base
 * ============================ */

/**
 * @brief Cria uma nova aeronave com campos inicializados a NULL.
 *
 * @return Um ponteiro AIRCRAFT criado dinamicamente.
 */
AIRCRAFT create_aircraft(void);

/**
 * @brief Liberta a memória associada a uma aeronave.
 *
 * Esta função é compatível com destrutores de GHashTable.
 *
 * @param data Ponteiro genérico para a aeronave a libertar.
 */
void free_aircraft(void *data);

/* ============================
 *           Setters
 * ============================ */

/**
 * @brief Define o identificador da aeronave.
 *
 * @param a Ponteiro para a aeronave.
 * @param identifier String com o ID da aeronave.
 */
void set_aircraft_identifier(AIRCRAFT a, const gchar *identifier);

/**
 * @brief Define o fabricante da aeronave.
 *
 * @param a Ponteiro para a aeronave.
 * @param manufacturer Nome do fabricante.
 */
void set_aircraft_manufacturer(AIRCRAFT a, const gchar *manufacturer);

/**
 * @brief Define o modelo da aeronave.
 *
 * @param a Ponteiro para a aeronave.
 * @param model Nome do modelo da aeronave.
 */
void set_aircraft_model(AIRCRAFT a, const gchar *model);

/*
 * Caso ativados futuramente, os seguintes setters permitem
 * armazenar dados adicionais sobre a aeronave:
 *
 * void set_aircraft_year(AIRCRAFT a, gint year);
 * void set_aircraft_capacity(AIRCRAFT a, gint capacity);
 * void set_aircraft_range(AIRCRAFT a, gint range);
 */

/* ============================
 *           Getters
 * ============================ */

/**
 * @brief Obtém o identificador da aeronave.
 *
 * @param a Ponteiro para a aeronave.
 * @return String constante com o identificador.
 */
const gchar* get_aircraft_identifier(const AIRCRAFT a);

/**
 * @brief Obtém o fabricante da aeronave.
 *
 * @param a Ponteiro para a aeronave.
 * @return String com o nome do fabricante.
 */
const gchar* get_aircraft_manufacturer(const AIRCRAFT a);

/**
 * @brief Obtém o modelo da aeronave.
 *
 * @param a Ponteiro para a aeronave.
 * @return String com o modelo.
 */
const gchar* get_aircraft_model(const AIRCRAFT a);

/*
 * Getters opcionais correspondentes aos campos comentados:
 *
 * gint get_aircraft_year(const AIRCRAFT a);
 * gint get_aircraft_capacity(const AIRCRAFT a);
 * gint get_aircraft_range(const AIRCRAFT a);
 */

/* ============================
 *      Construção e Debug
 * ============================ */

/**
 * @brief Constrói uma aeronave a partir de um array de campos.
 *
 * Usado no parser ao carregar dados dos ficheiros CSV.
 *
 * @param fields Array de strings contendo os atributos da aeronave.
 * @return AIRCRAFT inicializado com os campos recebidos.
 */
AIRCRAFT build_aircraft(char **fields);

/**
 * @brief Imprime no stdout todos os campos da aeronave.
 *
 * Função útil para debugging.
 *
 * @param a Ponteiro para a aeronave.
 */
void print_aircraft(AIRCRAFT a);

#endif
