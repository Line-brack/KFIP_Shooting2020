#pragma once



Bullet initBulletConstant(Vec2d p,double v, int degree,int shapeid, int colorid, int damage );
Bullet initBulletAccelarate(Vec2d p, double v, double a,int degree, int shapeid , int colorid , int damage);
int getBulletHandle(int shapeid);
void initBulletHandles();
//�e�̍\����
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
//�e�̍\���̃��X�g
typedef struct bulletlist {
	Bullet b;//�v���C���[�̃V���b�g�f�[�^
	struct bulletlist *before;//�O�̃f�[�^�ւ̃|�C���^
	struct bulletlist *next;//���̃f�[�^�ւ̃|�C���^
}BulletList;


void calcEnemyBullet();
void calcPlayerBullet();
void drawEnemyBullet();
void drawPlayerBullet();

void addEnemyBullet(Bullet b);
BulletList* delEnemyBullet(BulletList *p);
void delAllEnemyBullet();
//�v���C���[�e
void addPlayerBullet(Bullet b);
BulletList* delPlayerBullet(BulletList *p);
void delAllPlayerBullet();
