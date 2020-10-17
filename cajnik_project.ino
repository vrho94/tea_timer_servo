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
int timer_size_min_addr_eeprom_address = 0;
int timer_size_sec_addr_eeprom_address = 3;
int servo_max_eeprom_address = 1;
int servo_min_eeprom_address = 2;
//incializacija
int max_pos = EEPROM.read(servo_max_eeprom_address);
int min_pos = EEPROM.read(servo_min_eeprom_address);
int timer_size_min = EEPROM.read(timer_size_min_addr_eeprom_address);
int timer_size_sec = EEPROM.read(timer_size_sec_addr_eeprom_address);
bool poz_up=0;
bool poz_down=0;
//oled
Ssd1306Console console;
//icons
const unsigned char tea_cup_32_32[] PROGMEM = {
    // 'tea-icon-32-32, 32x32px
    0xff, 0xfe, 0xf7, 0xff, 0xff, 0xde, 0xff, 0xff, 0xff, 0xdf, 0xef, 0xff, 0xff, 0xdf, 0xef, 0xff,
    0xff, 0xdf, 0xef, 0xff, 0xff, 0xde, 0xff, 0xff, 0xff, 0xee, 0xf7, 0xff, 0xff, 0xef, 0x7f, 0xff,
    0xff, 0xef, 0x7f, 0xff, 0xff, 0xef, 0x77, 0xff, 0xff, 0xef, 0xf7, 0xff, 0xff, 0xfe, 0xf7, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xfd, 0xef, 0xf7, 0xff, 0xff, 0xef,
    0xf7, 0xff, 0xff, 0xe7, 0xf7, 0xff, 0xff, 0xeb, 0xf7, 0xff, 0xff, 0xeb, 0xf7, 0xff, 0xff, 0xef,
    0xff, 0xff, 0xff, 0xef, 0xfb, 0xff, 0xff, 0xdb, 0xfb, 0xff, 0xff, 0xdb, 0xfd, 0xff, 0xff, 0x87,
    0xfd, 0xff, 0xff, 0x3f, 0xfe, 0xff, 0xff, 0x7f, 0xff, 0x3f, 0xfc, 0xff, 0xff, 0xdf, 0xf3, 0xff,
    0xf8, 0x00, 0x00, 0x1f, 0xf7, 0xff, 0xff, 0xef, 0xf7, 0xff, 0xff, 0xef, 0xf8, 0x00, 0x00, 0x1f};
void setup()
{
  //oled
  ssd1306_128x64_i2c_init();
  ssd1306_fillScreen(0x00);
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  ssd1306_printFixed2x(0, 8, "Cajnik", STYLE_NORMAL);
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
  //test----------------------------------------------------
  /*while (1)
  {
    move_servo_down_speed(30);
    delay(500);
    move_servo_up_speed(30);
    delay(500);
  }*/
  //test----------------------------------------------------
  if (timer_size_sec > 59)
    timer_size_sec = 59;
  move_servo_up_speed(30);
}

