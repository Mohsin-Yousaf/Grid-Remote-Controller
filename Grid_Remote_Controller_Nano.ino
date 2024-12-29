//pin usage as follow:
//             CS  DC/RS  RESET  SDI/MOSI  SDO/MISO  SCK  LED    VCC     GND    
//Arduino Uno  A3   A1     A2      11        12      13   A0   5V/3.3V   GND
//            T_IRQ  T_DO  T_DIN  T_CS  T_CLK
//Arduino Uno  6      7      8     9      10

#include <LCDWIKI_SPI.h> //Hardware-specific library
#include <LCDWIKI_TOUCH.h> //touch screen library

//paramters define
#define MODEL ILI9341
#define CS   A3    
#define RST  A2
#define DC   A1
#define LED  A0   //if you don't need to control the LED pin,you should set it to -1 and set it to 3.3V

//touch screen paramters define
#define TCLK  10
#define TCS   9
#define TDIN  8
#define TDOUT 7
#define TIRQ  6

//the definiens of hardware spi mode as follow:
LCDWIKI_SPI my_lcd(MODEL,CS,DC,RST,LED); //model,cs,dc,reset,led

//the definiens of touch mode as follow:
LCDWIKI_TOUCH my_touch(TCS,TCLK,TDOUT,TDIN,TIRQ); //tcs,tclk,tdout,tdin,tirq

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

uint16_t px,py;

void show_string(uint8_t *str, int16_t x, int16_t y, uint8_t csize, uint16_t fc, uint16_t bc, boolean mode)
{
    my_lcd.Set_Text_Mode(mode);
    my_lcd.Set_Text_Size(csize);
    my_lcd.Set_Text_colour(fc);
    my_lcd.Set_Text_Back_colour(bc);
    my_lcd.Print_String(str,x,y);
}

boolean is_pressed(int16_t x1,int16_t y1,int16_t x2,int16_t y2,int16_t px,int16_t py)
{
    if((px > x1 && px < x2) && (py > y1 && py < y2))
    {
        return true;  
    } 
    else
    {
        return false;  
    }
 }

int WarningPin = A4;

int BreakerIn1 = 2;
int BreakerOut1 = 3;

int IsolatorIn1 = 4;
int IsolatorOut1 = 5;

String TouchValue = "";

int Breaker1_State = LOW;
int Isolator1_State = LOW;

void setup(void) 
{

pinMode(WarningPin, OUTPUT);

pinMode(BreakerIn1, INPUT);
pinMode(BreakerOut1, OUTPUT);
pinMode(IsolatorIn1, INPUT);
pinMode(IsolatorOut1, OUTPUT);

digitalWrite(WarningPin, LOW);

digitalWrite(BreakerOut1, LOW);
digitalWrite(IsolatorOut1, LOW);

 my_lcd.Init_LCD();
 my_lcd.Set_Rotation(45);
 my_touch.TP_Set_Rotation(2);
 my_touch.TP_Init(my_lcd.Get_Rotation(),my_lcd.Get_Display_Width(),my_lcd.Get_Display_Height()); 
 my_lcd.Fill_Screen(BLUE); 
 
 my_lcd.Set_Draw_color(0, 0, 0);
 my_lcd.Draw_Fast_HLine(0, 3, my_lcd.Get_Display_Width());

/////////////////////////1st Set////////////////////////////////////

 my_lcd.Set_Draw_color(0, 0, 0);
 my_lcd.Fill_Rectangle(10, 10, 155, 115);
 show_string("1",15,15,2,YELLOW, BLACK,1);
 my_lcd.Set_Draw_color(0, 255, 0);
 my_lcd.Fill_Rectangle(30, 30, 135, 60);               // Breaker1
 show_string("Breaker",40,35,2,RED, BLACK,1);
 my_lcd.Set_Draw_color(0, 255, 255);
 my_lcd.Fill_Rectangle(30, 70, 135, 100);              // Isolator1
 show_string("Isolator",35,75,2,RED, BLACK,1);


 my_lcd.Set_Draw_color(255, 0, 0);
 my_lcd.Fill_Rectangle(140, 40, 150, 50);
 my_lcd.Fill_Rectangle(140, 80, 150, 90);
/////////////////////////2nd Set////////////////////////////////////

 my_lcd.Set_Draw_color(0, 0, 0);
 my_lcd.Fill_Rectangle(165, 10, 310, 115);
 show_string("2",170,15,2,YELLOW, BLACK,1);
 my_lcd.Set_Draw_color(0, 255, 0);
 my_lcd.Fill_Rectangle(185, 30, 290, 60);              // Breaker2
 show_string("Breaker",195,35,2,RED, BLACK,1);
 my_lcd.Set_Draw_color(0, 255, 255);
 my_lcd.Fill_Rectangle(185, 70, 290, 100);             // Isolator2
 show_string("Isolator",190,75,2,RED, BLACK,1);

/////////////////////////3rd Set////////////////////////////////////

 my_lcd.Set_Draw_color(0, 0, 0);
 my_lcd.Fill_Rectangle(10, 125, 155, 230);
 show_string("3",15,130,2,YELLOW, BLACK,1);
 my_lcd.Set_Draw_color(0, 255, 0);
 my_lcd.Fill_Rectangle(30, 145, 135, 175);              // Breaker3
 show_string("Breaker",40,150,2,RED, BLACK,1);
 my_lcd.Set_Draw_color(0, 255, 255);
 my_lcd.Fill_Rectangle(30, 185, 135, 215);              // Isolator3
 show_string("Isolator",35,190,2,RED, BLACK,1);

/////////////////////////4th Set////////////////////////////////////

 my_lcd.Set_Draw_color(0, 0, 0);
 my_lcd.Fill_Rectangle(165, 125, 310, 230);
 show_string("4",170,130,2,YELLOW, BLACK,1);
 my_lcd.Set_Draw_color(0, 255, 0);
 my_lcd.Fill_Rectangle(185, 145, 290, 175);             // Breaker4
 show_string("Breaker",195,150,2,RED, BLACK,1);
 my_lcd.Set_Draw_color(0, 255, 255);
 my_lcd.Fill_Rectangle(185, 185, 290, 215);             // Isolator4
 show_string("Isolator",190,190,2,RED, BLACK,1);

 Serial.begin(9600);



}

