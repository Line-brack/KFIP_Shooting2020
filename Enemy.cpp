#include "Vec2d.h"
#include "Enemy.h"
#include "DxLib.h"
#include <math.h>
#include "list.h"
#define PI acos(-1)

EnemyList *eHead = NULL;
EnemyList *eTail = NULL;

//����+�G��ނ̏�����
Enemy moveConstant(Attack atk, Vec2d p, double v, int degree, int enemyid=1) {
	double cx = cos(degree*PI / 180.0);
	double sx = sin(degree*PI / 180.0);
	Enemy e;
	e.moveid = 1;
	e.atk = atk;
	e.p = p;
	e.v = Vec2d(v*cx, v*sx);
	initEnemyInfo(&e, enemyid);
	return e;
}
Enemy moveAccelarate(Attack atk, Vec2d p, double v, double a, int degree, int enemyid=1) {
	double cx = cos(degree*PI / 180.0);
	double sx = sin(degree*PI / 180.0);
	Enemy e;
	e.moveid = 2;
	e.atk = atk;
	e.p = p;
	e.v = Vec2d(v*cx, v*sx);
	e.a = Vec2d(a*cx, a*sx);
	initEnemyInfo(&e, enemyid);
	return e;
}
void initEnemyInfo(Enemy *e, int enemyid) {
	switch (enemyid) {
	case 1:
		e->gpHandle = 0;//��
		e->exRate = 0.5;
		e->sizeX = 10;
		e->sizeY = 10;
		e->r = 10;
			break;
		default:
			break;
	}
}
//�e���p�^�[���̏�����
Attack simpleShot(double v, double a, int degree, int atkspan) {
	Attack atk;
	double c = cos(degree / 180.0*PI);
	double s = sin(degree / 180.0*PI);
	atk.attackid = 1;
	atk.p = Vec2d(0,0);
	atk.v = Vec2d(v*c, v*s);
	atk.a = Vec2d(a*c, v*s);
	atk.atkspan = atkspan;
}
Attack radiationShot(double v, double a, int degree, int n, int atkspan) {
	Attack atk;
	double c = cos(degree / 180.0*PI);
	double s = sin(degree / 180.0*PI);
	atk.attackid = 1;
	atk.p = Vec2d(0, 0);
	atk.v = Vec2d(v*c, v*s);
	atk.a = Vec2d(a*c, v*s);
	atk.atkspan = atkspan;
	atk.n = n;
}

void calcAttack(Enemy e) {
	int t = e.count;
	if (t%e.atk.atkspan != 0)return;//�]��0�ȊO�̂Ƃ��͈ȉ������s���Ȃ�
	//�e�𐶐�����
	switch (e.atk.attackid) {
	case 1:
		break;
	}
}
//�v�Z����(��)
void calcEnemy(Enemy *e) {
	switch (e->moveid) {
	case 1:
		e->p += e->v;
		break;
	case 2:
		e->p += e->v;
		e->v += e->a;
		break;
	}
	calcAttack(*e);
}
//�`�揈��(��)
void drawEnemy(Enemy *e) {
	double rad = atan2(e->v.y, e->v.x);
	DrawRotaGraph(e->p.x, e->p.y, e->exRate, rad, e->gpHandle, 1);
}

//�G�̒ǉ�(���X�g�̖����ɒǉ�)
void addEnemy(Enemy e) {
	// �V����node�f�[�^
	EnemyList *pnode = (EnemyList *)malloc(sizeof(EnemyList));
	pnode->e = e;//�V���ȃm�[�h�Ƀf�[�^������
	pushBack(&pnode, &eHead, &eTail);//�����ɒǉ�

}

//�G�̍폜
//�����F�폜����m�[�h���w���|�C���^
//�ߒl�F���̃|�C���^
EnemyList* delEnemy(EnemyList *e) {
	return deleteNode(&e, &eHead, &eTail);
}

//���X�g�S�̂�����
void delAllEnemy() {
	deleteAllNode(&eHead, &eTail);
}
