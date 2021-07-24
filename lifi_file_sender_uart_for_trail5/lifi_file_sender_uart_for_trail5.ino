#include "FS.h"
#include "SPIFFS.h"
short int led=4;
short int indicator_led=2;
short int step_time=400;
//short int bin[30];
double val,power;
int count=0;
double start;
unsigned short int delay_time;
#define FORMAT_SPIFFS_IF_FAILED true
void send_light()
{
  //delay_time=micros()-start;
  delay_time=step_time;//-delay_time;
  delayMicroseconds(delay_time);
  digitalWrite(led,LOW);
  delayMicroseconds(step_time);
  //start=micros();
  for(int i=0;i<8;i++)
  {
   power=pow(2,7-i);
   if(val>=power)
   {
    digitalWrite(led,HIGH);
    val=val-power;
   }
   else
   {
    digitalWrite(led,LOW);
   }
   //delay_time=micros()-start;
   delay_time=step_time;//-delay_time;
   delayMicroseconds(delay_time);
   //start=micros();
  }
  digitalWrite(led,HIGH);
}
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
            readFile(SPIFFS,file.name());
            delay(1000);
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path,FILE_READ);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }
    Serial.println("Ready to be sent waiting for start signal :");
    while(Serial.read()<=0);
    Serial.println("starting to send data");
    digitalWrite(indicator_led,LOW);
    start=micros();
    while(file.available()){
      val=file.read();
      send_light();
      count++;
      }
    digitalWrite(indicator_led,HIGH);
    Serial.print("finised sending bytes ");
    Serial.println(count);
    count=0;
    file.close();
}


void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}
void setup() {
      Serial.begin(115200);
    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    pinMode(led,OUTPUT);
    pinMode(indicator_led,OUTPUT);
    digitalWrite(led,HIGH);
    //digitalWrite(indicator_led,HIGH);
    deleteFile(SPIFFS, "/test.txt");
    
    listDir(SPIFFS, "/", 0);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