//da se ne ukvarjas s prekinitvami uporab nizko vrednost za sestevalnika za eno sekundo... primer delay na 10ms(1000/10=100 proženj)
int gumb_stanje;
int meni_program = 0; //0-proži timer,1-nastavi čas,2-nastavi višino
int meni_program_old = meni_program;
char menu_number_val_buff_oled[4];
void loop()
{
  //pogledaš ali se je spremenila vrednost meni_program
  meni_program = read_encoder_state(meni_program);
  if (meni_program > 1)
    meni_program = 2;
  if (meni_program < 1)
    meni_program = 0;
  //ali je stara vrednost enaka trenutni?
  if (meni_program_old != meni_program)
  {                                  //ne
    meni_program_old = meni_program; //zapisi novo vrednost
    //prestavi na ustrezen program
    ssd1306_fillScreen(0x00);
    switch (meni_program) ///////////////////////////////////////////////////////////////////////////////////////
    {
    case 0:
      ssd1306_printFixed(0, 0, "Prozi timer:", STYLE_NORMAL);
      break;
    case 1:
      ssd1306_printFixedN(0, 8, "Nastavi cas:", STYLE_NORMAL, 0.2);
      ssd1306_printFixedN(25, 16, " min", STYLE_NORMAL, 0.2);
      ssd1306_printFixedN(10, 16, itoa(timer_size_min, menu_number_val_buff_oled, 10), STYLE_NORMAL, 0.2);
      ssd1306_printFixedN(65, 16, " s", STYLE_NORMAL, 0.2);
      ssd1306_printFixedN(55, 16, itoa(timer_size_sec, menu_number_val_buff_oled, 10), STYLE_NORMAL, 0.2);
      break;
    case 2:
      ssd1306_printFixed(0, 8, "Nastavi visino", STYLE_NORMAL);
      ssd1306_printFixed(40, 16, itoa((max_pos * 100 / 180), menu_number_val_buff_oled, 10), STYLE_NORMAL);
      ssd1306_printFixed(40, 32, itoa((min_pos * 100 / 180), menu_number_val_buff_oled, 10), STYLE_NORMAL);
      ssd1306_printFixedN(60, 16, "%", STYLE_NORMAL, 0.2);
      ssd1306_printFixedN(60, 32, "%", STYLE_NORMAL, 0.2);
      break;
    default:
      ssd1306_printFixedN(0, 8, "Napaka!", STYLE_NORMAL, 0.2);
      break;
    }
  }
  if (digitalRead(gumb) == LOW)
  {
    delay(200);
    switch (meni_program) //////////////////////////////////////////////////
    {
    case 0:
      ssd1306_fillScreen(0x00);
      start_timer();
      //move_servo_up_speed(30);//je v funkciji
      break;
    case 1:
      set_timer_size_min();
      ssd1306_fillScreen(0x00);
      ssd1306_printFixedN(0, 8, "Nastavi cas:", STYLE_NORMAL, 0.2);
      ssd1306_printFixedN(25, 16, " min", STYLE_NORMAL, 0.2);
      ssd1306_printFixedN(10, 16, itoa(timer_size_min, menu_number_val_buff_oled, 10), STYLE_NORMAL, 0.2);
      ssd1306_printFixedN(65, 16, " s", STYLE_NORMAL, 0.2);
      ssd1306_printFixedN(55, 16, itoa(timer_size_sec, menu_number_val_buff_oled, 10), STYLE_NORMAL, 0.2);
      break;
    case 2:
      servo_hight_adjustment();
      ssd1306_fillScreen(0x00);
      ssd1306_printFixedN(0, 8, "Nastavi visino", STYLE_NORMAL, 0.2);
      ssd1306_printFixed(40, 16, itoa((max_pos * 100 / 180), menu_number_val_buff_oled, 10), STYLE_NORMAL);
      ssd1306_printFixed(40, 32, itoa((min_pos * 100 / 180), menu_number_val_buff_oled, 10), STYLE_NORMAL);
      ssd1306_printFixedN(60, 16, "%", STYLE_NORMAL, 0.2);
      ssd1306_printFixedN(60, 32, "%", STYLE_NORMAL, 0.2);
      move_servo_up_speed(30); //dvigne pozicijo servota nazaj na visoko
      break;
    default:
      ssd1306_fillScreen(0x00);
      ssd1306_printFixedN(0, 8, "Error", STYLE_NORMAL, 0.2);
      break;
    }
  }
}
//###################################################################################
//## Functions
//###################################################################################
void start_timer()
{
  ssd1306_invertMode();
  while (digitalRead(gumb) == LOW);
  delay(100);
  ssd1306_fillScreen(0x00);
  ssd1306_printFixedN(0, 8, "Odstevanje:", STYLE_NORMAL, 0.2);
  int timer_countdown_min = timer_size_min;
  int timer_countdown_sec = timer_size_sec;
  int timer_countdown_complete_min_sec = timer_size_min * 60 + timer_size_sec;
  int timer_sec_buf_cunt = 1000; //timer_countdown_sec * 1000;
  int pause_cont_or_end = 0;     //ce je nic potem normalno tece,1-sledi izhod;
  move_servo_down_speed(30);//servo.write(min_pos);
  ssd1306_printFixedN(25, 16, " min", STYLE_NORMAL, 0.2);
  ssd1306_printFixedN(10, 16, itoa(timer_countdown_min, menu_number_val_buff_oled, 10), STYLE_NORMAL, 0.2);
  ssd1306_printFixedN(65, 16, " s", STYLE_NORMAL, 0.2);
  ssd1306_printFixedN(55, 16, itoa(timer_countdown_sec, menu_number_val_buff_oled, 10), STYLE_NORMAL, 0.2);
  while (timer_countdown_complete_min_sec > 0)
  {
    ssd1306_printFixedN(0, 16, "                              ", STYLE_NORMAL, 0.2);
    ssd1306_printFixedN(20, 16, " min", STYLE_NORMAL, 0.2);
    ssd1306_printFixedN(10, 16, itoa(timer_countdown_min, menu_number_val_buff_oled, 10), STYLE_NORMAL, 0.2);
    ssd1306_printFixedN(65, 16, " s", STYLE_NORMAL, 0.2);
    ssd1306_printFixedN(55, 16, itoa(timer_countdown_sec, menu_number_val_buff_oled, 10), STYLE_NORMAL, 0.2);
    while (timer_sec_buf_cunt > 1)
    {
      if (digitalRead(gumb) == LOW)
      {
        pause_cont_or_end = pause();
        if (!pause_cont_or_end)
          move_servo_down_speed(30);//servo.write(min_pos);
      }
      if (pause_cont_or_end)//skoci iz sekundne while
      {
        break;
      }
      timer_sec_buf_cunt--;
      delay(1);
    }
    if (pause_cont_or_end)//skoci iz main timer while
      break;
    timer_sec_buf_cunt = 1000;
    timer_countdown_complete_min_sec--;
    timer_countdown_min = timer_countdown_complete_min_sec / 60;
    timer_countdown_sec = timer_countdown_complete_min_sec - timer_countdown_min * 60;
  }
  move_servo_up_speed(30);//dvigni ko koncas
  ssd1306_fillScreen(0x00);
  ssd1306_normalMode();
  ssd1306_printFixed(0, 0, "Prozi timer:", STYLE_NORMAL);
}

