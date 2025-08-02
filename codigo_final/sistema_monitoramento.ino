#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>
#include <avr/sleep.h> 
#include <avr/wdt.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int chipSelect = 4;
const int reator1 = A0;
const int reator2 = A1;
const int reator3 = A2;

// Resistências diferentes para cada reator (em ohms)
const float resistencia1 = 965.0;
const float resistencia2 = 969.0;
const float resistencia3 = 973.0;

volatile bool wdt_triggered = false;
int ciclos_sleep = 450; // 8s × 450 = ~1h

void setup() {
  pinMode( reator1, INPUT );
  pinMode( reator2, INPUT );
  pinMode( reator3, INPUT );

  Serial.begin(9600);
  analogReference( INTERNAL );

  lcd.init();
  lcd.backlight();

  if (!SD.begin(chipSelect)) {
    Serial.println("Erro ao iniciar SD");
    lcd.setCursor(0, 0);
    lcd.print("Erro SD Card!");
    while (1);
  }

  lcd.setCursor(0, 0);
  lcd.print("SD OK / LCD OK");
  delay(2000);
  lcd.clear();

  setupWatchdog();
}

void loop() {
  lcd.backlight();

  // Leituras
  int leitura1 = analogRead(reator1);
  int leitura2 = analogRead(reator2);
  int leitura3 = analogRead(reator3);

  float tensao1 = leitura1 * (1.1 / 1023.0);
  float tensao2 = leitura2 * (1.1 / 1023.0);
  float tensao3 = leitura3 * (1.1 / 1023.0);

  float corrente1 = tensao1 / resistencia1;
  float corrente2 = tensao2 / resistencia2;
  float corrente3 = tensao3 / resistencia3;

  // Serial Monitor
  Serial.print("Tensoes (V): ");
  Serial.print(tensao1, 2); Serial.print(" ");
  Serial.print(tensao2, 2); Serial.print(" ");
  Serial.print(tensao3, 2);
  Serial.print(" | Correntes (mA): ");
  Serial.print(corrente1 * 1000, 1); Serial.print(" ");
  Serial.print(corrente2 * 1000, 1); Serial.print(" ");
  Serial.println(corrente3 * 1000, 1);

  // Exibição no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T");
  lcd.print(tensao1, 2); lcd.print(" ");
  lcd.print(tensao2, 2); lcd.print(" ");
  lcd.print(tensao3, 2);
  lcd.print("V");

  lcd.setCursor(0, 1);
  lcd.print("C");
  lcd.print(corrente1 * 1000, 1); lcd.print(" ");
  lcd.print(corrente2 * 1000, 1); lcd.print(" ");
  lcd.print(corrente3 * 1000, 1);
  lcd.print("mA");

  // Gravação no SD
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print("Tempo: ");
    dataFile.print(millis() / 1000);
    dataFile.print("s");

    dataFile.print(", Reator1: ");
    dataFile.print(tensao1, 2); dataFile.print("V ");
    dataFile.print(corrente1 * 1000, 2); dataFile.print("mA");

    dataFile.print(", Reator2: ");
    dataFile.print(tensao2, 2); dataFile.print("V ");
    dataFile.print(corrente2 * 1000, 2); dataFile.print("mA");

    dataFile.print(", Reator3: ");
    dataFile.print(tensao3, 2); dataFile.print("V ");
    dataFile.print(corrente3 * 1000, 2); dataFile.println("mA");

    dataFile.close();
  } else {
    Serial.println("Erro ao gravar no SD");
  }

  delay(60000); // Espera acordado por 60 segundos

  lcd.noBacklight();

  // Dorme por ~1 hora
  for (int i = 0; i < ciclos_sleep; i++) {
    dormirComWatchdog();
  }
}

// === Sleep com Watchdog ===
void setupWatchdog() {
  MCUSR &= ~(1 << WDRF);
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDIE) | (1 << WDP3); // 8s
}

void dormirComWatchdog() {
  wdt_triggered = false;
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  noInterrupts();
  sleep_bod_disable();
  interrupts();
  sleep_cpu();
  sleep_disable();
}

ISR(WDT_vect) {
  wdt_triggered = true;
}
