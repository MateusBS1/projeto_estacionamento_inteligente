#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ===== DISPLAY =====
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== SENSORES =====
#define TRIG1 2
#define ECHO1 3
#define TRIG2 4
#define ECHO2 5

// ===== CONFIGURAÇÕES =====
const int distanciaLimite = 10; // cm para considerar vaga ocupada

// ===== CONTROLE DE TEMPO =====
unsigned long tempoSensor = 0;
unsigned long tempoDisplay = 0;

const unsigned long intervaloSensor = 100;   // leitura dos sensores
const unsigned long intervaloDisplay = 500;  // atualização do display

// ===== VARIÁVEIS =====
bool vaga1Ocupada = false;
bool vaga2Ocupada = false;
int vagasLivres = 2;

// ===== FUNÇÃO ULTRASSÔNICO =====
long medirDistancia(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duracao = pulseIn(echo, HIGH, 30000);
  long distancia = duracao * 0.034 / 2;

  return distancia;
}

void setup() {
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  unsigned long agora = millis();

  // ===== LEITURA DOS SENSORES =====
  if (agora - tempoSensor >= intervaloSensor) {
    tempoSensor = agora;

    long d1 = medirDistancia(TRIG1, ECHO1);
    long d2 = medirDistancia(TRIG2, ECHO2);

    vaga1Ocupada = (d1 > 0 && d1 < distanciaLimite);
    vaga2Ocupada = (d2 > 0 && d2 < distanciaLimite);

    vagasLivres = 2;
    if (vaga1Ocupada) vagasLivres--;
    if (vaga2Ocupada) vagasLivres--;
  }

  // ===== ATUALIZAÇÃO DO DISPLAY =====
  if (agora - tempoDisplay >= intervaloDisplay) {
    tempoDisplay = agora;

    lcd.clear();

    if (vagasLivres == 0) {
      lcd.setCursor(0, 0);
      lcd.print("ESTACIONAMENTO");
      lcd.setCursor(0, 1);
      lcd.print("   LOTADO   ");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Vagas livres:");
      lcd.setCursor(0, 1);
      lcd.print(vagasLivres);
      lcd.print(" disponiveis");
    }
  }
}