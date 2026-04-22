#include <SPI.h>
#include <SdFat.h>

const int volDownPin=16;
const int sdPin = SDCARD_SS_PIN;

SdFat sd;
SdFile root;
File myFile;
String rootPath = "/";
String currentDir="";

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
  enterDir("test");
  Serial.println("setup - currentDir: " + currentDir);
  enterDir("test1_1");
  Serial.println("setup - currentDir: " + currentDir);
  leaveCurrentDir();
  leaveCurrentDir();
  // re-open the file for reading:
  if (!myFile.open("index.txt", O_READ)) {
    sd.errorHalt("opening test.txt for read failed");
  }
  Serial.println("index.txt:");

  // read from the file until there's nothing else in it:
  int data;
  while ((data = myFile.read()) >= 0) {
    Serial.write(data);
  }
  // close the file:
  myFile.close();
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

bool enterDir(String dirName){
  String newDir = currentDir + "/" + dirName;
  Serial.println("dirname: " + dirName);
  Serial.println("currentDir: " + currentDir);
  currentDir = newDir;
  return sd.chdir(dirName);
}

bool leaveCurrentDir(){
  int lastSlash = currentDir.lastIndexOf("/");
  String target = "";
  if(lastSlash == 0){
    currentDir = "/";
  } else {
    currentDir = currentDir.substring(0, lastSlash);
  }
  Serial.println("currentDir: " + currentDir);
  return sd.chdir(currentDir);
}

// Button Actions
bool buttonMenuAction(){
  sd.chdir();
  return true;
}

bool buttonVolumeUpAction(){
  return true;
}

bool buttonVolumeDownAction(){
  return true;
}

bool buttonNextAction(){
  return true;
}

bool buttonPrevAction(){
  return true;
}

bool buttonStartPauseAction(){
  return true;
}

bool buttonSelectAction(){
  return true;
}

bool buttonStandbyAction(){
  return true;
}
