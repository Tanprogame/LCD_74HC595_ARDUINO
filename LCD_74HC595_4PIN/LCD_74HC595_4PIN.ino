#define _clock 7
#define _latch 5
#define _data 6
//#define RS    12
//#define E     11

#define FUNCTION_SET      0X28      //GIAO TIẾP 4 BIT 2 HÀNG
#define DISPLAY_CONTROL   0X0C
#define CLEAR_DISPLAY     0X01
#define ENTRY_MODE        0X06
#define SHIFT_LEFT        0X18
#define SHIFT_RIGHT       0X1C
#define ADDR_LINE1        0X80
#define ADDR_LINE2        0XC0

int ss,pp,hh;
uint8_t DV1,DV2,DV3, CH1,CH2,CH3;

void out_pin(uint8_t value){
  digitalWrite(_latch, LOW);
  for(int i=7; i>=0; i--){
    digitalWrite(_clock, 0);
    digitalWrite(_data, (value>>i)&0x01);
    digitalWrite(_clock, 1);
  } 
//  shiftOut(_data, _clock, MSBFIRST, value);
  digitalWrite(_latch, HIGH);
  
}
//void LCD_ENABLE(){
//  digitalWrite(E,1);  delayMicroseconds(1);
//  digitalWrite(E,0);  delayMicroseconds(100);
//}
void LCD_COMMAND(uint8_t mdk){
//  digitalWrite(RS,0); 
  out_pin(0x00);
  
  out_pin((mdk&0xF0));
//  LCD_ENABLE();
  out_pin((mdk&0xF0)|0x02);delayMicroseconds(1);
  out_pin((mdk&0xF0));delayMicroseconds(100);
  
  out_pin(mdk<<4);
//  LCD_ENABLE();
  out_pin((mdk<<4)|0x02);delayMicroseconds(1);
  out_pin(mdk<<4);delayMicroseconds(100);
}

void LCD_DATA(uint8_t mht){
//  digitalWrite(RS,1);
  out_pin(0x01);
  
  out_pin((mht&0xF0)|0x01);
//  LCD_ENABLE(); 
  out_pin((mht&0xF0)|0x03);delayMicroseconds(1);
  out_pin((mht&0xF0)|0x01);delayMicroseconds(100);
  
  out_pin((mht<<4)|0x01);
//  LCD_ENABLE();
  out_pin((mht<<4)|0x03);delayMicroseconds(1);
  out_pin((mht<<4)|0x01);delayMicroseconds(100);
  
}
void LCD_SDATA(String str){
  for(int i = 0; i<str.length(); i++){
    LCD_DATA(str.charAt(i));
  }
}
void LCD_SETUP(){
//  digitalWrite(E,0);
//  digitalWrite(RS,0);
  out_pin(0x30);
  out_pin(0x32);  delayMicroseconds(11);
  out_pin(0x30);  delayMicroseconds(110);
  out_pin(0x30);
  out_pin(0x32);  delayMicroseconds(11);
  out_pin(0x30);  delayMicroseconds(110);
  out_pin(0x30);
  out_pin(0x32);  delayMicroseconds(11);
  out_pin(0x30);  delayMicroseconds(110);
  out_pin(0x20);
  out_pin(0x22);  delayMicroseconds(11);
  out_pin(0x20);  delayMicroseconds(110);
  
//  LCD_ENABLE();   delayMicroseconds(10); 
//  out_pin(0x03); 
//  LCD_ENABLE();   delayMicroseconds(10); 
//  out_pin(0x03); 
//  LCD_ENABLE();   delayMicroseconds(10); 
//  out_pin(0x02); 
//  LCD_ENABLE();   delayMicroseconds(10); 
  
  LCD_COMMAND(FUNCTION_SET);        delay(2);
  LCD_COMMAND(CLEAR_DISPLAY);       delay(2);
  LCD_COMMAND(FUNCTION_SET);        delay(2);
  LCD_COMMAND(DISPLAY_CONTROL);     delay(2);
  LCD_COMMAND(ENTRY_MODE);          delay(2);
//  LCD_COMMAND(SHIFT_LEFT);          delayMicroseconds(200);
}
void HT_LCD(){
  DV1 = ss%10+0x30;
  CH1 = ss/10+0x30;
  DV2 = pp%10+0x30;
  CH2 = pp/10+0x30;
  DV3 = hh%10+0x30;
  CH3 = hh/10+0x30;
  
  LCD_COMMAND(0xC6);delay(1);
  LCD_DATA(CH3);LCD_DATA(DV3);
  LCD_DATA(':');
  LCD_DATA(CH2);LCD_DATA(DV2);
  LCD_DATA(':');
  LCD_DATA(CH1);LCD_DATA(DV1);
}
void setup() {
  Serial.begin(9600);
  pinMode(_latch, OUTPUT);
  pinMode(_clock, OUTPUT);
  pinMode(_data, OUTPUT);
//  pinMode(RS, OUTPUT);
//  pinMode(E, OUTPUT);
  LCD_SETUP();
  delay(50);

//  LCD_COMMAND(ADDR_LINE1);  delay(1);
//  LCD_DATA(0x30);

  LCD_COMMAND(ADDR_LINE1);        delay(1);
  LCD_SDATA("<<<TANPROGAME>>>");  delay(10);
  LCD_COMMAND(ADDR_LINE2);        delay(1);
  LCD_SDATA("TIME: ");            delay(10);
  hh=18; pp=26; ss=0;             //Set time initial

  HT_LCD();
}

void loop() {
//  LCD_DATA(0x30);delay(500);
//  LCD_DATA(0x31);delay(500);

  ss++;
  if(ss==60){
    ss=0;
    pp++;
    if(pp==60){
      pp=0;
      hh++;
      if(hh==24){
        hh=0;
      }
    }
  }
  HT_LCD();
  delay(1000); 
} 
