void setup() {
  // put your setup code here, to run once:
  pinMode(9, OUTPUT); // INPUT1
  pinMode(10, OUTPUT); // INPUT2
  pinMode(8, OUTPUT); // Enable


}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(8, HIGH); //Enable
  digitalWrite(9, LOW);   //Rojo
  digitalWrite(10, HIGH); //Negro
  delay(5000);

  digitalWrite(10, LOW);  //Negro
  digitalWrite(9, HIGH);  //Rojo
  digitalWrite(8, HIGH); //Enable
  delay(5000);
}
