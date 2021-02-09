#pragma once



Bullet initBulletConstant(Vec2d p,double v, int degree,int shapeid, int colorid, int damage );
Bullet initBulletAccelarate(Vec2d p, double v, double a,int degree, int shapeid , int colorid , int damage);
int getBulletHandle(int shapeid);
void initBulletHandles();
//弾の構造体
typedef struct {
	int moveid;
	int colorid;
	int shapeid;
	int damage;
	Vec2d p, v ,a;
	double r;
	int gphandle;
}Bullet;

void calcBullet(Bullet *b);
//弾の構造体リスト
typedef struct bulletlist {
	Bullet b;//プレイヤーのショットデータ
	struct bulletlist *before;//前のデータへのポインタ
	struct bulletlist *next;//次のデータへのポインタ
}BulletList;


void calcEnemyBullet();
void calcPlayerBullet();
void drawEnemyBullet();
void drawPlayerBullet();

void addEnemyBullet(Bullet b);
BulletList* delEnemyBullet(BulletList *p);
void delAllEnemyBullet();
//プレイヤー弾
void addPlayerBullet(Bullet b);
BulletList* delPlayerBullet(BulletList *p);
void delAllPlayerBullet();
