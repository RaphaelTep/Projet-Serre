
#include <WiFi.h>

//Definition du nom du point d'accès et de son mot de passe :
const char *ssid = "ESP32_AP";
const char *passphrase = "123456789";

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid,passphrase) ? "Ready" : "Failed!");
  //WiFi.softAP(ssid);
  //WiFi.softAP(ssid, passphrase, channel, ssdi_hidden, max_connection)
  
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  Serial.print("[Server Connected] ");
  Serial.println(WiFi.softAPIP());

  delay(500);
}

/*
#include <WiFi.h>

//Definition du nom du point d'accès et de son mot de passe :
const char *ssid = "ESP32_AP";
const char *passphrase = "123456789";

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

//creation du serveur http sur le port 80
WiFiServer server(80);
  
void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid,passphrase) ? "Ready" : "Failed!");
  //WiFi.softAP(ssid);
  //WiFi.softAP(ssid, passphrase, channel, ssdi_hidden, max_connection)
  
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}

void loop() 
{
  Serial.print("[Server Connected] ");
  Serial.println(WiFi.softAPIP());

  //delay(500);
  WiFiClient client = server.available();
  if (client) 
  {
    Serial.println("Nouveau client !");
    String currentLine = "";
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        Serial.write(c);

        // Fin de ligne HTTP
        if (c == '\n') 
        {
          // Ligne vide, envoyer une réponse HTTP
          if (currentLine.length() == 0) 
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<!DOCTYPE html><html>");
            client.println("<head><title>ESP32 Serveur</title></head>");
            client.println("<body><h1>Bonjour depuis l'ESP32 !</h1></body>");
            client.println("</html>");
            break;
          } 
          else 
          {
            currentLine = "";
          }
        } 
        else if (c != '\r') 
        {
          currentLine += c;
        }
      }
    }
    // Déconnexion du client
    client.stop();
    Serial.println("Client déconnecté.");
  }
}
*/

