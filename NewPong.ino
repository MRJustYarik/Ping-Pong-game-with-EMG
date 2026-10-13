// библиотека для работы с дисплеем
#include <UTFT.h>
#include "squareLogic.h"
#include "colliderLogic.h"
#include "Starting_Values.h"
#include "EmgWork.h"

Square Plat_Player;
Square Plat_Bot;
Square Ball;
Collider col;
Starting_Values_Of_Parameters str;
EMGWork emg;

#define lBut 6
#define rBut 7
bool lBut_Push = 0;
bool rBut_Push = 0;

#define diod 4

#define TFT_CS 10
#define TFT_DC 9
#define TFT_MOSI 11
#define TFT_SCK 13
#define TFT_RST 8
#define TFT_MISO 12
//LED это 3.3v
//MOSI == SDI
//Резисторы: 2000 у контактов, 5000 у минуса


UTFT myGLCD(TFT01_22SP, TFT_MOSI, TFT_SCK , TFT_CS , TFT_RST , TFT_DC);

bool startMAX = 0;
bool KLB = false; //Режим проигрыша - false, Режим без поражений - true
bool pozCenter = true;

//Параметры логики игры---
#define plat_Speed 3
short ball_Speed_X = 3;
short ball_Speed_Y = 3;

//Метка остановки мячика
struct Extra_Sens
{
  int X;
  int Y;
  bool stop = false;
  short mode = 0;
};
Extra_Sens ex;

struct Save_Pos_Color{
  int X;
  int Y;
  short R;
  short G;
  short B;
  bool goUp;
  bool goRight;
};
Save_Pos_Color save;

struct Respawn_Ball_In_New_Raund
{
  unsigned long long timer;
  const int pause = 3;
  short textNum = 0; 

  bool ballDown = 0;
  bool oneAct = true;
};
Respawn_Ball_In_New_Raund rest;
//------------------------

//Параметры счета------
short POWER = 0;
struct Score_Control
{
  short player = 0;
  short bot = 0;
  const short win = 5;

  bool update = 0;
};
Score_Control score;
//------

int vrY;
short sig;

bool toch_P;
bool toch_B;

struct distantion{
  short Right;
  short Left;
  short All;
  short Center;
};
distantion dist;

int kP = (Ball.size.Y/2*-1)-(Plat_Player.size.Y-Ball.size.Y/2);
int kB = Ball.size.Y;

bool goRight;
bool goUp;

// объявления встроенного шрифта
extern uint8_t BigFont[];

//UTFT_SdRaw myFiles(&myGLCD);

void setup(){
  Serial.begin(115200);
  // инициализируем дисплей с горизонтальной ориентацией
  //myGLCD.InitLCD(LANDSCAPE);
  myGLCD.InitLCD(0);
  //myGLCD.InitLCD(PORTRAIT);
  // очищаем экран
  myGLCD.clrScr(); 
  //Plat_Player.StartTFT();

  pinMode(diod,OUTPUT);

  //Инициализация игрока
  Plat_Player.Creat(1,str.size.X[1],str.size.Y[1],str.color.R[1],str.color.G[1],str.color.B[1],1,0);
  Plat_Player.pos.X = str.pos.X[Plat_Player.index];
  Plat_Player.pos.Y = str.pos.Y[Plat_Player.index];
  Plat_Player.Draw();
  col.loadSizeeData(Plat_Player.index,Plat_Player.size.X, Plat_Player.size.Y);
  col.loadPositionData(Plat_Player.index,Plat_Player.pos.X, Plat_Player.pos.Y);
  //Инициализация бота
  Plat_Bot.Creat(2,str.size.X[2],str.size.Y[2],str.color.R[2],str.color.G[2],str.color.B[2],1,0);
  Plat_Bot.pos.X = str.pos.X[Plat_Bot.index];
  Plat_Bot.pos.Y = str.pos.Y[Plat_Bot.index];
  Plat_Bot.Draw();
  col.loadSizeeData(Plat_Bot.index,Plat_Bot.size.X, Plat_Bot.size.Y);
  col.loadPositionData(Plat_Bot.index,Plat_Bot.pos.X, Plat_Bot.pos.Y);
  //Инициализация Мяча
  Ball.Creat(0,str.size.X[0],str.size.Y[0],str.color.R[0],str.color.G[0],str.color.B[0],1,1);
  Ball.pos.X = str.pos.X[Ball.index];
  Ball.pos.Y = str.pos.Y[Ball.index];
  Ball.Draw();
  col.loadSizeeData(Ball.index,Ball.size.X, Ball.size.Y);
  col.loadPositionData(Ball.index,Ball.pos.X, Ball.pos.Y);


  kP = (Ball.size.Y/2*-1)-(Plat_Player.size.Y-Ball.size.Y/2);
  kB = Ball.size.Y;

  myGLCD.setFont(BigFont);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("Score", 240, 10, 90);
  scoreControl(0,0,0);

}

