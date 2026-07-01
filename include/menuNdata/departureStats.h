#ifndef DEPARTURES_STATS_H
#define DEPARTURES_STATS_H

#include "Catalogos/manager_c.h"
#include <glib.h>

/**
 * @file departures_stats.h
 * @brief Módulo responsável pela construção das estatísticas de partidas por data.
 *
 * Este módulo percorre o catálogo de voos e constrói uma estrutura hierárquica:
 *
 *     data  ->  (aeroporto -> número de partidas)
 *
 * A tabela resultante é armazenada dentro do MANAGER_C, permitindo que outras
 * queries acedam rapidamente às estatísticas sem recalcular a distribuição.
 */


/**
 * @brief Constrói a tabela de partidas por data e armazena-a no manager.
 *
 * A função percorre todos os voos no catálogo e:
 *  - extrai a data da partida;
 *  - identifica o aeroporto de origem;
 *  - incrementa o contador correspondente.
 *
 * A estrutura gerada é uma hash table principal onde:
 *  - a chave é uma string com a data (YYYY-MM-DD)
 *  - o valor é outra hash table (aeroporto -> número de partidas)
 *
 * @param manager Estrutura principal que contém todos os catálogos.
 */
void build_departures_by_date(MANAGER_C manager);


/**
 * @brief Imprime no terminal a hash de partidas por data.
 *
 * Função útil para debugging e validação.
 * Mostra todas as datas e, para cada uma, os aeroportos e respetivos contadores.
 *
 * @param manager Estrutura principal onde os dados foram previamente armazenados.
 */
void print_departures_by_date(MANAGER_C manager);

#endif
