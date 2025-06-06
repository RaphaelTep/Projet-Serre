#include "Serveur.h"
#define EEPROM_SIZE 32

Serveur::Serveur(int port, const char* indexHtml) 
    : server(port), indexHtml(indexHtml) {}

void Serveur::init() {
    server.begin();
    Serial.println("Serveur HTTP démarré !");
}

void Serveur::definirRoutes(float& temperature, float& hygrometrie, int& hum_plante_1, int& hum_plante_2,
                            float& temp_min, float& temp_max, float& hygrometrie_min, float& hygrometrie_max,
                            int& hum_min_1, int& hum_min_2) {

    server.on("/", HTTP_GET, [&temperature, &hygrometrie, &hum_plante_1, &hum_plante_2, this](AsyncWebServerRequest* request) {
        String page = indexHtml;
        page.replace("%TEMPERATURE%", String(temperature));
        page.replace("%HYGROMETRIE%", String(hygrometrie));
        page.replace("%HUM_PLANTE_1%", String(hum_plante_1));
        page.replace("%HUM_PLANTE_2%", String(hum_plante_2));
        request->send(200, "text/html", page);
    });

    server.on("/seuils", HTTP_GET, [&temp_min, &temp_max, &hygrometrie_min, &hygrometrie_max, &hum_min_1, &hum_min_2](AsyncWebServerRequest* request) {
        String page = "<html><body><h2>Configuration des Seuils</h2><form method='POST' action='/seuils'>"
                      "Temperature Min : <input type='text' name='temp_min' value='" + String(temp_min) + "'><br>"
                      "Temperature Max : <input type='text' name='temp_max' value='" + String(temp_max) + "'><br>"
                      "Hygrometrie Min : <input type='text' name='hygrometrie_min' value='" + String(hygrometrie_min) + "'><br>"
                      "Hygrometrie Max : <input type='text' name='hygrometrie_max' value='" + String(hygrometrie_max) + "'><br>"
                      "Humidite sol 1 : <input type='text' name='hum_min_1' value='" + String(hum_min_1) + "'><br>"
                      "Humidite sol 2 : <input type='text' name='hum_min_2' value='" + String(hum_min_2) + "'><br>"
                      "<input type='submit' value='Enregistrer'></form></body></html>";
        request->send(200, "text/html", page);
    });

    server.on("/seuils", HTTP_POST, [&temp_min, &temp_max, &hygrometrie_min, &hygrometrie_max, &hum_min_1, &hum_min_2](AsyncWebServerRequest* request) {
        if (request->hasParam("temp_min", true)) temp_min = request->getParam("temp_min", true)->value().toFloat();
        if (request->hasParam("temp_max", true)) temp_max = request->getParam("temp_max", true)->value().toFloat();
        if (request->hasParam("hygrometrie_min", true)) hygrometrie_min = request->getParam("hygrometrie_min", true)->value().toFloat();
        if (request->hasParam("hygrometrie_max", true)) hygrometrie_max = request->getParam("hygrometrie_max", true)->value().toFloat();
        if (request->hasParam("hum_min_1", true)) hum_min_1 = request->getParam("hum_min_1", true)->value().toInt();
        if (request->hasParam("hum_min_2", true)) hum_min_2 = request->getParam("hum_min_2", true)->value().toInt();

        request->send(200, "text/html", "<html><body><h3>Seuils mis à jour !</h3><a href='/seuils'>Retour</a></body></html>");

        EEPROM.begin(EEPROM_SIZE);
        int addr = 0;
        EEPROM.put(addr, temp_min); addr += sizeof(temp_min);
        EEPROM.put(addr, temp_max); addr += sizeof(temp_max);
        EEPROM.put(addr, hygrometrie_min); addr += sizeof(hygrometrie_min);
        EEPROM.put(addr, hygrometrie_max); addr += sizeof(hygrometrie_max);
        EEPROM.put(addr, hum_min_1); addr += sizeof(hum_min_1);
        EEPROM.put(addr, hum_min_2); addr += sizeof(hum_min_2);
        EEPROM.commit();
    });

    server.on("/temperature", HTTP_GET, [&temperature](AsyncWebServerRequest* request) {
        request->send(200, "text/plain", String(temperature));
    });

    server.on("/hygrometrie", HTTP_GET, [&hygrometrie](AsyncWebServerRequest* request) {
        request->send(200, "text/plain", String(hygrometrie));
    });

    server.on("/hum_plante_1", HTTP_GET, [&hum_plante_1](AsyncWebServerRequest* request) {
        request->send(200, "text/plain", String(hum_plante_1));
    });

    server.on("/hum_plante_2", HTTP_GET, [&hum_plante_2](AsyncWebServerRequest* request) {
        request->send(200, "text/plain", String(hum_plante_2));
    });

    server.on("/mesures", HTTP_GET, [&temperature, &hygrometrie, &hum_plante_1, &hum_plante_2](AsyncWebServerRequest* request) {
        String json = "{";
        json += "\"temperature\":" + String(temperature) + ",";
        json += "\"hygrometrie\":" + String(hygrometrie) + ",";
        json += "\"hum_plante_1\":" + String(hum_plante_1) + ",";
        json += "\"hum_plante_2\":" + String(hum_plante_2);
        json += "}";
        request->send(200, "application/json", json);
    });
}