void loop() {
  if(rest.ballDown == false){
    //emg.setCrutch(80);
    ///emg.softPoten(emg.poten[1],80,1);
    ///emg.k = emg.poten[0];
    //emg.readSig();
    emg.flt_MX90_R(emg.getPotenc(80),50);
    //emg.flt_MX90();
    emg.sig();
    buttonSetMAX();

    if(digitalRead(lBut) == 0){
      //vrMove();
      //chekButton();
      movePlayer(1);
      moveBot(POWER,0); // (0,0) - 1 бот, (0,1) - 2 бота 
      //moveBallControl();
      moveBall();
      //chekSig(0);
      emg.ping = emg.timePing();
    }
    else{
      delay(emg.ping); //Задержка имитирующая задержку во время игры
    }
  }
  else{
    respawnBall();
    PowerUp(0,2,4);
  }
}

void buttonSetMAX(){
  if(digitalRead(lBut) == 1){
    if(startMAX == 0){
      emg.setMax(0);    
      startMAX = 1;
      //KLB = false;
    }
    else{
      emg.setMax(1);
    }
  }
  else if(digitalRead(lBut) == 0 && startMAX == 1){
    startMAX = 0;
  }
}

//Движение игрока------
//MODE 0 - управление кнопками
//MODE 1 - управление ЕМГ
void movePlayer(short MODE){
  if(MODE == 0){//----------------------------
    if(lBut_Push == 1){
      Plat_Player.MoveLeft(plat_Speed);
      col.loadPositionData(Plat_Player.index,Plat_Player.pos.X, Plat_Player.pos.Y);
    }

    if(rBut_Push == 1){
      Plat_Player.MoveRight(plat_Speed);
      col.loadPositionData(Plat_Player.index,Plat_Player.pos.X, Plat_Player.pos.Y);
    }
  }
  else if(MODE == 1){//------------------------
    if(emg.getSig() != 0){sig = emg.getSig();}
      if(sig == 1){
        if(Plat_Player.pos.X+Plat_Player.size.X < 320){
          Plat_Player.MoveRight(plat_Speed);
          col.loadPositionData(Plat_Player.index,Plat_Player.pos.X, Plat_Player.pos.Y);
          if(Plat_Player.pos.X+Plat_Player.size.X/2 > 155 && Plat_Player.pos.X+Plat_Player.size.X/2 < 165){sig = 0;}
        }else{sig = 0;}
      }
      else if(sig == -1){
        if(Plat_Player.pos.X > 0){
          Plat_Player.MoveLeft(plat_Speed);
          col.loadPositionData(Plat_Player.index,Plat_Player.pos.X, Plat_Player.pos.Y);
          if(Plat_Player.pos.X+Plat_Player.size.X/2 > 155 && Plat_Player.pos.X+Plat_Player.size.X/2 < 165){sig = 0;}
        }else{sig = 0;}
     }
  }
}

