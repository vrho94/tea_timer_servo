

#include <SeeedOLED.h>//grove lib-glej example!mogoce rabs se i2c inicializacijo in njeno kniznco
#include <Servo.h>
#include <EEPROM.h>

//rabs interrupt za timer in mogoce enkoder(pin int.) pazi na fronto proženja nastavitev

Servo servo;
int position_of_servo = 0;
int *ptr_of_servo_pos = &position_of_servo;
int timer_naslov_eeprom = 0; //naslov
void setup() {
  Serial.begin(9600);
  servo.attach(9);
}

//da se ne ukvarjas s prekinitvami uporab nizko vrednost za sestevalnika za eno sekundo... primer delay na 10ms(1000/10=100 proženj)

void loop() {
  if (Serial.available()) {
    position_of_servo = Serial.parseInt();
    servo.write(position_of_servo);
    Serial.write("\nPozicija sevota je:");
    Serial.print(position_of_servo);
    delay(100);
  }


}


void servo_trenutna_pozicija(){
    //uporabljena funkcija pri odštevanju časa in pa nastavitev_visine
  }

void nastavitev_visine(){
  //enako kot spodaj
  }

void set_timer() {
 //če je stanje drugačno kot prej preglej ali se timer povečuje ali manjša
 //se prav da je a signal pred bjom-se veča in obratno se man
}
/*
 * primer za nastavitev časa z enkoderjom(https://howtomechatronics.com/tutorials/arduino/rotary-encoder-works-use-arduino/):
  aState = digitalRead(outputA);
  
  if (aState != aLastState){     
     if (digitalRead(outputB) != aState) { 
       counter ++;
       angle ++;
       rotateCW();  
     }
     else {
       counter--;
       angle --;
       rotateCCW(); 
     }
     if (counter >=30 ) {
      counter =0;
     }
     
     lcd.clear();
     lcd.print("Position: ");
     lcd.print(int(angle*(-1.8)));
     lcd.print("deg"); 
     lcd.setCursor(0,0);
     
   }
  aLastState = aState;
}
*/
