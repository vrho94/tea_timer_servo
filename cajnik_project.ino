#include <stdlib.h>
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
//#include "eeprom_timer.h"
/*
  pini:
  enkoder: modra gnd,zelena 3.3v,rumena DT, oranzna clk
*/
//encoder***************
#define outputA 6
#define outputB 7
//int counter = 0;
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
const int gumb = 9; // the number of the pushbutton pin
//naslovi
int timer_size_addr_eeprom_address = 0;
int servo_max_eeprom_address = 1;
int servo_min_eeprom_address = 2;
//incializacija
int max_pos = EEPROM.read(servo_max_eeprom_address);
int min_pos = EEPROM.read(servo_min_eeprom_address);
int timer_size = EEPROM.read(timer_size_addr_eeprom_address);
//oled
Ssd1306Console console;
void setup()
{
  //oled
  ssd1306_128x64_i2c_init();
  ssd1306_fillScreen(0x00);
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  ssd1306_printFixed(0, 8, "Cajnik", STYLE_NORMAL);
  //encoder
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  aLastState = digitalRead(outputA);
  //uart
  Serial.begin(9600); //ne rabs!!!samo za debugging
  //servo pin:
  servo.attach(servo_pin);
  //gumb
  pinMode(gumb, INPUT_PULLUP);
}

//da se ne ukvarjas s prekinitvami uporab nizko vrednost za sestevalnika za eno sekundo... primer delay na 10ms(1000/10=100 proženj)
int gumb_stanje;
int meni_program = 0; //0-proži timer,1-nastavi čas,2-nastavi višino
int meni_program_old = meni_program;
void loop()
{
  //pogledaš ali se je spremenila vrednost meni_program
  meni_program = read_encoder_state(meni_program);
  if (meni_program > 2)
    meni_program = 2;
  else if (meni_program < 1)
    meni_program = 0;
  //ali je stara vrednost enaka trenutni?
  if (meni_program_old != meni_program)
  {                                  //ne
    meni_program_old = meni_program; //zapisi novo vrednost
    //prestavi na ustrezen program
    ssd1306_fillScreen(0x00);
    switch (meni_program)
    {
    case 0:
      ssd1306_printFixedN(0, 8, "Prozi timer:", STYLE_BOLD, 0.2);
      break;
    case 1:
      ssd1306_printFixedN(0, 8, "Nastavi cas", STYLE_BOLD, 0.2);
      break;
    case 2:
      ssd1306_printFixedN(0, 8, "Nastavi visino", STYLE_BOLD, 0.2);
      break;
    default:
      ssd1306_printFixedN(0, 8, "default", STYLE_BOLD, 0.2);
      break;
    }
    delay(100);
  }
  if (digitalRead(gumb) == LOW)
  {
    delay(200);
    switch (meni_program)
    {
    case 0:
      ssd1306_printFixedN(0, 8, "Prozi timer:", STYLE_BOLD, 0.2);
      break;
    case 1:
      set_timer_size();
      break;
    case 2:
      servo_hight_adjustment();
      break;
    default:
      ssd1306_printFixedN(0, 8, "Error", STYLE_BOLD, 0.2);
      break;
    }
  }
  //set_timer_size();
  //servo_hight_adjustment();
}

void servo_hight_adjustment()
{
  if (digitalRead(gumb) == LOW)
  {
    char num_str_buf[4];
    int buff_num;
    ssd1306_fillScreen(0x00);
    ssd1306_printFixedN(0, 8, "Nastavi visino(MAX):", STYLE_BOLD, 0.2);
    servo.write(max_pos);
    while (digitalRead(gumb) == LOW)
      ;
    delay(200);
    while (digitalRead(gumb) == HIGH)
    {
      set_servo_max_height();
    }
    ssd1306_fillScreen(0x00);
    ssd1306_printFixedN(0, 8, "Nastavi visino(MIN):", STYLE_BOLD, 0.2);
    servo.write(min_pos);
    while (digitalRead(gumb) == LOW)
      ;
    delay(200);
    while (digitalRead(gumb) == HIGH)
    {
      set_servo_min_height();
    }
    ssd1306_fillScreen(0x00);
    ssd1306_printFixedN(0, 8, "Izhod VS test!", STYLE_BOLD, 0.2);
    ssd1306_printFixed(40, 16, itoa(max_pos, num_str_buf, 10), STYLE_NORMAL);
    ssd1306_printFixed(40, 32, itoa(min_pos, num_str_buf, 10), STYLE_NORMAL);
    Serial.print(max_pos);

    while (digitalRead(gumb) == LOW)
      ;
    delay(200);
  }
}

