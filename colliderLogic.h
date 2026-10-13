// библиотека для работы с дисплеем
#include <UTFT.h>


#define TFT_CS 10
#define TFT_DC 9
#define TFT_MOSI 11
#define TFT_SCK 13
#define TFT_RST 8
#define TFT_MISO 12

UTFT class2GLCD(TFT01_22SP, TFT_MOSI, TFT_SCK , TFT_CS , TFT_RST , TFT_DC);

class Collider {
public:
//Переменные
  struct SizeSquare{
    short X;
    short Y;
  };
  SizeSquare size[3];
  
  struct posSquare{
    int X;
    int Y;
  };
  posSquare pos[3];

//Функции
  //Загружаем данные фигруг--------------------------
  void loadSizeeData(short i,int X, int Y){
    size[i].X = X;
    size[i].Y = Y;
  }
  void loadPositionData(short i,int X, int Y){
    pos[i].X = X;
    pos[i].Y = Y;
  }

  bool CollisChek(short i1, short i2, short P){
    const short CollisTrue = 6;

    if(md(pos[i1].Y+P - pos[i2].Y) <= CollisTrue){
      //return 1;
      if((pos[i1].X > pos[i2].X) && (pos[i1].X < pos[i2].X+size[i2].X)){
        return 1;
      }
      else
      if((pos[i1].X+size[i1].X > pos[i2].X) && (pos[i1].X+size[i1].X < pos[i2].X+size[i2].X)){
          return 1;    
      }
      else{return 0;}
    }
    else{
      return 0;
    }
  }

  void DistEdges(short &R, short &L,short i2,short P){
    short i1 = 0;

    int k = pos[i2].X;
    int pX[2] = {pos[i2].X - k,(pos[i2].X+size[i2].X) - k};
    int bX[2] = {pos[i1].X - k,(pos[i1].X+size[i1].X) - k};

    L = md(pX[0]-bX[0]);
    R = md(pX[1]-bX[1]);

    if(pos[i2].X > pos[i1].X){
      L *= -1;
    }
    if(pos[i2].X+size[i2].X < pos[i1].X+size[i1].X){
      R *= -1;
    }
  }

  char G_CollisBorder_Y(short i,short speed){
    if(pos[i].Y-speed <= 0 + 20){
      return 'D';
    }
    else if(pos[i].Y+speed >= 220-size[i].Y - 20){
      return 'U';
    }
    else{
      return '0';
    }
  }

  char CollisBorder_Y(short i,short speed){
    if(pos[i].Y-speed <= 0){
      return 'D';
    }
    else if(pos[i].Y+speed >= 220-size[i].Y){
      return 'U';
    }
    else{
      return '0';
    }
  }

  char CollisBorder_X(short i,short speed){
    if(pos[i].X-(speed+1) <= 0){
      return 'L';
    }
    else if(pos[i].X+(speed+1) >= 320-size[i].X){
      return 'R';
    }
    else{
      return '0';
    }
  } 

  int md(int N){
    if(N<0){N*=-1;}
    return N;
  }
  
  

private:
};
