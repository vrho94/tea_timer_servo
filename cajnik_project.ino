#include <SeeedOLED.h>//grove lib-glej example!mogoce rabs se i2c inicializacijo in njeno kniznco
#include <Servo.h>
#include <EEPROM.h>
#include <Wire.h>//i2c lib
//encoder***************
#define outputA 6
#define outputB 7
int counter = 0;
int aState;
int aLastState;
//encoder end*********
//servo+++++++++++++
#define servo_pin 9
Servo servo;
int position_of_servo = 0;
int *ptr_of_servo_pos = &position_of_servo;
//servo end++++++++++
int timer_addr_eeprom = 0; //naslov
void setup() {
  //i2c in oled
  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display
  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("Cajnik!"); //Print the String
  //encoder
  pinMode (outputA, INPUT);
  pinMode (outputB, INPUT);
  aLastState = digitalRead(outputA);
  //uart
  Serial.begin(9600);
  //servo pin:
  servo.attach(servo_pin);
}

//da se ne ukvarjas s prekinitvami uporab nizko vrednost za sestevalnika za eno sekundo... primer delay na 10ms(1000/10=100 proženj)

void loop() {
  read_encoder_state();
}


void servo_position_set(int position) {
  //uporabljena funkcija pri odštevanju časa in pa nastavitev_visine
  servo.write(position);
}

void servo_hight_adjustment() {
  //enako kot spodaj
}

void set_timer() {
  //če je stanje drugačno kot prej preglej ali se timer povečuje ali manjša
  //se prav da je a signal pred bjom-se veča in obratno se man
}

void read_encoder_state() {
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (aState != aLastState) {
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(outputB) != aState) {
      if (counter < 254)counter ++;
    } else {
      if (counter > 1)counter --;
    }
    Serial.print("Position: ");
    Serial.println(counter);
    servo_position_set(counter);
  }
  aLastState = aState; // Updates the previous state of the outputA with the current state

}
