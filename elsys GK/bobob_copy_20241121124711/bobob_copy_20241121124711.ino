#include <WiFi.h>
#include <HTTPClient.h>

// Wi-Fi-konfigurasjon
const char* ssid = "June";        // Wi-Fi-nettverkets navn
const char* password = "takkjune";  // Wi-Fi-passord
const char* serverName = "http://172.20.10.10:5003/update";  // Flask-serverens lokale IP-adresse og riktig port

//pins for ultralydsensoren
int trigPin = 17;
int echoPin = 16;

long distance; // Variabel for å lagre avstanden i cm
String materialType; // Variabel for å lagre materialtyben som bestemmes av avstanden

void setup() {
  // Starter seriell kommunikasjon for debugging
    Serial.begin(115200);
    // Konfigurerer pins for ultralydsensoren
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Koble til Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { // vent til Wi-Fi tilkoblingen er etablert
        delay(1000);
        Serial.println("Kobler til Wi-Fi...");
    }
    Serial.println("Koblet til Wi-Fi"); //Indikerer vellykket tilkobling
}

void loop() {
    // Måling av avstand med ultraldsensoren
    long duration;
    digitalWrite(trigPin, LOW); // Sett trigPin lavt for å "nullstille"
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH); // send en 10-mikrosekunders puls
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW); // Sett trigPin lavt igjen

    // mål ekkoets varighet (i mikrosekunder) og bergen avstand
    duration = pulseIn(echoPin, HIGH);
    distance = (duration / 2) / 29.1; // Konverterer til avstand i cm (lydens hastighet i luft)

    // Klassifiserer materiale basert på avstand
    if (distance > 0 && distance <= 50) {  // Innenfor 50 cm
        materialType = "plast";
    } else if (distance > 50 && distance <= 100) {  // Mellom 50cm og 100cm
        materialType = "annet materiale";
    } else {  
        materialType = "søker"; //utenfor 100cm
    }

//sende data til Flask-serveren
    if (WiFi.status() == WL_CONNECTED) { //sjekker om ESP32 forsatt er tilkoblet Wi-Fi
        HTTPClient http; //Opprett en HTTP-klient
        http.begin(serverName); // Angi serverens URL
        http.addHeader("Content-Type", "application/json"); //Angi at vi sender JSON-data

        // Lag en JSON-streng som innehodler materialtypen
        String jsonPayload = "{\"material\": \"" + materialType + "\"}";
        int httpResponseCode = http.POST(jsonPayload); //Send post-forespørselen med JSON-data

// Skriv HTTP-responskoden til den serielle porten (fro debugging)
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        http.end(); // Lukk HTTP-tilkoblingen
    } else {
        Serial.println("Wi-Fi ikke tilkoblet"); //Feilmelding hvis WIFI-tilkoblingen ikke funker
    }
    delay(1000);  // Vent ett sekund før neste måling og dataoverføring
}
