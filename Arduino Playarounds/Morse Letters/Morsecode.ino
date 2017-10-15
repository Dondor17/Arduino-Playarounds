#include <Esplora.h>

  char * letters[] = {
    ".-",
    "-...",
    "-.-.",
    "-..",
    ".",
    "..-.",
    "--.", // A-G
    "....",
    "..",
    ".---",
    "-.-",
    ".-..",
    "--",
    "-.", // H-N
    "---",
    ".--.",
    "--.-",
    ".-.",
    "...",
    "-",
    "..-", // O-U
    "...-",
    ".--",
    "-..-",
    "-.--",
    "--.." // V-Z
  };
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}
void loop() {
  char haha = tolower(Serial.read());
  int i =  haha - 'a';
  if (i != -98) {
    //metoda
    work(i);
  }
}
void flash(int doba) {
  Esplora.writeRGB(0, 255, 0);
  delay(doba);
  Esplora.writeRGB(0, 0, 0);
  delay(200);
}
void work(int i) {
  char * blik = letters[i];
  for (int i = 0; i < strlen(blik); i++) {
    if (blik[i] == '.') {
      flash(200);
    }
    if (blik[i] == '-') {
      flash(800);
    }
    if (blik[i] == ' ') {
      flash(1000);
    }
  }
}
