#include <Esplora.h>
#include <SPI.h>
#include <TFT.h>

char tempCelsius[3];


void setup()
{
  EsploraTFT.begin();
  EsploraTFT.background(0, 0, 0);
  EsploraTFT.stroke(255, 255, 255);
}

void loop()
{
  String celsius = String(Esplora.readTemperature(DEGREES_C));
  celsius.toCharArray(tempCelsius, 3);
  EsploraTFT.stroke(255, 255, 255);
  EsploraTFT.text(tempCelsius, 0, 0);
  delay(500);
  EsploraTFT.stroke(0, 0, 0);
  EsploraTFT.text(tempCelsius, 0, 0);

}
