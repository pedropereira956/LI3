#include "Executavel/programaPrincipal.h"
#include <stdio.h>
#include <stdlib.h>
#include "Catalogos/aircrafts_c.h"
#include "Catalogos/airports_c.h"
#include "Catalogos/flights_c.h"
#include "Catalogos/manager_c.h"
#include "Catalogos/passengers_c.h"
#include "Catalogos/reservations_c.h"
#include "Utils/utils.h"
#include "Entidades/airport.h"
#include "IO/interpreter.h"
#include "menuNdata/departureStats.h"


void programa_principal (char* path1, char* path2) {

        // Criação dos catálogos
        PASSENGERS_C passengers_catalog = create_passenger_catalog();
        FLIGHTS_C flights_catalog = create_flight_catalog();
        RESERVATIONS_C reservations_catalog = create_reservation_catalog();
        AIRCRAFTS_C aircrafts_catalog = create_aircraft_catalog();
        AIRPORTS_C airports_catalog = create_airport_catalog();
    
        // Criação do catálogo gestor
        MANAGER_C manager_catalog = create_manager_c(flights_catalog,passengers_catalog,aircrafts_catalog,reservations_catalog,airports_catalog);


        if (!set_catalogs(manager_catalog,path1)) {
            return;
        }

        if(!execute_queries(manager_catalog,path2,1)) {
            return;
        }

        free_manager_c(manager_catalog);
}