#include <SPI.h>
#include <SdFat.h>

const int volDownPin=16;
const int sdPin = SDCARD_SS_PIN;

SdFat sd;
SdFile root;
File myFile;
String rootPath = "/";

void setup() {

  Serial.begin(9600);
  while(!Serial);
  
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!sd.begin(sdPin)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  pinMode(volDownPin, INPUT);
  Serial.println("indexing...");
  processDirectory("/");
  Serial.println("Done");
}

void loop() {
  int volDownButtonState = digitalRead(volDownPin);
  int read = analogRead(A1);
  float voltage = read;

  //AudioEngine

  //InputManager

  //PlayerState

}

void processDirectory(const char* path) {
  FsFile dir;
  if (!dir.open(path)) {
    Serial.print("Failed to open dir: ");
    Serial.println(path);
    return;
  }

  // build index file path
  char indexPath[128];
  if (strcmp(path, "/") == 0) {
    snprintf(indexPath, sizeof(indexPath), "/index.txt");
  } else {
    snprintf(indexPath, sizeof(indexPath), "%s/index.txt", path);
  }

  FsFile indexFile;
  if (!indexFile.open(indexPath, O_WRITE | O_CREAT | O_TRUNC)) {
    Serial.print("Failed to create: ");
    Serial.println(indexPath);
    dir.close();
    return;
  }

  FsFile file;

  while (file.openNext(&dir, O_RDONLY)) {
    char name[64];
    file.getName(name, sizeof(name));

    // skip index file itself
    if (strcmp(name, "index.txt") == 0) {
      file.close();
      continue;
    }

    if (file.isDirectory()) {
      // write directory name with '/'
      indexFile.print(name);
      indexFile.println("/");

      // build subdirectory path
      char subPath[128];
      if (strcmp(path, "/") == 0) {
        snprintf(subPath, sizeof(subPath), "/%s", name);
      } else {
        snprintf(subPath, sizeof(subPath), "%s/%s", path, name);
      }

      file.close();              // important!
      processDirectory(subPath); // recurse
    } else {
      // write file name
      indexFile.println(name);
      file.close();
    }
  }

  indexFile.close();
  dir.close();
}