#pragma once

/*以下に定数の定義(define)*/
#define MX 400//最大x
#define MY 480//最大y
#define CX MX/2//中心のx
#define CY MY/2//中心のy
#define PI acos(-1)//円周率π
/*HPバーの長さの設定*/
#define HPBAR_XSIZE 10
#define HPBAR_YSIZE 100
#define HPBAR_X MX+50
#define HPBAR_Y 450

enum Scene {
	start,
	end,
	stage1,
	ending
};


namespace PLAYER {
	//プレイヤーの初期値の設定
	static const double x = CX;
	static const double y = CY;
	static const double v = 3;
	static const int hp = 10;
	static const int r = 3;
	static const double interval = 5;
	//以下は基本変えない
	static const int s_x = 60;
	static const int s_y = 100;
	static const double exrate = 0.5;
}
namespace COLOR {
	static const int white = GetColor(255, 255, 255);
	static const int red = GetColor(255, 0, 0);
	static const int green = GetColor(0, 255, 0);
	static const int blue = GetColor(0, 0, 255);
	static const int gray = GetColor(128, 128, 128);
}
namespace BULLET {
	enum BulletType {
		constant,
		accelarate
	};
}
namespace MOVE {
	enum MoveType {
		constant,
		accelarate,
		stop
	};
}
/*画像データ*/
typedef struct {
	const char *path;
	const int width, height;
	const int numSliceX, numSliceY;
	int *handle;
}Graphic;
//画像情報の初期化(handleは更新しない)
Graphic initGraph(const char *path, int width, int height, int numSliceX, int numSliceY);

namespace GRAPHIC {
	Graphic player = initGraph("graphic/player_dot_rev.png", 180, 300, 3, 3);
	Graphic fairy = initGraph("graphic/fairy.png", 200, 200, 2, 2);
	Graphic circleGage = initGraph("graphic/img_en.png", 224, 224, 1, 1);
	int fairyB[2];
	int fairyR[2];
}

/*以下に構造体の定義*/
//動作パターンの構造体
typedef struct {
	MOVE::MoveType ptn;
	double vx, vy, ax, ay;
	int dt;
}MovePtn;
//弾のパターンの構造体
typedef struct {
	BULLET::BulletType ptn;
	double vx, vy, ax, ay;
	int damage, color;
}BulletPtn;
//敵パターンの構造体
typedef struct {
	MovePtn move;
	BulletPtn bullet;
	int hp;
	int *gpHandle;
	double sizeX, sizeY;
	double exRate;
}EnemyPtn;
MovePtn initMoveConstant(double v, int degree);
MovePtn initMoveAccelarate(double v, double a, int degree);
MovePtn initMoveStop(int dt);
BulletPtn initBulletConstant(double v, int degree, int color = COLOR::white, int damage = 1);
BulletPtn initBulletAccelarate(double v, double a, int degree, int color = COLOR::white, int damage = 1);
EnemyPtn initEnemy(int hp, MovePtn mv, BulletPtn bl, Graphic enemy, double exRate = 0.5);
//stage1のパターン
namespace STG1 {
	//動きのパターン
	MovePtn mvL1 = initMoveConstant(1, 45);
	MovePtn mvR1 = initMoveConstant(2, 180-45);
	MovePtn mv2 = initMoveAccelarate(5, 0.1, 90);
	//弾のパターン
	BulletPtn bl1 = initBulletConstant(1, 90);
	BulletPtn bl2 = initBulletAccelarate(1, -0.03, -90);
	//敵のパターン
	EnemyPtn fairyL1 = initEnemy(100, mvL1, bl1, GRAPHIC::fairy);
	EnemyPtn fairyR1 = initEnemy(100, mvR1, bl2, GRAPHIC::fairy);
}
//プレイヤーの構造体
typedef struct {
	double x, y;//座標
	int hp;//体力
	int r;//半径
	int size_x, size_y;//画像の大きさ
	double v;//4方向の速さ
	double exRate;//拡大率
	int interval;//ショットの間隔
	int sCount;//ショットを打ち始めるカウント
	int index;//画像ハンドルのインデックス
	int *graph;//画像ハンドル
}Player;

//弾の構造体
typedef struct {
	BulletPtn ptn;
	double x,y,r;
}Bullet;

