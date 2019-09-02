//Se utilizaron los siguientes tutoriales para realizar este código, en especial el del segundo link.
//https://www.rinconingenieril.es/como-usar-un-termistor-ntc/
//http://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/

int TermistorPin = A0; //Pin en donde leemos el voltaje proveniente del termistor.
int Vo;                //Variable para el voltaje del termistor.
float R1 = 10000;      //Resistencia que se está utilizando para el divisor de tensión.
float logR2, R2, T, Tc, Tf; //Variables utilizadas para que todo sea mas claro.
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07; //Coeficientes del termistor utilizado.

void setup() {
Serial.begin(9600);
}

void loop() {

  Vo = analogRead(TermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  //Tf = (Tc * 9.0)/ 5.0 + 32.0; 

  Serial.print("Temperatura: "); 
  Serial.print(Tc);
  Serial.println(" °C");   

  delay(500);
}