int pause()
{
  move_servo_up_speed(30);//servo.write(max_pos);
  ssd1306_normalMode();
  int cont_or_end = 0; //0-nadaljuj,1-konc
  ssd1306_fillScreen(0x00);
  ssd1306_printFixedN(0, 8, "PAVZA", STYLE_NORMAL, 0.2);
  ssd1306_printFixedN(0, 16, "Nadaljuj?", STYLE_NORMAL, 0.2);
  while (digitalRead(gumb) == LOW)
    ; //prepreci nadaljevanje dokler ne spustis gumba
  delay(100);
  while (digitalRead(gumb) == HIGH)
  { //nadaljuje ko pritisns gumb
    cont_or_end = read_encoder_state(cont_or_end);
    if (cont_or_end > 1)
      cont_or_end = 1;
    if (cont_or_end < 0)
      cont_or_end = 0;
    if (cont_or_end)
      ssd1306_printFixedN(0, 32, "Ne", STYLE_NORMAL, 0.2);
    else
      ssd1306_printFixedN(0, 32, "Da", STYLE_NORMAL, 0.2);
  }
  delay(100);
  while (digitalRead(gumb) == LOW)
    ; //prepreci nadaljevanje dokler ne spustis gumba
  delay(100);
  ssd1306_fillScreen(0x00);
  ssd1306_printFixedN(0, 8, "Odstevanje:", STYLE_NORMAL, 0.2);
  ssd1306_invertMode();
  return cont_or_end;
}

void servo_hight_adjustment()
{
  if (digitalRead(gumb) == LOW)
  {
    ssd1306_invertMode();
    char num_str_buf[4];
    int buff_num;
    ssd1306_fillScreen(0x00);
    ssd1306_printFixedN(0, 8, "Nastavi visino(MAX):", STYLE_NORMAL, 0.2);
    ssd1306_drawProgressBar(max_pos * 100 / 180);
    move_servo_up_speed(30);//servo.write(max_pos);
    while (digitalRead(gumb) == LOW)
      ;
    delay(200);
    while (digitalRead(gumb) == HIGH)
    {
      set_servo_max_height();
    }
    ssd1306_fillScreen(0x00);
    ssd1306_printFixedN(0, 8, "Nastavi visino(MIN):", STYLE_NORMAL, 0.2);
    ssd1306_drawProgressBar(min_pos * 100 / 180);
    move_servo_down_speed(30);//servo.write(min_pos);
    while (digitalRead(gumb) == LOW)
      ;
    delay(200);
    while (digitalRead(gumb) == HIGH)
    {
      set_servo_min_height();
    }
    ssd1306_fillScreen(0x00);
    ssd1306_printFixedN(0, 8, "Izhod VS test!", STYLE_NORMAL, 0.2);
    ssd1306_printFixed(40, 16, itoa((max_pos * 100 / 180), num_str_buf, 10), STYLE_NORMAL);
    ssd1306_printFixed(40, 32, itoa((min_pos * 100 / 180), num_str_buf, 10), STYLE_NORMAL);
    Serial.print(max_pos);
    EEPROM.write(servo_max_eeprom_address, max_pos);
    EEPROM.write(servo_min_eeprom_address, min_pos);
    while (digitalRead(gumb) == LOW)
      ;
    delay(200);
    ssd1306_normalMode();
  }
}

