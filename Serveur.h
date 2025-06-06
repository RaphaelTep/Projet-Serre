#ifndef SERVEUR_H
#define SERVEUR_H

#include <ESPAsyncWebServer.h>
#include <EEPROM.h>

class Serveur {
private:
    AsyncWebServer server;
    const char* indexHtml;

public:
    Serveur(int port, const char* indexHtml);

    void init(); // Initialiser le serveur

    void definirRoutes(
        float& temperature,
        float& hygrometrie,
        int& hum_plante_1,
        int& hum_plante_2,
        float& temp_min,
        float& temp_max,
        float& hygrometrie_min,
        float& hygrometrie_max,
        int& hum_min_1,
        int& hum_min_2
    );
};

#endif
