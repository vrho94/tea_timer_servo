#include <ssd1306_uart.h>
#include <ssd1306_8bit.h>
#include <nano_gfx_types.h>
#include <nano_engine.h>
#include <ssd1306_16bit.h>
#include <nano_gfx.h>
#include <ssd1306.h>
#include <font6x8.h>
#include <ssd1306_console.h>
#include <ssd1306_generic.h>
#include <sprite_pool.h>
#include <ssd1306_1bit.h>
#include <ssd1306_fonts.h>

#include <Servo.h>
#include <EEPROM.h>

/*
  pini:
  enkoder: modra gnd,zelena 3.3v,rumena DT, oranzna clk
*/
//encoder***************
#define outputA 6
#define outputB 7
int counter = 0;
int aState;
int aLastState;
//encoder end*********
//servo+++++++++++++
#define servo_pin 8
Servo servo;
int position_of_servo = 0;
int *ptr_of_servo_pos = &position_of_servo;
//servo end++++++++++
//gumb
const int gumb = 9;     // the number of the pushbutton pin
int timer_addr_eeprom = 0; //naslov
void setup() {
  //oled
  ssd1306_128x64_i2c_init();
  ssd1306_fillScreen(0x00);
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  ssd1306_printFixed (0,  8, "Cajnik", STYLE_NORMAL);
  //encoder
  pinMode (outputA, INPUT);
  pinMode (outputB, INPUT);
  aLastState = digitalRead(outputA);
  //uart
  Serial.begin(9600);//ne rabs!!!samo za debugging
  //servo pin:
  servo.attach(servo_pin);
  //gumb
  //pinMode(gumb, INPUT);
}

//da se ne ukvarjas s prekinitvami uporab nizko vrednost za sestevalnika za eno sekundo... primer delay na 10ms(1000/10=100 proženj)
int gumb_stanje;
void loop() {
  read_encoder_state();
}


void servo_position_set(int position) {
  //uporabljena funkcija pri odštevanju časa in pa nastavitev_visine
  servo.write(position);
}

void servo_hight_adjustment() {
  if(digitalRead(gumb)==HIGH){
  ssd1306_printFixedN (0,  8, "Cajnik naklon vodila:", STYLE_BOLD,0.2);
  while(digitalRead(gumb)==HIGH);
  while(digitalRead(gumb)==LOW)read_encoder_state();
  ssd1306_printFixedN (0,  8, "Konc:", STYLE_BOLD,0.2);
  }
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
      if(counter<0)counter=0;
      else counter ++;
    } else {
      if(counter>180)counter=180;
      else counter --;
    }
    Serial.print("Position: ");
    Serial.println(counter);
    //ssd1306_drawProgressBar( counter/1.5 );
    servo_position_set(counter);
  }
  aLastState = aState; // Updates the previous state of the outputA with the current state
}
void write_number_to_string_on_oled(int x, int y,int num, char string[]) {
  ssd1306_printFixed (x,  y, itoa(num, string, 4), STYLE_NORMAL);
}
