#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o display no endereço I2C 0x27 com 16 colunas e 2 linhas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Configuração do pino analógico e resistência
const int potenciometro = A0;
const float resistencia = 1000.0; // Em ohms (1kΩ)

void setup() {
  Serial.begin(9600);     // Inicia a comunicação serial
  lcd.init();             // Inicializa o LCD
  lcd.setBacklight(HIGH); // Liga a luz de fundo
}

void loop() {
  // Leitura do valor analógico
  int leituraAnalogica = analogRead(potenciometro);

  // Conversão para tensão
  float tensao = leituraAnalogica * (5.0 / 1023.0);

  // Cálculo da corrente usando a Lei de Ohm
  float corrente = tensao / resistencia; // Corrente em A

  // Exibe no monitor serial
  Serial.print("Tensão: ");
  Serial.print(tensao);
  Serial.println(" V");

  Serial.print("Corrente: ");
  Serial.print(corrente * 1000); // Em mA
  Serial.println(" mA");

  // Exibe no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(tensao, 2);
  lcd.print("V");

  lcd.setCursor(0, 1);
  lcd.print("I:");
  lcd.print(corrente * 1000, 2); // Em mA
  lcd.print("mA");

  delay(1000); // Atualiza a cada 1 segundo
}