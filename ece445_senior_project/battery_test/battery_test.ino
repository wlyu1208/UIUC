/*
 * The following is a testing script for measuring the battery voltage.
 */

// 5.45 V -- > ~1505
// 4.92 V -- > ~1365
// ~264 per 1V

#define VOLTAGE_CONVERT 278.0

double analog2voltage(int input){
  return input/278.0;
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int volt_read = analogRead(1);
  Serial.print("input: ");
  Serial.println(volt_read);
  Serial.print("voltage: ");
  Serial.println(analog2voltage(volt_read));
  delay(200);
}
