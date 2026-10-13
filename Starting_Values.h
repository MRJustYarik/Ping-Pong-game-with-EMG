//   __________
//  /   Index  \
// +--------+---+
// |   Ball | 0 |
// | Player | 1 |
// |   Bot  | 2 |
// +--------+---+


/*

//Инициализация игрока [1]
  Plat_Player.Creat(1,80,10,start.color[1][0],start.color[1][1],start.color[1][2],1,0);
  Plat_Player.position.X = 80;
  Plat_Player.position.Y = 0;

  //Инициализация бота [2]
  Plat_Bot.Creat(2,80,10,0,0,255,1,0);
  Plat_Bot.position.X = 80;
  Plat_Bot.position.Y = 230;

  //Инициализация Мяча [0]
  Ball.Creat(0,14,14,0,255,255,1,1);
  Ball.position.X = 100;
  Ball.position.Y = 200;

*/

class Starting_Values_Of_Parameters{
public:
	short index;

	struct Start_Size{
  		int X[3] = {14, 80,80};
  		int Y[3] = {14, 10,10};
	};
	Start_Size size;

	struct Start_Position{
  		int X[3] = {100, 120,120};
  		int Y[3] = {180, 0,210};
	};
	Start_Position pos;

	struct Start_Color{
  		int R[3] = {255,   0,255};
  		int G[3] = {255,   0,  0};
  		int B[3] = {255, 255,0};
	};
	Start_Color color;

};