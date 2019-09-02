// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 7, d6 = 3, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int A = 2;      //variable A a pin digital 2 (CLK en modulo)
int B = 4;        //variable B a pin digital 4 (DT en modulo)

int ANTERIOR = 20;    // almacena valor anterior de la variable POSICION

volatile int POSICION = 20; // variable POSICION con valor inicial de 20 y definida
// como global al ser usada en loop e ISR (encoder)

///////////////////////////////////Termistor/////////////////////////////////////////////////////
int TermistorPin = A0; //Pin en donde leemos el voltaje proveniente del termistor.
int Vo;                //Variable para el voltaje del termistor.
float R1 = 10000;      //Resistencia que se está utilizando para el divisor de tensión.
float logR2, R2, T, Tc, Tf; //Variables utilizadas para que todo sea mas claro.
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07; //Coeficientes del termistor utilizado.

void setup() {
  Serial.begin(9600);   // incializacion de comunicacion serie a 9600 bps
  /////////////////////////////ENCODER/////////////////////////////////////////////////////////////
  pinMode(A, INPUT);    // A como entrada
  pinMode(B, INPUT);    // B como entrada
  pinMode(A1, INPUT);   //Botón Encoder
  attachInterrupt(digitalPinToInterrupt(A), encoder, LOW);// interrupcion sobre pin A con
  // funcion ISR encoder y modo LOW

  ////////////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////LCD//////////////////////////////////////////////////////////////////
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("SELECIONAR TEMP:");

  lcd.setCursor(0, 1);
  lcd.print(POSICION);
  lcd.print(" 'C");
  ///////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////PELTIER///////////////////////////////////////////////////////////
  pinMode(9, OUTPUT); // INPUT1 Peltier
  pinMode(10, OUTPUT); // INPUT2 Peltier
  pinMode(6, OUTPUT); // Enable Peltier
}
int contador = 0;
void loop() {
  //int  Temp = 35;
  // put your main code here, to run repeatedly:
  Vo = analogRead(TermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  Tc = T - 273.15;
  //Tf = (Tc * 9.0)/ 5.0 + 32.0;
  int boton = analogRead(A1);
  Serial.println(boton);

  if (boton >= 5 && contador == 0) {
    if (POSICION != ANTERIOR) { // si el valor de POSICION es distinto de ANTERIOR
      // set the cursor to column 0, line 1
      // (note: line 1 is the second row, since counting begins with 0):
      lcd.setCursor(0, 1);
      // print the number of seconds since reset:
      lcd.print(POSICION);
      ANTERIOR = POSICION ; // asigna a ANTERIOR el valor actualizado de POSICION
    }
    contador = 1;
  }

  if (boton >= 5 && contador == 1) {
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print(POSICION);
    lcd.print(" 'C");
    if (POSICION != ANTERIOR) { // si el valor de POSICION es distinto de ANTERIOR
      // set the cursor to column 0, line 1
      // (note: line 1 is the second row, since counting begins with 0):
      lcd.setCursor(0, 1);
      // print the number of seconds since reset:
      lcd.print(POSICION);
      lcd.print(" 'C");
      lcd.print("      ");
      ANTERIOR = POSICION ; // asigna a ANTERIOR el valor actualizado de POSICION
    }
  }

  if (boton <= 5 && contador == 1) {
    lcd.setCursor(0, 1);
    lcd.print("     ");
    lcd.setCursor(0, 1);
    lcd.print("BOTON");
    delay(1000);
    contador = 2;
  }

  if (contador == 2) {
    lcd.begin(16, 2);
    lcd.print("TEMP DESEADA: ");
    lcd.print(POSICION);
    contador = 3;
  }

  if (contador == 3) {
    static int Temp = POSICION;

    if (Tc <= Temp) {
      digitalWrite(6, HIGH); //Enable
      digitalWrite(9, LOW);   //Rojo
      digitalWrite(10, HIGH); //Negro
    }
    else {
      digitalWrite(10, LOW);  //Negro
      digitalWrite(9, HIGH);  //Rojo
      digitalWrite(6, HIGH); //Enable
    }

    lcd.setCursor(0, 1);
    lcd.print("        ");
    lcd.setCursor(0, 1);
    lcd.print(int(Tc));
    lcd.print(" 'C");
    delay(200);
  }
}

void encoder()  {
  static /*unsigned*/ long ultimaInterrupcion = 0;  // variable static con ultimo valor de
  // tiempo de interrupcion
  unsigned long tiempoInterrupcion = millis();  // variable almacena valor de func. millis

  if (tiempoInterrupcion - ultimaInterrupcion > 5) {  // rutina antirebote desestima
    // pulsos menores a 5 mseg.
    if (digitalRead(B) == HIGH)     // si B es HIGH, sentido horario
    {
      POSICION++ ;        // incrementa POSICION en 1
    }
    else {          // si B es LOW, senti anti horario
      POSICION-- ;        // decrementa POSICION en 1
    }

    POSICION = min(100, max(0, POSICION));  // establece limite inferior de 0 y
    // superior de 100 para POSICION
    ultimaInterrupcion = tiempoInterrupcion;  // guarda valor actualizado del tiempo
  }           // de la interrupcion en variable static
}
