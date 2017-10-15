#include < Esplora.h >
#include < TFT.h >
#include < SPI.h >

  enum states {
    HOME_SEL_TEMP,
    HOME_SEL_MIC,
    HOME_SEL_ACCE,
    TEMP,
    MIC,
    ACCE
  };
enum states STATE, NEXT_STATE;
char tempPrintout[3];
unsigned long entered;

int lasttemp = 0;

#
define DOWN SWITCH_1# define UP SWITCH_4# define ENTER SWITCH_2# define BACK SWITCH_3

byte buttonFlag = 0;

void setup() {
  Serial.begin(9600);

  // initialize the LCD
  EsploraTFT.begin();

  // start out with a white screen
  EsploraTFT.background(0, 0, 0);
  EsploraTFT.stroke(255, 255, 255);
  EsploraTFT.setTextSize(2);

  STATE = HOME_SEL_TEMP;
  display_menu(); // zobrazení domovské obrazovky
}

void loop() {
  switch (STATE) {
  case HOME_SEL_TEMP: // MENU - TEMPERATURE
    if (buttonEvent(DOWN)) // je stisknuto tlačítko 'pohyb dolu'?
    {
      NEXT_STATE = HOME_SEL_MIC;
      display_menu(); // změna domovské obrazovky
    } else if (buttonEvent(ENTER)) // je stisknuto tlačítko 'vybrat senzor'?
    {
      NEXT_STATE = TEMP;
      draw_temp();
    }
    break;

  case HOME_SEL_MIC: // MENU - MICROPHONE
    if (buttonEvent(DOWN)) // je stisknuto tlačítko 'pohyb dolu'?
    {
      NEXT_STATE = HOME_SEL_ACCE;
      display_menu(); // změna domovské obrazovky
    } else if (buttonEvent(UP)) // je stisknuto tlačítko 'pohyb nahoru'?
    {
      NEXT_STATE = HOME_SEL_TEMP;
      display_menu(); // změna domovské obrazovky
    } else if (buttonEvent(ENTER)) // je stisknuto tlačítko 'vybrat senzor'?
    {
      NEXT_STATE = MIC;
      draw_mic();
    }
    break;

  case HOME_SEL_ACCE: // MENU - ACCELEROMETER
    if (buttonEvent(UP)) // je stisknuto tlačítko 'pohyb nahoru'?
    {
      NEXT_STATE = HOME_SEL_MIC;
      display_menu(); // změna domovské obrazovky
    } else if (buttonEvent(ENTER)) // je stisknuto tlačítko 'vybrat senzor'?
    {
      NEXT_STATE = ACCE;
      draw_acce();
    }
    break;

  case TEMP: // DRAW TEMPERATURE
    if (buttonEvent(BACK)) // je stisknuto tlačítko 'zpět do menu'?
    {
      NEXT_STATE = HOME_SEL_TEMP;
      display_menu(); // zobrazení domovské obrazovky
    } else {
      draw_temp(); // funkce draw_temp vykreslí aktuální teplotu každé 2 sekundy
    }
    break;

  case MIC: // DRAW MICROPHONE
    if (buttonEvent(BACK)) // je stisknuto tlačítko 'zpět do menu'?
    {
      NEXT_STATE = HOME_SEL_MIC;
      display_menu(); // zobrazení domovské obrazovky
    } else {
      draw_mic(); // funkce draw_mic vykreslí křivku podle momentálního zvuku z mikrofonu
    }
    break;

  case ACCE: //  DRAW ACCELEROMETER
    if (buttonEvent(BACK)) // je stisknuto tlačítko 'zpět do menu'?
    {
      NEXT_STATE = HOME_SEL_ACCE;
      display_menu(); // zobrazení domovské obrazovky
    } else {
      draw_acce(); // funkce draw_acce zobrazí kolečko, které se bude pohybovat v závislosti na náklonu Esplory
    }
    break;
  }
  STATE = NEXT_STATE;
}

void display_menu() {
  EsploraTFT.background(0, 0, 0);
  int a = 0;
  int b = 0;
  int c = 0;
  switch (NEXT_STATE) {
  case HOME_SEL_TEMP:
    a = 15;
    EsploraTFT.text(">", 0, 0);
    break;

  case HOME_SEL_MIC:
    b = 15;
    EsploraTFT.text(">", 0, 20);
    break;

  case HOME_SEL_ACCE:
    c = 15;
    EsploraTFT.text(">", 0, 40);
    break;
  }
  EsploraTFT.text("Show Temp", a, 0);
  EsploraTFT.text("Show Mic", b, 20);
  EsploraTFT.text("Show Acce", c, 40);
}

void draw_temp() {
  if (NEXT_STATE != STATE || millis() - entered > 2000) {

    String temperature(Esplora.readTemperature(DEGREES_C));
    temperature.toCharArray(tempPrintout, 3);
    EsploraTFT.background(0, 0, 0);
    EsploraTFT.text("Stupne Celsia:\n ", 0, 0);
    EsploraTFT.text(tempPrintout, 0, 30);
    entered = millis();
  }
}

int xPos = 0;
void draw_mic() {
  int sensor = Esplora.readMicrophone();
  int graphHeight = map(sensor, 0, 1023, 25, EsploraTFT.height());

  EsploraTFT.line(xPos, EsploraTFT.height() - graphHeight, xPos, EsploraTFT.height());

  if (xPos >= 160) {
    xPos = 0;
    EsploraTFT.background(0, 0, 0);
  } else {

    xPos++;
  }

  delay(16);

  //EsploraTFT.stroke(0,0,0);
}

int oldEndY;
int oldStartY;
int yStart = EsploraTFT.height() / 2;
int yEnd = EsploraTFT.height() / 2;

void draw_acce() {
  EsploraTFT.background(0, 0, 0);
  int tilt = Esplora.readAccelerometer(X_AXIS);

  yStart = map(tilt, -100, 100, EsploraTFT.height(), 0);
  yEnd = map(tilt, -100, 100, 0, EsploraTFT.height());

  if (oldStartY != yStart || oldEndY != yEnd) {
    EsploraTFT.stroke(0, 0, 0);
    EsploraTFT.line(0, oldStartY, EsploraTFT.width(), oldEndY);
  }

  EsploraTFT.stroke(255, 0, 255);
  EsploraTFT.line(0, yStart, EsploraTFT.width(), yEnd);

  oldStartY = yStart;
  oldEndY = yEnd;
  delay(10);
}

bool buttonEvent(int button) {
  switch (button) {
  case UP:
    if (Esplora.readButton(UP) == LOW) {
      buttonFlag |= 1;
    } else if (buttonFlag & 1) {
      buttonFlag ^= 1;
      return true;
    }
    break;

  case DOWN:
    if (Esplora.readButton(DOWN) == LOW) {
      buttonFlag |= 2;
    } else if (buttonFlag & 2) {
      buttonFlag ^= 2;
      return true;
    }
    break;

  case BACK:
    if (Esplora.readButton(BACK) == LOW) {
      buttonFlag |= 4;
    } else if (buttonFlag & 4) {
      buttonFlag ^= 4;
      return true;
    }
    break;

  case ENTER:
    if (Esplora.readButton(ENTER) == LOW) {
      buttonFlag |= 8;
    } else if (buttonFlag & 8) {
      buttonFlag ^= 8;
      return true;
    }
  }
  return false;
}