// Configuração do pino analógico 
const int potenciometro = A0; 
const float resistencia = 1000.0; // Resistência em ohms (exemplo: 1 kΩ) 

void setup() { 
  Serial.begin(9600);  // Inicia a comunicação serial 
} 
 void loop() { 
   
int leituraAnalogica = analogRead(potenciometro); // Lê o valor analógico (0-1023) 
// Converte para tensão (0-5V) 
float tensao = leituraAnalogica * (5.0 / 1023.0); 
// Calcula a corrente usando a Lei de Ohm 
float corrente = tensao / resistencia; // Corrente em amperes 

// Envia os valores para o monitor serial 
Serial.print("Tensão: "); 
Serial.print(tensao); 
Serial.println(" V"); 
Serial.print("Corrente: "); 
Serial.print(corrente * 1000); // Corrente em mA 
Serial.println(" mA"); 
delay(500); // Aguarda 500 ms 
 }