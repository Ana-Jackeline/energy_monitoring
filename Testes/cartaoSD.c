#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Pinos do SD
#define SCK  18
#define MISO 19
#define MOSI 23
#define CS   5

// Pino do potenciômetro
const int potenciometro = 34; // Use um pino ADC do ESP32
const float resistencia = 1000.0; // Ohms

SPIClass spi = SPIClass(VSPI);

void setup() {
  Serial.begin(115200);
  delay(1000); // Espera a porta serial iniciar

  // Inicia SPI para SD
  spi.begin(SCK, MISO, MOSI, CS);

  if (!SD.begin(CS, spi, 80000000)) {
    Serial.println("Falha ao montar o cartão SD");
    return;
  }

  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("Nenhum cartão SD detectado");
    return;
  }

  Serial.println("Cartão SD inicializado com sucesso.");
  
  // Cria arquivo e cabeçalho, se ainda não existir
  if (!SD.exists("/dados.csv")) {
    File file = SD.open("/dados.csv", FILE_WRITE);
    if (file) {
      file.println("Tensao (V),Corrente (mA)");
      file.close();
    }
  }
}

void loop() {
  // Leitura analógica
  int leitura = analogRead(potenciometro);
  
  // Conversão para tensão (ESP32: 0-4095 para 0-3.3V por padrão)
  float tensao = leitura * (3.3 / 4095.0);
  
  // Cálculo da corrente (Lei de Ohm)
  float corrente = tensao / resistencia; // em amperes

  // Exibe no monitor serial
  Serial.print("Tensão: ");
  Serial.print(tensao, 3);
  Serial.println(" V");
  Serial.print("Corrente: ");
  Serial.print(corrente * 1000, 3);
  Serial.println(" mA");

  // Grava no cartão SD
  File file = SD.open("/dados.csv", FILE_APPEND);
  if (file) {
    file.printf("%.3f,%.3f\n", tensao, corrente * 1000); // mA
    file.close();
  } else {
    Serial.println("Erro ao abrir o arquivo para gravar");
  }

  delay(1000); // Intervalo de 1 segundo
}