//Движение БОТА------
void moveBot(short P, bool playerBot){
  if(goUp == 1){
    if(Plat_Bot.pos.X+Plat_Bot.size.X < 320){
      if(P == 0){
        if(Ball.pos.Y+Ball.size.Y/2 > 120){
          if((Plat_Bot.pos.X+Plat_Bot.size.X/2) + Plat_Bot.size.X/4 < Ball.pos.X+Ball.size.X/2){
            Plat_Bot.MoveRight(plat_Speed);
            col.loadPositionData(Plat_Bot.index,Plat_Bot.pos.X, Plat_Bot.pos.Y);
          }
        }
      }
      else if(P == 1){
        if((Plat_Bot.pos.X+Plat_Bot.size.X/2) + Plat_Bot.size.X/4 < Ball.pos.X+Ball.size.X/2){
          Plat_Bot.MoveRight(plat_Speed);
          col.loadPositionData(Plat_Bot.index,Plat_Bot.pos.X, Plat_Bot.pos.Y);
        }
      }
      else if(P == 2){
        if((Plat_Bot.pos.X+Plat_Bot.size.X/2) + Plat_Bot.size.X/4 < ex.X+Ball.size.X/2){
          Plat_Bot.MoveRight(plat_Speed);
          col.loadPositionData(Plat_Bot.index,Plat_Bot.pos.X, Plat_Bot.pos.Y);
        }
      }
    }
    if(Plat_Bot.pos.X > 0){
      if(P == 0){
        if(Ball.pos.Y+Ball.size.Y/2 > 120){
          if((Plat_Bot.pos.X+Plat_Bot.size.X/2) - Plat_Bot.size.X/4 > Ball.pos.X+Ball.size.X/2){
            Plat_Bot.MoveLeft(plat_Speed);
            col.loadPositionData(Plat_Bot.index,Plat_Bot.pos.X, Plat_Bot.pos.Y);
          }
        }
      }
      else if(P == 1){
        if((Plat_Bot.pos.X+Plat_Bot.size.X/2) - Plat_Bot.size.X/4 > Ball.pos.X+Ball.size.X/2){
          Plat_Bot.MoveLeft(plat_Speed);
          col.loadPositionData(Plat_Bot.index,Plat_Bot.pos.X, Plat_Bot.pos.Y);
        }
      }
      else if(P == 2){
        if((Plat_Bot.pos.X+Plat_Bot.size.X/2) - Plat_Bot.size.X/4 > ex.X+Ball.size.X/2){
          Plat_Bot.MoveLeft(plat_Speed);
          col.loadPositionData(Plat_Bot.index,Plat_Bot.pos.X, Plat_Bot.pos.Y);
        }
      }
    }
  }
  else if(POWER == 2){
    if(Plat_Bot.pos.X+Plat_Bot.size.X < 320){
      if((Plat_Bot.pos.X+Plat_Bot.size.X/2) + Plat_Bot.size.X/8 < 170){
        Plat_Bot.MoveRight(plat_Speed);
        col.loadPositionData(Plat_Bot.index,Plat_Bot.pos.X, Plat_Bot.pos.Y);
      }
    }
    if(Plat_Bot.pos.X > 0){
      if((Plat_Bot.pos.X+Plat_Bot.size.X/2) - Plat_Bot.size.X/8 > 170){
        Plat_Bot.MoveLeft(plat_Speed);
        col.loadPositionData(Plat_Bot.index,Plat_Bot.pos.X, Plat_Bot.pos.Y);
      }
    }
  }

  if(playerBot == 1){
    if(goUp == 0){
      if(Plat_Player.pos.X+Plat_Player.size.X < 320){
        //if((Plat_Player.pos.X+Plat_Player.size.X/2) + Plat_Player.size.X/4 < Ball.pos.X+Ball.size.X/2){
        if((Plat_Player.pos.X+Plat_Player.size.X/2) + Plat_Player.size.X/4 < ex.X+Ball.size.X/2){
          Plat_Player.MoveRight(plat_Speed);
          col.loadPositionData(Plat_Player.index,Plat_Player.pos.X, Plat_Player.pos.Y);
        }
      }
      if(Plat_Player.pos.X > 0){
        //if((Plat_Player.pos.X+Plat_Player.size.X/2) - Plat_Player.size.X/4 > Ball.pos.X+Ball.size.X/2){
        if((Plat_Player.pos.X+Plat_Player.size.X/2) - Plat_Player.size.X/4 > ex.X+Ball.size.X/2){
          Plat_Player.MoveLeft(plat_Speed);
          col.loadPositionData(Plat_Player.index,Plat_Player.pos.X, Plat_Player.pos.Y);
        }
      }
    }
    else{
      if(Plat_Player.pos.X+Plat_Player.size.X < 320){
        //if((Plat_Player.pos.X+Plat_Player.size.X/2) + Plat_Player.size.X/4 < 180){
        if((Plat_Player.pos.X+Plat_Player.size.X/2) + Plat_Player.size.X/8 < 170){
          Plat_Player.MoveRight(plat_Speed);
          col.loadPositionData(Plat_Player.index,Plat_Player.pos.X, Plat_Player.pos.Y);
        }
      }
      if(Plat_Player.pos.X > 0){
        //if((Plat_Player.pos.X+Plat_Player.size.X/2) - Plat_Player.size.X/4 > 180){
        if((Plat_Player.pos.X+Plat_Player.size.X/2) - Plat_Player.size.X/8 > 170){
        //if((Plat_Player.pos.X+Plat_Player.size.X/2) > 160){
          Plat_Player.MoveLeft(plat_Speed);
          col.loadPositionData(Plat_Player.index,Plat_Player.pos.X, Plat_Player.pos.Y);
        }
      }
    }
  }
  
}

