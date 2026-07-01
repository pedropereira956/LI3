#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include <glib.h>
#include "Entidades/flight.h"   
#include "Valida/validates.h"
#include "Utils/utils.h"
#include "Catalogos/flights_c.h"
#include "Entidades/aircraft.h"
#include "Entidades/passenger.h"
#include "Catalogos/aircrafts_c.h"
#include "Catalogos/passengers_c.h"
#include "Entidades/reservation.h"
#include "Catalogos/reservations_c.h"
#include "Catalogos/airports_c.h"
#include "Catalogos/manager_c.h"
#include "Executavel/programaPrincipal.h"
#include "Executavel/programaTestes.h"
#include "Executavel/programaIterativo.h" // Header da nova função

int main(int argc, char** argv) {

    // 1. MODO INTERATIVO
    // Se correres ./programa-iterativo (sem argumentos), entra aqui direto.
    if (argc == 1 && strstr(argv[0], "programa-iterativo") != NULL) {
        programa_interativo();
        return 0; 
    }

    // 2. MODOS BATCH E TESTES
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);

    // Verifica se é programa-principal (Batch)
    if (argc == 3 && strstr(argv[0], "programa-principal") != NULL) {
        programa_principal(argv[1], argv[2]);
    }
    // Verifica se é programa-testes
    else if (argc == 4 && strstr(argv[0], "programa-testes") != NULL) {
        programa_testes(argv[1], argv[2], argv[3]);
        // return 0; // Comentei para mostrar tempos, descomenta se não quiseres
    }
    else {
        printf("Argumentos inválidos ou executável desconhecido.\n");
        printf("  Iterativo: ./programa-iterativo\n");
        printf("  Batch:     ./programa-principal <dataset> <input>\n");
        printf("  Testes:    ./programa-testes <dataset> <input> <esperado>\n");
        return 0;
    }

    clock_gettime(CLOCK_REALTIME, &end);
    
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);

    printf("Elapsed time: %.6f seconds\n", elapsed);
    printf("Memory usage: %ld KB\n", r_usage.ru_maxrss);

    return 0;
}