void loop(void)
{
  //Serial.read();
  
  px = 0;
  py = 0;
  my_touch.TP_Scan(0);
  if (my_touch.TP_Get_State()&TP_PRES_DOWN) 
  {
    px = my_touch.x;
    py = my_touch.y;
  } 

///// Read Breaker1 and Isolator1 state and set the Screen Indicators /////

Breaker1_State = digitalRead(BreakerIn1);
Isolator1_State = digitalRead(IsolatorIn1);

if (Breaker1_State == LOW)
{
    my_lcd.Set_Draw_color(255, 0, 0);
    my_lcd.Fill_Rectangle(140, 40, 150, 50);
}
else if (Breaker1_State == HIGH)
{
    my_lcd.Set_Draw_color(0, 255, 0);
    my_lcd.Fill_Rectangle(140, 40, 150, 50);
}

if (Isolator1_State == LOW)
{
    my_lcd.Set_Draw_color(255, 0, 0);
    my_lcd.Fill_Rectangle(140, 80, 150, 90);
}
else if (Isolator1_State == HIGH)
{
    my_lcd.Set_Draw_color(0, 255, 0);
    my_lcd.Fill_Rectangle(140, 80, 150, 90);
}

////////////////////////////////////////////////////////////////////

/////////////////////////1st Set////////////////////////////////////

  if(is_pressed(30, 30, 135, 60, px, py))
  {
      TouchValue = "Breaker1";
      delay(1);
   }

  if(is_pressed(30, 70, 135, 100, px, py))
  {
       TouchValue = "Isolator1";
       delay(1);
   }

/////////////////////////2nd Set////////////////////////////////////

  if(is_pressed(185, 30, 290, 60, px, py))
  {
       Serial.write("Breaker2");
       delay(300);
   }

  if(is_pressed(185, 70, 290, 100, px, py))
  {
       Serial.write("Isolator2");
       delay(300);
   }

/////////////////////////3rd Set////////////////////////////////////

  if(is_pressed(30, 145, 135, 175, px, py))
  {
       Serial.write("Breaker3");
       delay(300);
   }

  if(is_pressed(30, 185, 135, 215, px, py))
  {
       Serial.write("Isolator3");
       delay(300);
   }

/////////////////////////4th Set////////////////////////////////////

  if(is_pressed(185, 145, 290, 175, px, py))
  {
       Serial.write("Breaker4");
       delay(300);
   }

  if(is_pressed(185, 185, 290, 2150, px, py))
  {
       Serial.write("Isolator4");
       delay(300);
   }


  if (TouchValue == "Breaker1")
    digitalWrite(BreakerOut1, HIGH);
    delay(300);
    digitalWrite(BreakerOut1, LOW);


  if (TouchValue == "Isolator1" && Breaker1_State == HIGH)
  {
    digitalWrite(WarningPin, HIGH);
    delay(300);
    digitalWrite(WarningPin, LOW);    
  }
  else if (TouchValue == "Isolator1" && Breaker1_State == LOW)
  {  
    digitalWrite(IsolatorOut1, HIGH);
    delay(300);
    digitalWrite(IsolatorOut1, LOW);
  }


    TouchValue = "";
}
