#include "secrets.h"
#include "whatsapp.h"

void setup() {
  Serial.begin(9600);
  connectWIFI();
  sendWhatsapp();
}

void loop() {
}