//ОТЛ движение по потенциометру------
void vrMove(){
  vrY = map(analogRead(A0),0,1024,Plat_Player.size.Y,240-Ball.size.Y-Plat_Bot.size.Y+1);
}

//Движение мяча------
void moveBall(){
  if(goRight == 1){Ball.MoveRight(Ball.speed.X);}
  else {Ball.MoveLeft(Ball.speed.X);}
  
  if(goUp == 1){Ball.MoveUp(Ball.speed.Y);}
  else {Ball.MoveDown(Ball.speed.Y);}
  
  //сохраняем позицию
  col.loadPositionData(Ball.index,Ball.pos.X, Ball.pos.Y);
  //проверяем на столкновение с рамками
  if(col.CollisBorder_X(0,Ball.speed.X) != '0'){ goRight = !goRight;}
  if(col.CollisBorder_Y(0,Ball.speed.Y) != '0'){ goUp = !goUp; if(!KLB){rest.ballDown = true;}}
  //проверяем на столкновение с платформами
  toch_P = col.CollisChek(0,1,kP);
  toch_B = col.CollisChek(0,2,kB);
  if(toch_P == 1 && goUp == 0 && goUp == 0){ 
    goUp = 1;
    //Расчет дистанции мяча относительно краёв платформы 
    col.DistEdges(dist.Right,dist.Left,1,kP);
    dist.All = dist.Right+dist.Left;
    dist.Center = col.md(col.md(dist.Right)-col.md(dist.Left)) / 2;
    //Задаём скорость
    setSpeed();
    //chekSig(2);

    gostBall(ex.mode);
  }
  if(toch_B == 1 && goUp == 1 && goUp == 1){
    goUp = 0;
    //Расчет дистанции мяча относительно краёв платформы
    col.DistEdges(dist.Right,dist.Left,2,kB);
    dist.All = dist.Right+dist.Left;
    dist.Center = col.md(col.md(dist.Right)-col.md(dist.Left)) / 2;
    //Задаём скорость
    setSpeed();
    //chekSig(2);

    gostBall(ex.mode);
  }
  digitalWrite(diod,(toch_B+toch_P));
}


