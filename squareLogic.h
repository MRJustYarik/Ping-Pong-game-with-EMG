// библиотека для работы с дисплеем
#include <UTFT.h>


#define TFT_CS 10
#define TFT_DC 9
#define TFT_MOSI 11
#define TFT_SCK 13
#define TFT_RST 8
#define TFT_MISO 12

UTFT classGLCD(TFT01_22SP, TFT_MOSI, TFT_SCK , TFT_CS , TFT_RST , TFT_DC);

class Square{
public:
//Переменные
  int index;


  struct SpeedSquad{
    short X = 1;
    short Y = 1;
  };
  SpeedSquad speed;
  struct SizeSquad{
    short X;
    short Y;
  };
  SizeSquad size;
  struct colorSquad{
    short R,G,B;
  };
  colorSquad color;
  struct positionSquad{
    int X;
    int Y;
  };
  positionSquad pos;
//Функции
  void StartTFT(){
    //classGLCD.InitLCD(LANDSCAPE);
    classGLCD.InitLCD(0);
    // очищаем экран
    classGLCD.clrScr(); 
  }

  //Инициализация---
  void Creat(short i,short s1,short s2,short c1,short c2,short c3,short spX,short spY){
    index = i;
    size.X = s1;
    size.Y = s2;
    color.R = c1;
    color.G = c2;
    color.B = c3;
    speed.X = spX;
    speed.Y = spY;
  }

  void setPos(short X,short Y){
    pos.X = X;
    pos.Y = Y;
  }
  
  void setSize(short X,short Y){
    size.X = X;
    size.Y = Y;
  }
  
  void SetSpeed(short X, short Y){
    speed.X = X;
    speed.Y = Y;
  }

  //Задаём цвет
  void setColor(short c1,short c2,short c3){
    color.R = c1;
    color.G = c2;
    color.B = c3;
  }
  //---------------

  //Функции движения-------------------------------------------------------------------------------
  void MoveRight(short Speed){
    classGLCD.setColor(color.R, color.G, color.B);
    //classGLCD.setColor(255, 50, 50); //TEST
    classGLCD.fillRect(pos.Y +1, pos.X+size.X +1, pos.Y+size.Y, pos.X+size.X+Speed);
    classGLCD.setColor(0, 0, 0);
    //classGLCD.setColor(255, 255, 255); //TEST
    classGLCD.fillRect(pos.Y +1, pos.X +1, pos.Y+size.Y, pos.X+Speed);

    pos.X+=Speed;
  }
  void MoveLeft(short Speed){
    classGLCD.setColor(color.R, color.G, color.B);
    //classGLCD.setColor(255, 50, 50); //TEST
    classGLCD.fillRect(pos.Y +1, pos.X -0, pos.Y+size.Y, pos.X-Speed+1);
    classGLCD.setColor(0, 0, 0);
    //classGLCD.setColor(255, 255, 255); //TEST
    classGLCD.fillRect(pos.Y +1, pos.X+size.X -0, pos.Y+size.Y, pos.X+size.X-Speed +1);

    pos.X-=Speed;
  }

  void MoveUp(short Speed){
    classGLCD.setColor(color.R, color.G, color.B);
    //classGLCD.setColor(50, 250, 50); //TEST
    classGLCD.fillRect(pos.Y+size.Y +1, pos.X +1, pos.Y+size.Y+Speed, pos.X+size.X);
    classGLCD.setColor(0, 0, 0);
    //classGLCD.setColor(255, 255, 255); //TEST
    classGLCD.fillRect(pos.Y +1, pos.X +1, pos.Y+Speed, pos.X+size.X);

    pos.Y+=Speed;
  }
  void MoveDown(short Speed){
    classGLCD.setColor(color.R, color.G, color.B);
    //classGLCD.setColor(50, 250, 50); //TEST
    classGLCD.fillRect(pos.Y -0, pos.X +1, pos.Y-Speed+1, pos.X+size.X);
    classGLCD.setColor(0, 0, 0);
    //classGLCD.setColor(255, 255, 255); //TEST
    classGLCD.fillRect(pos.Y+size.Y-Speed +1, pos.X +1, pos.Y+size.Y, pos.X+size.X);

    pos.Y-=Speed;
  }
  //-----------------------------------------------------------------------------------------------

  //Отрисовывание----------------------------------------------------------------------------------
  void Draw(){
    classGLCD.setColor(color.R, color.G, color.B);
    classGLCD.fillRect(pos.Y+1, pos.X+1, pos.Y+size.Y -0, pos.X+size.X -0);
  }

  void DrawPos(int x, int y,bool save){
    if(save == 1){
      pos.X = x;
      pos.Y = y;
    }
    classGLCD.setColor(color.R, color.G, color.B);
    classGLCD.fillRect(y, x, y+size.Y -1, x+size.X -1);
  }
  //-----------------------------------------------------------------------------------------------

  //Стирание---------------------------------------------------------------------------------------
  void Erase(){
    classGLCD.setColor(0, 0, 0);
    classGLCD.fillRect(pos.Y, pos.X, pos.Y+size.Y, pos.X+size.X);
  }

  void ErasePos(int x, int y,bool save){
    if(save == 1){
      pos.X = x;
      pos.Y = y;
    }
    classGLCD.setColor(0, 0, 0);
    classGLCD.fillRect(y, x, y+size.Y -1, x+size.X -1);
  }
  //-----------------------------------------------------------------------------------------------



private:
};
