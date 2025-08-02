#include <SPI.h>
#include <SD.h>

// Pino analógico e constante de resistência
const int potenciometro = A0; 
const float resistencia = 1000.0; // 1 kΩ

// Pino CS do módulo SD
const int chipSelect = 4;

void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando cartao SD...");

  // Inicializa o cartão SD
  if (!SD.begin(chipSelect)) {
    Serial.println("Falha ao acessar o cartao !");
    Serial.println("Verifique o cartao/conexoes e reinicie o Arduino...");
    while (1); // Para o programa se falhar
  }

  Serial.println("Cartao iniciado corretamente !");
  Serial.println();
}

void loop() {
  // Tempo desde o início em segundos
  unsigned long tempoSegundos = millis() / 1000;

  // Leitura e cálculo
  int leituraAnalogica = analogRead(potenciometro); 
  float tensao = leituraAnalogica * (5.0 / 1023.0); 
  float corrente = tensao / resistencia; // Em amperes

  // Exibe no serial
  Serial.print("Tempo: ");
  Serial.print(tempoSegundos);
  Serial.println(" s");

  Serial.print("Tensao: ");
  Serial.print(tensao, 3);
  Serial.println(" V");

  Serial.print("Corrente: ");
  Serial.print(corrente * 1000, 3); // Em mA
  Serial.println(" mA");
  Serial.println();

  // Grava no cartão SD
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.print("Tempo: ");
    dataFile.print(tempoSegundos);
    dataFile.print(" s, ");

    dataFile.print("Tensao: ");
    dataFile.print(tensao, 3);
    dataFile.print(" V, ");

    dataFile.print("Corrente: ");
    dataFile.print(corrente * 1000, 3);
    dataFile.println(" mA");

    dataFile.close();
  } else {
    Serial.println("Erro ao abrir o arquivo datalog.txt");
  }

  delay(2000); // Aguarda 2 segundos
}