void gostBall(short mode){
  save.X = Ball.pos.X;
  save.Y = Ball.pos.Y;
  save.R = Ball.color.R;
  save.G = Ball.color.G;
  save.B = Ball.color.B;
  save.goUp = goUp;
  save.goRight = goRight;
  Ball.setColor(255,255,0);
  ex.stop = false;
  while(ex.stop == false){
    if(mode == 1){
      if(goRight == 1){Ball.MoveRight(Ball.speed.X);}
      else {Ball.MoveLeft(Ball.speed.X);}
    
      if(goUp == 1){Ball.MoveUp(Ball.speed.Y);}
      else {Ball.MoveDown(Ball.speed.Y);}
    }
    else if(mode == 0 || mode == 2){
           if(goRight == 1 && goUp == 1){Ball.setPos(Ball.pos.X + Ball.speed.X,Ball.pos.Y + Ball.speed.Y);}
      else if(goRight == 1 && goUp == 0){Ball.setPos(Ball.pos.X + Ball.speed.X,Ball.pos.Y - Ball.speed.Y);}
      else if(goRight == 0 && goUp == 1){Ball.setPos(Ball.pos.X - Ball.speed.X,Ball.pos.Y + Ball.speed.Y);}
      else if(goRight == 0 && goUp == 0){Ball.setPos(Ball.pos.X - Ball.speed.X,Ball.pos.Y - Ball.speed.Y);}
    }
    //сохраняем позицию
    col.loadPositionData(Ball.index,Ball.pos.X, Ball.pos.Y);

    //проверяем на столкновение с рамками
    if(col.CollisBorder_X(0,Ball.speed.X) != '0'){ goRight = !goRight;}
    if(col.G_CollisBorder_Y(0,Ball.speed.Y) == 'D' && goUp == 0){  
      loadBall(mode);
    }
    else if(col.G_CollisBorder_Y(0,Ball.speed.Y) == 'U' && goUp == 1){  
      loadBall(mode);
    }
  }
}

void loadBall(short mode){
  ex.X = Ball.pos.X;
  ex.Y = Ball.pos.Y;
  ex.stop = true;
  goUp = save.goUp;
  goRight = save.goRight;
  
  if(mode == 2){
    Ball.Draw();
  }  

  //Ball.Erase();
  Ball.setColor(save.R,save.G,save.B);
  Ball.setPos(save.X,save.Y);
  //Plat_Bot.Draw();
  //Plat_Player.Draw();
}

//Управление градусом удара------
void setSpeed(){
  //Позиция 0
  if(dist.Center <= 18){
    Ball.SetSpeed(3,3);
    Ball.setColor(255,255,255);
  }
  //Позиция -1/1
  //else if(dist.Center <= Ball.size.Y*1.5){
  else if(dist.Right < 0 || dist.Left < 0){
  //else if(dist.Center <= Ball.size.Y*1.5){
    Ball.SetSpeed(4,2);
    Ball.setColor(255,0,255);
  }
  //Позиция -2/2
  else{
    Ball.SetSpeed(2,4);
    Ball.setColor(0,255,0);
  }
}

