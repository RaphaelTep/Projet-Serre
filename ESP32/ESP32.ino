#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <time.h>
#include <DHT.h>

// Définition du nom et mot de passe du Point d'Accès
const char *ssid = "ESP32_AP";
const char *passphrase = "DFxoUh02uF@*BHep";

// Définition de l'adresse IP du serveur
IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,1);  // Correction ici
IPAddress subnet(255,255,255,0);

// Déclaration du serveur Web
AsyncWebServer server(80); 

// Variables de capteur
int humidite; // humidité au sol
int hum_min;
#define HUM_PIN 34// Le pin de l'ESP32 qui va servir a lire le taux d'humidité au sol
#define SEUIL 500000 // Seuil du capteur d'humidité

float temperature; // Variable pour la température
float temp_min = 10.0;
float temp_max = 20.0;
float hygrometrie; // Variable pour le taux d'hygrométrie (humidité dans l'air)
float hygrometrie_min;
#define TEMP_HYG_PIN 4 // Le pin de l'ESP32 qui va servir a lire le capteur de température et d'hygrométrie
#define DHTTYPE DHT11 //On utilisera le DHT11
DHT temp_hyg(TEMP_HYG_PIN,DHTTYPE); // création d'un objet temp_hyg de la classe DHT

#define VENTI_PIN 13
#define ARROSE_PIN 12
#define BRUMI_PIN 33
#define HUMIVCC_PIN 32
 

time_t departTimer=time(NULL); // création d'un timer pour appeler les fonctions toutes les x secondes

// Page HTML avec mise à jour automatique
//Pour changer la vitesse d'actualisation, il faut changer le nombre après CONTENT= a la ligne 41 (en secondes)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
    <title>ESP32 Web Server</title>
    <META HTTP-EQUIV="Refresh" CONTENT="30;">  
    <script>
        function updateData() {
            fetch("/temperature")
                .then(response => response.text())
                .then(data => {
                    document.getElementById("tempValue").innerText = data;
            fetch("/humidite")
                .then(response => response.text())    
                .then(data => {
                    document.getElementById("HumValue").innerText = data;
            fetch("/hygrometrie")
                .then(response => response.text())
                .then(data => {
                    document.getElementById("hygValue").innerText = data;
                });
        }
        setInterval(updateData, 2000);
    </script>
</head>
<body>
    <h1>Informations de la serre</h1>
    <h1>Temperature         : <span id="tempValue">%TEMPERATURE%</span>°C</h1>
    <h1>Humidite dans l'air : <span id="HygValue">%HYGROMETRIE%</span>%</h1>
    <h1>Humidite au sol     : <span id="HumValue">%HUMIDITE%</span></h1>
</body>
</html>
)rawliteral";

void setup() {
    Serial.begin(9600);
    
    // Configuration du réseau WiFi AP
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(ssid, passphrase);
    delay(100);  // Ajout d'un court délai pour s'assurer que le WiFi démarre

    Serial.println("Point d'accès WiFi activé !");
    Serial.print("IP du serveur : ");
    Serial.println(WiFi.softAPIP());

    // Vérification du mode AP
    Serial.println("Vérification du mode AP...");
    if (WiFi.getMode() == WIFI_AP) {
        Serial.println("L'ESP32 est bien en mode Point d'Accès !");
    } else {
        Serial.println("ERREUR : L'ESP32 n'est pas en mode AP !");
    }

    // Définition des routes du serveur
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String page = index_html;
        page.replace("%TEMPERATURE%", String(temperature));
        page.replace("%HYGROMETRIE%", String(hygrometrie));
        page.replace("%HUMIDITE%", String(humidite));
        request->send(200, "text/html", page);
    });

    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(temperature));
    });

    server.on("/humidite", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(humidite));
    });

    server.on("/hygrometrie", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(hygrometrie));
    });

    // Démarrage du serveur
    server.begin();
    Serial.println("Serveur HTTP démarré !");
    
    // Initialisation du capteur DHT11
    temp_hyg.begin();

    //Définit le pin d'input 13 (celui du ventilateur) en tant que pin de sortie
    pinMode(VENTI_PIN,OUTPUT);
    pinMode(ARROSE_PIN,OUTPUT);
}

// code de la fonction qui récupère l'humidité au sol
void Hum_sol()
{
  humidite = analogRead(HUM_PIN);
  if (humidite < SEUIL)
  {
    Serial.print("\n \nLe sol est trop sec ! Il faut arroser !");
  }
  else
  {
    Serial.print("\nLe sol est assez humide");
  }
}
//
void Temp_Hum()
{
  temperature = temp_hyg.readTemperature();
  hygrometrie = temp_hyg.readHumidity();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  if (time(NULL) - departTimer >= 30) // Mise à jour des données toutes les x secondes
  {
    Temp_Hum();
    Hum_sol();
    departTimer = time(NULL);
    if (temperature>temperature_max)   
    {
      digitalWrite(VENTI_PIN, HIGH);
      delay(10000);
      digitalWrite(VENTI_PIN,LOW);
    }

    if (hygrometrie < hygrometrie_min)
    {
      digitalWrite(ARROSE_PIN, HIGH);
      delay(10000);
      digitalWrite(ARROSE_PIN,LOW);
    }
    Serial.print("\ntemperature : ");
    Serial.print(temperature);
    Serial.print("°C");
    Serial.print("\nHumidite dans l'air : ");
    Serial.print(hygrometrie);
    Serial.print("%");
    Serial.print("\nHumidité au sol : ");
    Serial.print(humidite);
    
  }
}