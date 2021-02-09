#pragma once


//攻撃パターンの構造体
typedef struct {
	int attackid;
	Vec2d p, v, a;
	int n, atkspan;
}Attack;
//敵パターンの構造体
typedef struct {
	int moveid;
	Attack atk;
	int enemyid;
	int hp;
	Vec2d p, v, a;
	int count;
	int gpHandle;
	double sizeX, sizeY;
	double exRate, r;
}Enemy;


//動き+敵種類の初期化
Enemy moveConstant(Attack atk,Vec2d p,double v, int degree,int enemyid);
Enemy moveAccelarate(Attack atk, Vec2d p, double v, double a, int degree,int enemyid);
void initEnemyInfo( Enemy *e,int enemyid);
//弾幕パターンの初期化
Attack simpleShot(double v, double a, int degree,int atkspan);
Attack radiationShot(double v, double a, int degree,int n,int atkspan);

//計算処理(個別)
void calcEnemy(Enemy *e);
//描画処理(個別)
void drawEnemy(Enemy *e);

//敵の構造体リスト
typedef struct enemylist {
	Enemy e;//敵データ
	struct enemylist *before;//前のデータへのポインタ
	struct enemylist *next;//次のデータへのポインタ
}EnemyList;

/*リスト系*/
//汎用
//敵
void addEnemy(Enemy e);
EnemyList* delEnemy(EnemyList *e);
void delAllEnemy();

