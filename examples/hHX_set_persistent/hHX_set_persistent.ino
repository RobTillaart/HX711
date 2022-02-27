
//
//    FILE: HX_set_persistent.ino
//  AUTHOR: Felix Moli grao la base de Rob Tillaart
// PURPOSE: HX711 demo
//     URL: https://github.com/Albalate/HX711



//scale.set_offset(-186985);
//scale.set_scale(14.18);

#include "HX711.h"

#include "EEPROM.h"
int addr = 0;
#define EEPROM_SIZE 512

bool forzado = true; //indica que queremos entrar en modo calibración
HX711 scale;

uint8_t dataPin  = 19;
uint8_t clockPin = 18;

//scale.set_offset(-181815);
//scale.set_scale(13.79);

void StupEeprom() {
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("failed to initialise EEPROM"); delay(1000000);
  }
  Serial.println(" bytes read from Flash . Values are:");
  for (int i = 0; i < EEPROM_SIZE; i++)
  {
    Serial.print(byte(EEPROM.read(i))); Serial.print(" ");
  }
}
float SaveFloat(int addr, float param) {
  EEPROM.writeFloat(addr, param);//EEPROM.put(address, param);
  EEPROM.commit();
}
long SaveLong(int addr, long param) {
  EEPROM.writeFloat(addr, param);//EEPROM.put(address, param);
  EEPROM.commit();
}

float LoadFloat(int addr) {
  float readParam;
  EEPROM.get(addr, readParam); //readParam=EEPROM.readFloat(address);
  Serial.print("Read param = ");
  Serial.println(readParam);
  return readParam;
}
long LoadLong(int addr) {
  long readParam;
  EEPROM.get(addr, readParam); //readParam=EEPROM.readFloat(address);
  Serial.print("Read param = ");
  Serial.println(readParam);
  return readParam;
}

void setup()
{
  Serial.begin(115200);
  void StupEeprom();
  float escala = LoadFloat(0);
  long offsSet = LoadLong(100);
  scale.set_scale(escala); //lee escala de la posicio 0 de la eeprom
  scale.set_offset(offsSet); //lee offSet de la posicio 100 de la eeprom

  scale.begin(dataPin, clockPin);// inicia la comunicación

  if ((escala = 0.00) || (offsSet = 0) || (forzado = true)) {

    Serial.print("UNITS: ");
    Serial.println(scale.get_units(10));

    Serial.println("\nEmpty the scale, press a key to continue");
    while (!Serial.available());
    while (Serial.available()) Serial.read();

    scale.tare();
    Serial.print("UNITS: ");
    Serial.println(scale.get_units(10));


    Serial.println("\nPut 1000 gram in the scale, press a key to continue");
    while (!Serial.available());
    while (Serial.available()) Serial.read();

    scale.calibrate_scale(1000, 5);
    Serial.print("UNITS: ");
    Serial.println(scale.get_units(10));

    Serial.println("\nScale is calibrated, your calibration values:");

    long scaleOffset = scale.get_offset();
    Serial.print("\nOffset \t");
    Serial.println(scaleOffset);

    SaveLong( 100, scaleOffset);
    EEPROM.end();

    float scaleFactor = scale.get_scale();
    Serial.print("Scale \t");
    Serial.println(scaleFactor);

    SaveFloat( 0, scaleFactor);

    Serial.println("\nUse this code for setting zero and calibration factor permanently:");

    Serial.print("\nscale.set_offset(");
    Serial.print(scaleOffset);
    Serial.println(");");
    Serial.print("scale.set_scale(");
    Serial.print(scaleFactor);
    Serial.println(");");

    Serial.println("\nPress a key to continue");
    while (!Serial.available());
    while (Serial.available()) Serial.read();

    EEPROM.commit();
    EEPROM.end();
    ESP.restart();

  } else {
    Serial.println("La bascula esta calibrada... pulse para continuar");
    while (!Serial.available());
    while (Serial.available()) Serial.read();
  }
}
void loop()
{
  Serial.print("UNITS: ");
  Serial.println(scale.get_units(15));
  delay(250);
}
