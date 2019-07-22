#include <SoftwareSerial.h>
#include <DFMiniMp3.h>
 
volatile int buttonState = 0; 
int state=0;
int buttonpressed=0;
int i;
 
class Mp3Notify
{
public:
  static void OnError(uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }
 
  static void OnPlayFinished(uint16_t globalTrack)
  {
    Serial.println();
    Serial.print("Play finished for #");
    Serial.println(globalTrack);  
    state=0;
  }
 
  static void OnCardOnline(uint16_t code)
  {
    Serial.println();
    Serial.print("Card online ");
    Serial.println(code);    
  }
 
  static void OnCardInserted(uint16_t code)
  {
    Serial.println();
    Serial.print("Card inserted ");
    Serial.println(code);
  }
 
  static void OnCardRemoved(uint16_t code)
  {
    Serial.println();
    Serial.print("Card removed ");
    Serial.println(code); 
  }
};
 
SoftwareSerial secondarySerial(11, 10); // RX, TX
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(secondarySerial);
 
void setup()
{
  Serial.begin(115200);
  Serial.println("initializing...");
  mp3.begin();
  uint16_t volume = mp3.getVolume();
  mp3.setVolume(18);
  uint16_t count = mp3.getTotalTrackCount();
  Serial.print("files ");
  for (i=2;i<=6;i++)
   {
     pinMode(i, INPUT_PULLUP);digitalWrite(i,1);
   }
  mp3.stop();
 
}
 
void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();
  while ((millis() - start) < msWait)
  {
    mp3.loop();
    delay(1);
  }
}
 
void loop()
{
   for (i=2;i<=6;i++)
   {
     if (digitalRead(i) == 0)
    {
     buttonpressed=i;
     waitMilliseconds(600);
    }  
   }
  
  if (buttonpressed==state && state>0)
   {
    Serial.print("Button pressed stop ");
    Serial.println(buttonpressed);
    mp3.stop();
    state=0;
    buttonpressed=0;
   };
  
   if (buttonpressed>0)
   {
    Serial.print("Button pressed start number ");
    Serial.println(buttonpressed);   
    mp3.playFolderTrack(1,buttonpressed-1);
    state=buttonpressed;
    buttonpressed=0;
   }
  
   mp3.loop();
}