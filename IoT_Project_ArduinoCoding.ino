/* Including Various Libraries */
#include <Wire.h>                                                                             
#include <LiquidCrystal_I2C.h> 
//#include <TWILiquidCrystal.h>
                                                       
#include "DHT.h"                                                                             
#include <LedControl.h>
//#include<hd44780.h>

/* Pin Definitions */

#define dht_data 7                                                                          /* Defining the sensor pin of DHT11 sensor connected to Arduino */
#define dht_type DHT11

const byte en = 2,rw = 1, rs = 0,d4 = 4,d5 = 5,d6 = 6,d7 = 7, bl = 3;    
const byte photo_in = A0; 
const byte moist_in = A1; 
const byte Din = 10, cs = 9, clk = 8;
/* const byte buzz = 12; */

/* Object Declaration */
LedControl lc = LedControl(Din, clk, cs, 0);                                                /* Defininng pins to control the 8X8 dot matrix */
LiquidCrystal_I2C lcd(0x27, 16, 2);                                                         /* Defining LCD connections to I2C bus */
DHT dht(dht_data, dht_type);                                                                /* Defining and Setting up DHT sensor */

/* Score Parameter Declarations */
short moist_val = 0;
short photo_val = 0;
float temp = 0, hum = 0;
byte score[4];
float final_score;
String comm = "";

/* 8x8 Matrix Face Setup */ 
byte smile[8] = {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};                                  /* Byte array to store the "Smile Expression"   */
byte neutral[8] = {0x3C,0x42,0xA5,0x81,0xBD,0x81,0x42,0x3C};                                /* Byte array to store the "Neutral Expression" */
byte sad[8] = {0x3C,0x42,0xA5,0x81,0x99,0xA5,0x42,0x3C};                                    /* Byte array to store the "Sad Expression"     */

void setup(){
  
  /* Declaring input pin */
  pinMode(dht_data,INPUT);
  pinMode(moist_in,INPUT);
  pinMode(photo_in,INPUT);

  /* Object Initialization */
  Serial.begin(9600);
  dht.begin();
  lcd.begin();
  lcd.backlight();
  /* Dot Matrix Initialization */
  lc.shutdown(0, false); 
  lc.setIntensity(0, 2); 
  lc.clearDisplay(0);

  /* Intro Message on the LCD Display */
  lcd_disp("Welcome","to");
  lcd_disp("Plant","Communicator");
  lcd_disp("for IoT","Domain Analyst");
}

void loop(){

  /* Data Collection */
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  moist_val = analogRead(moist_in);
  photo_val = analogRead(photo_in);


  /* --------------------------------------*/
  /* Temperature Parameters */
  if (temp >= 18 && temp <= 26){
    lcd_disp(("Temp:"+String(temp)),"Perfect");
    score[0] = 2.5;
  }
  else if (temp >= 6 && temp <= 34){
    lcd_disp(("Temp:"+String(temp)),"Fine");
    score[0] = 2.0;
  }
  else {
    lcd_disp(("Temp:"+String(temp)),"Bad");
    score[0] = 1.0;
  }

  /* --------------------------------------*/
  /* Humidity Parameters */
  if (hum >= 50 && hum <= 60){
    lcd_disp(("Humidity:"+String(hum)),"Perfect");
    score[1] = 2.5;
  }
  else if (hum >= 40 || hum <= 80){
    lcd_disp(("Humidity:"+String(hum)),"Fine");
    score[1] = 2.0;
  }
  else {
    lcd_disp(("Humidity:"+String(hum)),"Bad");
    score[1] = 1.0;
  }
  
  /* --------------------------------------*/
  /* Soil Moisture Parameters */
  if (moist_val >= 200 && moist_val <= 600){
    lcd_disp(("Soil Moisture"),"Perfect");
    score[2] = 3.0;
  }
  else if (moist_val > 600){
    lcd_disp("Soil Moisture","Low");
    score[2] = 1.0;
  }
  else {
    lcd_disp("Soil Moisture","Excess Water");
    score[2] = 0.5;
  }
  
  /* --------------------------------------*/
  /* Sunlight Parameters */
  if (photo_val >= 500){
    lcd_disp("Sunlight","Perfect");
    score[3] = 3.0;
  }
  else if (photo_val >= 300){
    lcd_disp("Sunlight","Could be better");
    score[3] = 1.0;
  }
  else {
    lcd_disp("Sunlight","Bad");
    score[3] = 0.5;
  }
  /* --------------------------------------*/

  final_score = 0;
  for(short i=0;i<4;i++) final_score = final_score + score[i];
  final_score = final_score / 4;

  /* Final Score */
  if (final_score >= 2.25){
    lcd_disp(("Score:"+String(final_score)),"Plant is HAPPY");
    printByte(smile);
  }
  else if (final_score >= 2.00){
    lcd_disp(("Score:"+String(final_score)),"Plant is Fine");
    printByte(neutral);
  }
  else {
    lcd_disp(("Score:"+String(final_score)),"Plant is SAD");
    printByte(sad);
  }
  /* --------------------------------------*/

  comm = String(temp)+"%%"+String(hum)+"%%"+String(moist_val)+"%%"+String(photo_val)+"%%"+String(final_score);
  Serial.println(comm);
  
}


/* Display fuction for the LCD */
void lcd_disp(String txt1,String txt2){
  lcd.clear();
  
  /* Putting the data in the middle */
  String gap1="",gap2="";
  byte a = (16 - txt1.length()) / 2;
  byte b = (16 - txt2.length()) / 2;
  for(a;a>0;a--) gap1 = gap1 +" ";
  for(b;b>0;b--) gap2 = gap2 +" ";
  txt1 = gap1+txt1;
  txt2 = gap2+txt2;

  /* Printing the data in the monitor */  
  lcd.setCursor(0,0);
  lcd.print(txt1);
  lcd.setCursor(0,1);
  lcd.print(txt2);
  delay(2000);
}

/* Function, called to print Expressions on the DOT Matrix Display*/
void printByte(byte character[]) 
{
  int i = 0;
  for (i = 0; i < 8; i++) lc.setRow(0, i, character[i]);
}
