// Importação de bibliotecas
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <WiFi.h>                      
#include <IOXhop_FirebaseESP32.h>        
#include <ArduinoJson.h> 

// Definições do Wi-Fi e Firebase
#define WIFI_SSID "--------"                  // Nome da rede Wi-Fi
#define WIFI_PASSWORD "--------"             // Senha do Wi-Fi
#define FIREBASE_HOST "-----------"  // URL do Firebase Database  
#define FIREBASE_AUTH "--------------" // Chave de autenticação


// Inicializa o Firebase e LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço do LCD 16x2

// Configuração do sensor
int potenciometroPin = 34;  // Pino do potenciômetro (ESP32 - troque para A0 no Arduino Uno)
const float resistencia = 1000.0;  // Resistência em ohms

void setup() {

Serial.begin(115200);      //inicia comunicação serial
  Serial.println();          //imprime pulo de linha

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);     //inicia comunicação com wifi com rede definica anteriormente
  
  Serial.print("Conectando ao wifi");       //imprime "Conectando ao wifi"
  while (WiFi.status() != WL_CONNECTED)     //enquanto se conecta ao wifi fica colocando pontos
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();                         //imprime pulo de linha

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   //inicia comunicação com firebase definido anteriormente


  // Inicializando LCD
  lcd.init();
  lcd.backlight();
}

void loop() {
  // Ler a tensão do potenciômetro
  int leituraPot = analogRead(potenciometroPin);
  float tensao = (leituraPot / 4095.0) * 3.3;  // Para ESP32 (3.3V). Se for Arduino Uno, troque para 5.0
  
  // Calcular a corrente usando a Lei de Ohm
  float corrente = tensao / resistencia;

  // Exibir no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print(tensao, 2);
  lcd.print(" V");
  
  lcd.setCursor(0, 1);
  lcd.print("I: ");
  lcd.print(corrente, 3);
  lcd.print(" A");

 

  // Enviar dados para o Firebase
  Firebase.setInt("/Biorreator/Tensao", tensao);
  Firebase.setInt("/Biorreator/Corrente", corrente);
  delay(5000); // Aguarde 5 segundos antes da próxima leitura
}