//ОТЛ движение мяча------
void moveBallControl(){
  if(lBut_Push == 1){
    Ball.MoveLeft(Ball.speed.X);
    //Ball.DrawPos(Ball.pos.X-Ball.speed.X,Ball.pos.Y,1);
    col.loadPositionData(Ball.index,Ball.pos.X, Ball.pos.Y);

    col.DistEdges(dist.Right,dist.Left,1,kP);
    dist.All = dist.Right+dist.Left;
    dist.Center = col.md(col.md(dist.Right)-col.md(dist.Left)) / 2;
    //Задаём скорость
    setSpeed();
    //chekSig(2);
    Ball.SetSpeed(1,1);
  }

  if(rBut_Push == 1){
    Ball.MoveRight(Ball.speed.X);
    //Ball.DrawPos(Ball.pos.X+Ball.speed.X,Ball.pos.Y,1);
    col.loadPositionData(Ball.index,Ball.pos.X, Ball.pos.Y);

    col.DistEdges(dist.Right,dist.Left,1,kP);
    dist.All = dist.Right+dist.Left;
    dist.Center = col.md(col.md(dist.Right)-col.md(dist.Left)) / 2;
    //Задаём скорость
    setSpeed();
    //chekSig(2);
    Ball.SetSpeed(1,1);
  }

  if(Ball.pos.Y+3 < vrY){
    Ball.MoveUp(ball_Speed_Y);
    col.loadPositionData(Ball.index,Ball.pos.X, Ball.pos.Y);
  }
  if(Ball.pos.Y-3 > vrY){
    Ball.MoveDown(ball_Speed_Y);
    col.loadPositionData(Ball.index,Ball.pos.X, Ball.pos.Y);
  }
  
  
  if(Ball.pos.Y < 120){
    //toch_P = col.CollisChek(0,1,-10);  
    toch_P = col.CollisChek(0,1,kP);  
    digitalWrite(diod,toch_P);
  }
  else{
    //toch_B = col.CollisChek(0,2,0);
    toch_B = col.CollisChek(0,2,kB);
    digitalWrite(diod,toch_B);
  }

}

//Показ и контроль счета------
void scoreControl(short setPlayer, short setBot,bool MODE){
  String ScoreText;
  if(MODE == true){
    ScoreText = (String)score.player +":"+ (String)score.bot;
    
    myGLCD.setColor(VGA_BLACK);
    myGLCD.print(ScoreText, 240, 110, 90);
    
    score.player = setPlayer;
    score.bot = setBot;
    if(score.bot >= score.win){WinBOT(1);}
    if(score.player >= score.win){WinBOT(0);}

    //PowerUp(0,2,4);
    ScoreText = (String)score.player +":"+ (String)score.bot;

    myGLCD.setColor(VGA_WHITE);
    myGLCD.print(ScoreText, 240, 110, 90);
  }
  else{
    ScoreText = (String)score.player +":"+ (String)score.bot;
    myGLCD.print(ScoreText, 240, 110, 90);
  }
}

void PowerUp(short L,short M, short H){
  if(score.player == L){POWER = 0;}
  else if(score.player == M){POWER = 1;}
  else if(score.player == H){POWER = 2;}
}

//Рестарт уровня------
void respawnBall(){
  if(rest.oneAct == true){
    Plat_Bot.Erase();
    Plat_Player.Erase();
    Ball.Erase();

    Plat_Player.setPos(str.pos.X[Plat_Player.index], str.pos.Y[Plat_Player.index]);
    Plat_Bot.setPos(str.pos.X[Plat_Bot.index], str.pos.Y[Plat_Bot.index]);
    
    if(goUp == 1){
      Ball.setPos(rnd(2,320-Ball.size.X-2), 140-Ball.size.Y/2 - (100-Plat_Player.size.Y));
      goRight = rnd(0,1);
    }
    else{
      Ball.setPos(rnd(2,320-Ball.size.X-2), 80-Ball.size.Y/2 + (100-Plat_Player.size.Y)); 
      goRight = rnd(0,1);
    }

    Plat_Bot.Draw();
    Plat_Player.Draw();
    Ball.Draw();

    col.loadPositionData(Plat_Player.index,Plat_Player.pos.X, Plat_Player.pos.Y);
    col.loadPositionData(Plat_Bot.index,Plat_Bot.pos.X, Plat_Bot.pos.Y);
    col.loadPositionData(Ball.index,Ball.pos.X, Ball.pos.Y);

    if(goUp == true){
      scoreControl(score.player,score.bot+1,1);
    }
    else{
      scoreControl(score.player+1,score.bot,1); 
    }

    rest.textNum = rest.pause;
    rest.oneAct = false;
    rest.timer = millis();
    
    textRestart(rest.textNum, 1);
  }
  else{
    //chekSig(1);
    if(millis()-rest.timer > 1000){
      textRestart(rest.textNum, 0);
      rest.textNum--;
      textRestart(rest.textNum, 1);
      rest.timer = millis();
    
      if(rest.textNum <= 0){
        textRestart(rest.textNum, 0);
        rest.ballDown = false;
        rest.oneAct = true;
        //Serial.println("----------------");
        gostBall(0);
      }
    }
  }
}

