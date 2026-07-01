#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "Executavel/programaIterativo.h"
#include "Catalogos/manager_c.h"
#include "menuNdata/queries.h"
#include "Utils/utils.h"
#include "IO/interpreter.h" 

#define MAX_BUFFER 512

// CAMINHO PADRÃO DO DATASET
// Nota: Certifica-te que este caminho existe na tua máquina ou muda para "dataset/data" se fores entregar
#define DEFAULT_DATASET_PATH "/Users/octavioph/Desktop/dataset-fase-2/sem_erros"

void programa_interativo() {
    char input[MAX_BUFFER];
    char *path = DEFAULT_DATASET_PATH;

    printf("=== MODO INTERATIVO ===\n");
    printf("A carregar dataset de: '%s'...\n", path);

    // 1. Inicialização dos catálogos
    PASSENGERS_C passengers = create_passenger_catalog();
    FLIGHTS_C flights = create_flight_catalog();
    RESERVATIONS_C reservations = create_reservation_catalog();
    AIRCRAFTS_C aircrafts = create_aircraft_catalog();
    AIRPORTS_C airports = create_airport_catalog();
    
    MANAGER_C manager = create_manager_c(flights, passengers, aircrafts, reservations, airports);

    // 2. Carregar Dados (Automático)
    if (!set_catalogs(manager, path)) { 
        printf("[ERRO] Não foi possível encontrar ficheiros CSV em '%s'.\n", path);
        printf("Certifique-se que o caminho está correto.\n");
        free_manager_c(manager);
        return;
    }

    printf(">>> Dados carregados com sucesso! <<<\n");
    printf("Insira a query (ex: 6 Portugal) ou 'sair':\n");

    // 3. Loop de Queries
    while (1) {
        printf(">> ");
        if (!fgets(input, MAX_BUFFER, stdin)) break;
        input[strcspn(input, "\n")] = 0; // Remove newline

        if (strlen(input) == 0) continue;
        if (strcasecmp(input, "sair") == 0 || strcasecmp(input, "exit") == 0) break;

        // Parse de argumentos (separa por espaços)
        // AVISO: Se a nacionalidade tiver espaços (ex: "United States"), este parser vai falhar.
        // O ideal seria melhorar o parser para suportar aspas, mas para casos simples funciona.
        char *args[10] = {NULL};
        int i = 0;
        char *token = strtok(input, " ");
        while (token && i < 10) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        if (i > 0) {
            int queryID = atoi(args[0]);
            char *result = NULL;
            
            // args + 1 ignora o ID da query e passa só os parâmetros
            switch (queryID) {
                case 1: result = (char*)query1(manager, args + 1, 0); break;
                case 2: result = (char*)query2(manager, args + 1, 0); break;
                case 3: result = (char*)query3(manager, args + 1, 0); break;
                case 5: result = (char*)query5(manager, args + 1, 0); break;
                
                // --- ADICIONADO: QUERY 6 ---
                case 6: result = (char*)query6(manager, args + 1, 0); break;

                default: printf("Query %d não implementada ou inválida.\n", queryID);
            }
            
            if (result) {
                // Verifica se o resultado não é vazio ou apenas uma quebra de linha
                if (strlen(result) > 0 && strcmp(result, "\n") != 0)
                    printf("%s", result);
                else
                    printf("Sem resultados.\n");
                
                free(result); // Importante: libertar a memória alocada pela query
            }
        }
    }

    free_manager_c(manager);
    printf("Programa terminado.\n");
}