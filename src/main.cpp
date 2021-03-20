#include <Arduino.h>
#include <CPU.h>

#define INT_PIN 21
CPU *cpu;


void updateCPU();

void setup() {
  Serial.begin(19200); //Serial comunications
  XMCRA = 1 << SRE; //enables external memory RAM

  cpu = new CPU();
  cpu->print_reg_status();
  pinMode(INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), &updateCPU, RISING);

}

void loop() {
  
  
}

void updateCPU(){
  cpu->update();
  cpu->print_reg_status();
}