void set_timer_size_min()
{
  char num_str_buf[4];
  int timer_old_buff = timer_size_min;
  if (digitalRead(gumb) == LOW)
  {
    ssd1306_invertMode();
    ssd1306_fillScreen(0x00);
    ssd1306_printFixedN(0, 8, "Nastavi cas(min):", STYLE_NORMAL, 0.8);
    ssd1306_printFixedN(32, 16, itoa(timer_size_min, num_str_buf, 10), STYLE_NORMAL, 1.2);
    while (digitalRead(gumb) == LOW)
      ; //release the button
    delay(200);
    while (digitalRead(gumb) == HIGH)
    {
      timer_size_min = read_encoder_state(timer_size_min);
      if (timer_size_min < 1)
        timer_size_min = 0;
      if (timer_old_buff != timer_size_min)
      {
        timer_old_buff = timer_size_min;
        ssd1306_printFixedN(32, 16, "    ", STYLE_NORMAL, 1.2);
        ssd1306_printFixedN(32, 16, itoa(timer_size_min, num_str_buf, 10), STYLE_NORMAL, 1.2);
      }
    }
    while (!digitalRead(gumb))
      ;
    delay(200);
    ssd1306_fillScreen(0x00);
    ssd1306_printFixedN(0, 8, "Nastavi cas(s):", STYLE_NORMAL, 0.8);
    while (digitalRead(gumb) == HIGH)
    {
      timer_size_sec = read_encoder_state(timer_size_sec);
      if (timer_size_sec > 59)
        timer_size_sec = 59;
      if (timer_size_sec < 1)
        timer_size_sec = 0;
      if (timer_old_buff != timer_size_sec)
      {
        timer_old_buff = timer_size_sec;
        ssd1306_printFixedN(32, 16, "    ", STYLE_NORMAL, 1.2);
        ssd1306_printFixedN(32, 16, itoa(timer_size_sec, num_str_buf, 10), STYLE_NORMAL, 1.2);
      }
    }
    EEPROM.write(timer_size_min_addr_eeprom_address, timer_size_min);
    EEPROM.write(timer_size_sec_addr_eeprom_address, timer_size_sec);
    while (digitalRead(gumb) == LOW)
      ;
    delay(200);
    ssd1306_normalMode();
  }
  //če je stanje drugačno kot prej preglej ali se timer_size_min povečuje ali manjša
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
      if (counter > 180)
        counter = 180;
      else
        counter--;
      /*if (counter < 0)
        counter = 0;
      else
        counter++;*/
    }
    else
    {
      if (counter < 0)
        counter = 0;
      else
        counter++;
      /*if (counter > 180)
        counter = 180;
      else
        counter--;*/
    }
  }
  aLastState = aState; // Updates the previous state of the outputA with the current state
  delay(1);
  return counter;
}
//funkciji za nastavljanje visine
void set_servo_max_height()
{
  int max_pos_prog_val = 0;
  if (max_pos < 1)
    max_pos = 0;
  max_pos_prog_val = 0;
  if (max_pos > 180)
    max_pos = 180;
  max_pos_prog_val = 100;
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
    if (max_pos < 1)
      max_pos_prog_val = 0;
    else
      max_pos_prog_val = max_pos * 100 / 180;
    ssd1306_drawProgressBar(max_pos_prog_val);
  }
  aLastState = aState; // Updates the previous state of the outputA with the current state
}

void set_servo_min_height()
{
  int min_pos_prog_val = 0;
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (min_pos < 1)
    min_pos = 0;
  min_pos_prog_val = 0;
  if (min_pos > 180)
    min_pos = 180;
  min_pos_prog_val = 100;
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
    if (min_pos < 1)
      min_pos_prog_val = 0;
    else
      min_pos_prog_val = min_pos * 100 / 180;
    ssd1306_drawProgressBar(min_pos_prog_val);
  }
  aLastState = aState; // Updates the previous state of the outputA with the current state
}

void write_number_to_string_on_oled(int x, int y, int num, char string[])
{
  ssd1306_printFixed(x, y, itoa(num, string, 4), STYLE_NORMAL);
}
void move_servo_down_speed(int step_speed)
{
  if(!poz_down){
  for (int i = max_pos; i > min_pos; i--)
  {
    servo.write(i);
    delay(step_speed);
  }
  poz_down=1;
  poz_up=0;
  }
}
void move_servo_up_speed(int step_speed)
{
  if(!poz_up){
  for (int i = min_pos; i < max_pos; i++)
  {
    servo.write(i);
    delay(step_speed);
  }
  poz_down=0;
  poz_up=1;
  }
}
