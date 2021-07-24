#include<driver/adc.h>
#include<SD.h>
#include<FS.h>
#include<SPI.h>
unsigned short int lig,led_pin,sd_led_pin,high,stage,ticks,bytes;
unsigned short int inp[50000];
unsigned short int delaytime=359;
unsigned int count;
double start_time;
File file;
const char path[]="/data.txt";
short int light(unsigned short int del)
{
  //del=del*(micros()-start_time);
  del=del*delaytime;//delaytime-del;
  //start_time=micros();
  delayMicroseconds(del);
  return (4095-adc1_get_raw(ADC1_CHANNEL_0));
}
void setup() {
  count=0;
  led_pin=4;
  sd_led_pin=2;
  stage=1;
  ticks=40;
  bytes=0;
  pinMode(led_pin,OUTPUT);
  pinMode(sd_led_pin,OUTPUT);
  Serial.begin(115200);
  while(1)
  {
   if(!SD.begin())
   {
    Serial.println("Card Mount Failed");
   }
   else
   {
    Serial.println("mounted");
    digitalWrite(sd_led_pin,HIGH);
    //SD.remove(path);
    file = SD.open(path, FILE_WRITE);
    while(!file)
    {
     Serial.println("file openning failed trying to re open");
     file = SD.open(path, FILE_WRITE);
    }
    Serial.println("file opened and sdcard is ready ");
    break;
   }
  }
  while(1)
  {
    lig=light(0);
    if(lig==0)
    {
      Serial.println("check sensor "); 
    }
    else
    {
      break;
    }
  }
  Serial.print("hit send");
  //while(Serial.read()<1);
  int current;
  high=light(0);
  for(int i=0;i<20;i++)
  {
    current=light(0);
    if(high<current)
    {
      high=current;
    } 
  }
  if(high!=4095)
  {
    setup();
  }
  Serial.println("highest value found to be ");
  Serial.println(high);
  high=high-(high/2);
  //digitalWrite(led_pin,HIGH);
}

void loop() {
  switch (stage){
    case 1:
      for(int i=0;i<ticks;i++)
      {
        lig=light(0);
        if(lig<high)
        {
          delayMicroseconds(delaytime/2);
          if(light(0)<high)
          {
            stage=2;
            //digitalWrite(led_pin,LOW);
            break;
          }
        }
      }
      if (stage==1&& count>0)
      {
        stage=3;
      }
      break;
    case 2:
      for(int j=0;j<8;j++)
      {
        lig=light(1);
        if(lig>high)
        {
          bytes=(bytes*2)+1;
        }
        else
        {
          bytes=bytes*2;
        }
      }
      //file.write(bytes);
      inp[count]=bytes;
      bytes=0;
      
      count++;
      stage=1;
      break;
    case 3:
      //file.close();
      stage=4;
      Serial.print("recived bytes : ");
      Serial.println(count);
      for (int i=0;i<count;i++)
      {
        Serial.write(inp[i]);
        file.write(inp[i]);
       }
       
        file.close();
        Serial.println("file closed");
        digitalWrite(led_pin,HIGH);
        digitalWrite(sd_led_pin,HIGH);
      break;
      
  }
}
