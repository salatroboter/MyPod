#include <WiFiNINA.h>
#include <utility/wifi_drv.h>
#include <SPI.h>
#include <SD.h>

int bluePin=27;
int redPin=26;
int greenPin=25;
int volDownPin=0;

int volDownButtonState = 0;
bool buttonPressed=false;

File root;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);

  Serial.print("Initialising SD reader ... ");
  if(!SD.begin(10)){
    Serial.println("error");
    while(true);
  }
  root = SD.open("/");
  Serial.println("sucess!");
  pinMode(volDownPin, INPUT);
  WiFiDrv::pinMode(greenPin, OUTPUT);
  WiFiDrv::pinMode(redPin, OUTPUT);
  WiFiDrv::pinMode(bluePin, OUTPUT);
}

void loop() {
  volDownButtonState = digitalRead(volDownPin);
  if(volDownButtonState == HIGH){
    Serial.println("down");
    
  }
  Serial.println("up");
  // put your main code here, to run repeatedly:
  //ledBlinkOnce(1000, bluePin);
  //ledBlinkOnce(1000, redPin);
  //ledBlinkOnce(1000, greenPin);
}

void ledBlinkOnce(int time, int pin){
  WiFiDrv::digitalWrite(pin, HIGH); //Turn On Green light
  delay(time);
  WiFiDrv::digitalWrite(pin, LOW); //Turn Off Green light
  delay(time);
}
void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more file
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}