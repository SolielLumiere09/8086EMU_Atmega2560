#include <Arduino.h>
#include <CPU.h>

CPU *cpu;

void setup() {
  Serial.begin(19200); //Serial comunications
  XMCRA = 1 << SRE; //enables external memory RAM

  cpu = new CPU();




}

void loop() {
  cpu->update();
  delay(1000);
}