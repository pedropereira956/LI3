/**
 * @file programaPrincipal.h
 * @brief Declaração da função principal de execução do sistema.
 *
 * Este módulo contém a interface da função responsável por coordenar
 * todo o fluxo de execução do programa. A função programa_principal
 * é o ponto central onde ocorre o carregamento dos datasets, criação
 * dos catálogos, interpretação dos comandos do ficheiro de input e
 * geração dos outputs. 
 *
 * Atua como "controlador" de alto nível, estabelecendo a ligação entre
 * os módulos de parsing, validação, catálogos, queries e output.
 */

#ifndef PROGRAMAPRINCIPAL_H
#define PROGRAMAPRINCIPAL_H

#include "Catalogos/aircrafts_c.h"
#include "Catalogos/airports_c.h"
#include "Catalogos/flights_c.h"
#include "Catalogos/manager_c.h"
#include "Catalogos/passengers_c.h"
#include "Catalogos/reservations_c.h"
#include "Utils/utils.h"

/**
 * @brief Função principal que coordena todo o processamento do sistema.
 *
 * Esta função é responsável por:
 *  - Carregar os ficheiros .csv a partir do diretório especificado.
 *  - Criar os diferentes catálogos (voos, passageiros, aeroportos,
 *    aeronaves e reservas).
 *  - Construir o manager_c, que agrega todos os catálogos.
 *  - Interpretar o ficheiro de input, executando cada query pela ordem
 *    indicada.
 *  - Gerar os ficheiros commandN_output.txt com os resultados.
 *  - Libertar toda a memória alocada ao longo da execução.
 *
 * @param path1 Caminho para o diretório dos datasets (.csv).
 * @param path2 Caminho para o ficheiro de input contendo as queries.
 */
void programa_principal(char *path1, char *path2);

#endif
