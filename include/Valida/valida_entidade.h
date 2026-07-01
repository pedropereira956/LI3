#ifndef VALIDA_ENTIDADE_H
#define VALIDA_ENTIDADE_H   

/**
 * @file valida_entidade.h
 * @brief Módulo responsável pela validação do conteúdo dos campos de cada entidade.
 *
 * Este módulo contém funções que verificam se uma linha do CSV contém dados
 * válidos para criação de uma entidade (passageiro, aeroporto, aeronave, voo ou
 * reserva). Cada função aplica regras específicas de formato, lógica e consistência.
 *
 * Caso a validação falhe, a entidade não deve ser inserida nos catálogos e a linha
 * original deve ser escrita no respetivo ficheiro *_errors.csv.
 */


/**
 * @brief Valida os campos correspondentes a um passageiro.
 *
 * Verifica:
 *  - formato do número de documento;
 *  - nomes válidos;
 *  - presença de campos obrigatórios.
 *
 * @param fields Vetor de strings com os campos da linha.
 * @return 1 se válido, 0 se inválido.
 */
int verify_passenger(char **fields);


/**
 * @brief Valida os campos de um aeroporto.
 *
 * Verifica:
 *  - código IATA (3 letras);
 *  - nome e cidade;
 *  - país;
 *  - tipo de aeroporto.
 *
 * @param fields Vetor de strings com os campos da linha.
 * @return 1 se válido, 0 se inválido.
 */
int verify_airport(char **fields);


/**
 * @brief Valida os campos de uma aeronave.
 *
 * Verifica:
 *  - identificador;
 *  - fabricante;
 *  - modelo;
 *  - formato dos campos obrigatórios.
 *
 * @param fields Vetor de strings com os campos da linha.
 * @return 1 se válido, 0 se inválido.
 */
int verify_aircraft(char **fields);


/**
 * @brief Valida os campos de um voo.
 *
 * Verifica:
 *  - formato das datas de partida/chegada (real e prevista);
 *  - airports válidos;
 *  - estado do voo;
 *  - integridade dos campos essenciais.
 *
 * @param fields Vetor de strings com os campos da linha.
 * @return 1 se válido, 0 se inválido.
 */
int verify_flight(char **fields);


/**
 * @brief Valida os campos de uma reserva.
 *
 * Verifica:
 *  - existência do passageiro associado;
 *  - lista de voos válida e existente;
 *  - integridade dos campos obrigatórios.
 *
 * @param fields Vetor de strings com os campos da linha.
 * @return 1 se válido, 0 se inválido.
 */
int verify_reservation(char **fields);

#endif