//自弾の構造体リスト
typedef struct PlayerBullet {
	Bullet s;//プレイヤーのショットデータ
	struct PlayerBullet *before;//前のデータへのポインタ
	struct PlayerBullet *next;//次のデータへのポインタ
}PBullet;

//敵弾のリスト
typedef struct EnemyBullet {
	Bullet s;//弾データ
	struct EnemyBullet *before;//前のデータへのポインタ
	struct EnemyBullet *next;//次のデータへのポインタ
}EBullet;

//敵の構造体
typedef struct {
	double x, y;//座標
	EnemyPtn ptn;//パターン
	double r;//半径
}Enemy;

//敵の構造体リスト
typedef struct EnemyList {
	Enemy e;//敵データ
	struct EnemyList *before;//前のデータへのポインタ
	struct EnemyList *next;//次のデータへのポインタ
}Elist;
//WorldCounter
typedef struct {
	int count;//ステージのカウンタ
	int index;//時系列敵データのインデクス
}WldCounter;


//押した瞬間(pushNow),離した瞬間(pullNow),それ以外の状態(other)
enum KeyState {
	pushNow = 1,
	pullNow = -1,
	other = 0
};
typedef struct _keys {
	char preKeys[256];//前のキーの押下状態(1or0)
	char nowKeys[256];//現在のキーの押下状態(1or0)
	KeyState state[256];//現在のキーの状態
}Key;




/*以下に関数のプロトタイプ宣言を書く*/
/*リスト系*/
//汎用
//head->...->tailの末尾にnodeを追加
template<typename T>
void pushBack(T **node, T ** head, T **tail);
//nodeをhead-tailのリストから消す(nodeの次のノードを返す)
template<typename T>
T* deleteNode(T **node, T **head, T **tail);
//head-tailのリストのノードをすべて消去
template<typename T>
void deleteAllNode(T **head, T **tail);
//敵
void addEnemy(Enemy e);
Elist* delEnemy(Elist *p);
void delAllEnemy();
//敵弾
void addEnemyBullet(Bullet b);
EBullet* delEnemyBullet(EBullet *p);
void delAllEnemyBullet();
//プレイヤー弾
void addPlayerBullet(Bullet b);
PBullet* delPlayerBullet(PBullet *p);
void delAllPlayerBullet();

/*描画系*/
void drawHPBar(double x, double y, double hp_per, int vertical);
void drawHPCircle(double x, double y, double hp_per, double size_per);
void drawUI(Player p);
void drawPlayer(Player *p);
void drawPlayerBullet();
void drawEnemy();
void drawEnemyBullet();
/*更新系*/
void movePlayer(Player *p);
void calcPlayerBullet();
void calcEnemy();
void calcEnemyBullet();
/*汎用関数*/
int Btwn(int p1, int p, int p2);
void rotate2d(double *x, double *y, double deg);
int isInWall(double x, double y, double blank);
double getAngle(Bullet b, Player p);
/*create系*/
void createPlayerShot(Player *p);
void createEnemyShot(Enemy e, double r);
int genEnemies(int *cnt, double *x, double *y, EnemyPtn *ePtn, int n);
/*自機*/
Player initializePlayer();
/*敵*/
/*キー入力*/
void updateKeys();
/*当たり判定*/
template<typename T, typename U>
int onCollisionCircle(T obj1, U obj2);
void collisionEnemyAndPlayerShot();
void collisionPlayerAndEnemy(Player *p);
void collisionPlayerAndEnemyShot(Player *p);
/*ゲーム進行*/
void gameStage1(Player *p);
void gameEnd(Player *p);
/*画像*/

Graphic initGraph(const char *path, int width, int height, int numSliceX, int numSliceY);
void getHandle(Graphic *g);
void loadGraphs();
/*パターン*/
//動き
MovePtn initMoveConstant(double v, int degree);
MovePtn initMoveAccelarate(double v, double a, int degree);
MovePtn initMoveStop(int dt);
//弾
BulletPtn initBulletConstant(double v, int degree, int color, int damage);
BulletPtn initBulletAccelarate(double v, double a, int degree, int color, int damage);
//敵
EnemyPtn initEnemy(int hp, MovePtn mv, BulletPtn bl, Graphic enemy, double exRate);
//ワールドカウンター
void initWldCounter();




