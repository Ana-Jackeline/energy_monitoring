#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,16,2);

int potenciometroPin = 34;  // Pino analógico para o potenciômetro (tensão)
const float resistencia = 1000.0;  // Resistência em ohms
 
void setup()
{
 lcd.init();
}
 
void loop()
{
  // Ler a tensão do potenciômetro (em Volts)
  int leituraPot = analogRead(potenciometroPin);
  float tensao = (leituraPot / 4095.0) * 3.3;  // Tensão máxima do Arduino é 5V
  
  // Calcular a corrente usando a Lei de Ohm
  float corrente = tensao / resistencia;  // Corrente em amperes

  lcd.setBacklight(HIGH);
  
  // Exibir no LCD
  lcd.setCursor(0, 0);  // Posiciona o cursor na primeira linha
  lcd.print("T: ");
  lcd.print(tensao, 2);  // Exibe a tensão com 2 casas decimais
  lcd.print(" V");
  
  lcd.setCursor(0, 1);  // Posiciona o cursor na segunda linha
  lcd.print("I: ");
  lcd.print(corrente, 3);  // Exibe a corrente com 3 casas decimais (em Ampere)
  lcd.print(" A");

  delay(1000);