void set_timer_size()
{
  char num_str_buf[4];
  int timer_old_buff = timer_size;
  if (digitalRead(gumb) == LOW)
  {
    ssd1306_fillScreen(0x00);
    ssd1306_printFixedN(0, 8, "Nastavi cas(min):", STYLE_BOLD, 0.2);
    while (digitalRead(gumb) == LOW)
      ; //release the button
    delay(200);
    while (digitalRead(gumb) == HIGH)
    {
      timer_size = read_encoder_state(timer_size);
      if (timer_old_buff != timer_size)
      {
        timer_old_buff = timer_size;
        ssd1306_printFixed(40, 16, "    ", STYLE_NORMAL);
        ssd1306_printFixed(40, 16, itoa(timer_size, num_str_buf, 10), STYLE_NORMAL);
      }

      // ssd1306_printFixed (40,  16, itoa(timer_size, num_str_buf, 4), STYLE_NORMAL);
    }
    ssd1306_printFixedN(0, 8, "Izhod!", STYLE_BOLD, 0.2);
    ssd1306_printFixed(0, 16, "MAX:", STYLE_NORMAL);
    ssd1306_printFixed(0, 32, "MIN:", STYLE_NORMAL);
    //    ssd1306_printFixed (40,  16, itoa(max_pos, num_str_buf, 4), STYLE_NORMAL);
    //   ssd1306_printFixed (40,  32, itoa(min_pos, num_str_buf, 4), STYLE_NORMAL);

    while (digitalRead(gumb) == LOW)
      ;
    delay(200);
  }
  //če je stanje drugačno kot prej preglej ali se timer_size povečuje ali manjša
  //se prav da je a signal pred bjom-se veča in obratno se man
}

int read_encoder_state(int counter)
{
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (aState != aLastState)
  {
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(outputB) != aState)
    {
      if (counter < 0)
        counter = 0;
      else
        counter++;
    }
    else
    {
      if (counter > 180)
        counter = 180;
      else
        counter--;
    }
  }
  aLastState = aState; // Updates the previous state of the outputA with the current state
  return counter;
}
//funkciji za nastavljanje visine
void set_servo_max_height()
{
  if (max_pos < 1)
    max_pos = 0;
  if (max_pos > 180)
    max_pos = 180;
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (aState != aLastState)
  {
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(outputB) != aState)
    {
      max_pos++;
    }
    else
    {
      max_pos--;
    }
    servo.write(max_pos);
    Serial.print("max_pos: ");
    Serial.println(max_pos);
    //ssd1306_drawProgressBar( counter/1.5 );
  }
  aLastState = aState; // Updates the previous state of the outputA with the current state
}

void set_servo_min_height()
{
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (min_pos < 1)
    min_pos = 0;
  if (min_pos > 180)
    min_pos = 180;
  if (aState != aLastState)
  {
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(outputB) != aState)
    {
      min_pos++;
    }
    else
    {
      min_pos--;
    }
    servo.write(min_pos);
    Serial.print("min_pos: ");
    Serial.println(min_pos);
    //ssd1306_drawProgressBar( counter/1.5 );
  }
  aLastState = aState; // Updates the previous state of the outputA with the current state
}

void write_number_to_string_on_oled(int x, int y, int num, char string[])
{
  ssd1306_printFixed(x, y, itoa(num, string, 4), STYLE_NORMAL);
}
