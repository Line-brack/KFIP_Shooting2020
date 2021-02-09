#pragma once


//�U���p�^�[���̍\����
typedef struct {
	int attackid;
	Vec2d p, v, a;
	int n, atkspan;
}Attack;
//�G�p�^�[���̍\����
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


//����+�G��ނ̏�����
Enemy moveConstant(Attack atk,Vec2d p,double v, int degree,int enemyid);
Enemy moveAccelarate(Attack atk, Vec2d p, double v, double a, int degree,int enemyid);
void initEnemyInfo( Enemy *e,int enemyid);
//�e���p�^�[���̏�����
Attack simpleShot(double v, double a, int degree,int atkspan);
Attack radiationShot(double v, double a, int degree,int n,int atkspan);

//�v�Z����(��)
void calcEnemy(Enemy *e);
//�`�揈��(��)
void drawEnemy(Enemy *e);

//�G�̍\���̃��X�g
typedef struct enemylist {
	Enemy e;//�G�f�[�^
	struct enemylist *before;//�O�̃f�[�^�ւ̃|�C���^
	struct enemylist *next;//���̃f�[�^�ւ̃|�C���^
}EnemyList;

/*���X�g�n*/
//�ėp
//�G
void addEnemy(Enemy e);
EnemyList* delEnemy(EnemyList *e);
void delAllEnemy();