//Генератор рандомного значения------
int rnd(int min,int max){
  return (millis() % (max-min)) + min;  
}

void WinBOT(bool bot){
  myGLCD.clrScr();
  myGLCD.setColor(VGA_WHITE);
  if(bot == true){
    myGLCD.print("Bot WIN", 140, 100, 90);
    delay(3000);
    myGLCD.setColor(VGA_RED);
    myGLCD.print("FATALITE", 120, 100, 90);
    delay(99000);
  }
  else{
    myGLCD.print("You WIN", 140, 100, 90);
    delay(99000);
  }
}


//Управление текстом
void textRestart(int num, bool print){
  String S = (String)num;
  if(print == true){
    myGLCD.setColor(VGA_WHITE);
    myGLCD.print(S, 140, 170, 90);
  }
  else{
    myGLCD.setColor(VGA_BLACK);
    myGLCD.print(S, 140, 170, 90);
  }
}

//Работа кнопок------
void chekButton(){
  if(digitalRead(lBut) > 0){
    lBut_Push = 1;
  }
  else{lBut_Push = 0;}
  
  if(digitalRead(rBut) > 0){
    rBut_Push = 1;
  }
  else{rBut_Push = 0;}
}

//Показ сигнала------
void chekSig(short T){
  if(T == 0){
    Serial.print("RANGE Plr: ");
    Serial.print(col.md((col.pos[0].Y+(Ball.size.Y/2*-1)-(Plat_Player.size.Y-Ball.size.Y/2)) - col.pos[1].Y));
    Serial.print(" [");
    Serial.print(toch_P);
    Serial.print("] | ");
    Serial.print("Bot: ");
    Serial.print(col.md((col.pos[0].Y+Ball.size.Y) - col.pos[2].Y));
    Serial.print(" [");
    Serial.print(toch_B);
    Serial.print("] | Ball X/Y: ");
    Serial.print(Ball.pos.X);
    Serial.print("/");
    Serial.println(Ball.pos.Y);
  }
  else if(T == 1){
    int N = (millis()-rest.timer);
    Serial.print("time ms: ");
    Serial.print(N);
    Serial.print(" sec: ");
    Serial.print(rest.textNum);
    Serial.print(" rnd(0-200): ");
    Serial.println(rnd(0,200));
  }
  else if(T == 2){
    if(goUp == 1){
      Serial.println("Toch Player ");

      Serial.print(" Player X: ");
      Serial.println(Plat_Player.pos.X);
    }
    else{
      Serial.println("Toch Bot ");

      Serial.print(" Bot X: ");
      Serial.println(Plat_Bot.pos.X);
    }
      Serial.print(" Ball X: ");
      Serial.println(Ball.pos.X);
      
      Serial.print(" L/R: ");
      Serial.print(" [");
      Serial.print(dist.Left);
      Serial.print("/");
      Serial.print(dist.Right);
      Serial.println("]");

      Serial.print(" A/C: ");
      Serial.print(" (");
      Serial.print(dist.All);
      Serial.print("/");
      Serial.print(dist.Center);
      Serial.println(")");

      Serial.println("--------------");
    
  }
}
