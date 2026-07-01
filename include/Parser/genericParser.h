#ifndef GENERICPARSER_H
#define GENERICPARSER_H

#include <stdio.h>
#include <glib.h>

/**
 * @file genericparser.h
 * @brief Módulo responsável pelo parsing genérico de ficheiros CSV.
 *
 * Este módulo abstrai a lógica de leitura e separação de campos de linhas CSV,
 * permitindo reutilizar código para diferentes tipos de entidades. Inclui
 * funções para ler linha a linha, dividir em campos e encaminhar os dados para
 * as funções de validação e construção das entidades correspondentes.
 */


/**
 * @brief Lê e processa todas as linhas de um ficheiro CSV.
 *
 * Esta função:
 *  - lê o ficheiro linha a linha;
 *  - divide cada linha nos seus campos através da função parseL();
 *  - valida e constrói cada entidade usando a função específica passada pelo catálogo;
 *  - insere a entidade nos respetivos catálogos;
 *  - escreve entradas inválidas num ficheiro de erro.
 *
 * É uma função genérica usada para todos os tipos de ficheiros (voos, passageiros,
 * aeronaves, reservas, aeroportos…).
 *
 * @param f Ficheiro CSV a ser lido.
 * @param nome Nome da entidade (ex.: "flight", "passenger") usado para mensagens internas.
 * @param max_fields Número máximo de campos esperados por linha.
 * @param catalog Catálogo onde as entidades válidas serão inseridas.
 * @param error_f Ficheiro onde serão registadas linhas inválidas.
 */
void parseF(FILE* f, gchar* nome, int max_fields, void* catalog, FILE* error_f);


/**
 * @brief Divide uma linha CSV nos seus vários campos.
 *
 * Esta função trata:
 *  - separação por delimitadores;
 *  - remoção de quebras de linha;
 *  - alocação dinâmica de memória;
 *  - garantia de que a linha retorna sempre exatamente max_fields campos,
 *    preenchendo campos em falta com strings vazias.
 *
 * @param line Linha original do CSV.
 * @param max_fields Número de campos esperados.
 * @return Array dinâmico de strings contendo os campos já separados.
 *         (Deve ser libertado após utilização)
 */
char** parseL(char* line, int max_fields);

#endif
