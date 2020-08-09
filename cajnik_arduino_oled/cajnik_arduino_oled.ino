#include <SeeedOLED.h>


char sline_0[16];
char sline_1[16];
char sline_2[16];
char sline_3[16];
char sline_4[16];
char sline_5[16];
char sline_6[16];
char sline_7[16];
char
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("Moja Natalija !"); //Print the String
  SeeedOled.setTextXY(2, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("Moja Natalija !"); //Print the String
}
int index=0;
void loop() {
  
  while (Serial.available()) {
    delay(2);  //delay to allow byte to arrive in input buffer
    char c = Serial.read();
    if(index<16)sline_0[index] += c;
    if(index>16 && index < 32)sline_1[index] += c;
    if(index>=32 && index < 64)sline_2[index] += c;
    if(index>=64 && index < 128)sline_3[index] += c;
    if(index>=128 && index < 256)sline_4[index] += c;
    if(index>=256 && index < 512)sline_5[index] += c;
    if(index>=512 && index < 1024)sline_6[index] += c;
    if(index>=16 && index < 32)sline_7[index] += c;
    index++;
  }
  if(index>0){
    SeeedOled.clearDisplay();
    SeeedOled.setTextXY(0, 0);
    SeeedOled.putString(string);
    index=0;
  for(int i=0;i<sizeof(string);i++){
    sline_0[i]=0;
    sline_1[i]=0;
    sline_2[i]=0;
    sline_3[i]=0;
    sline_4[i]=0;
    sline_5[i]=0;
    sline_6[i]=0;
    sline_7[i]=0;
    
  }
    }


}
