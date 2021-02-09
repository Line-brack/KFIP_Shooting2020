#include "Vec2d.h"
#include "Bullet.h"
#include "list.h"
#include "DxLib.h"
#include <math.h>
#include "Shooting.h"
#define PI acos(-1)


BulletList *ebHead = NULL;//�G�e�̋󃊃X�g(�擪)���쐬
BulletList *ebTail = NULL;//�G�e�̋󃊃X�g(����)���쐬
BulletList *pbHead = NULL;//���e�̋󃊃X�g(�擪)���쐬
BulletList *pbTail = NULL;//���e�̋󃊃X�g(����)���쐬
int *BulletHandles;//�e�p�̉摜�n���h��


Bullet initBulletConstant(Vec2d p, double v, int degree, int shapeid = 1, int colorid = 1, int damage = 1){
	double cos0 = cos(degree / 180.0*PI);
	double sin0 = sin(degree / 180.0*PI);
	Bullet b;
	b.moveid = 1;
	b.p = p;
	b.v = Vec2d(v*cos0, v*sin0);
	b.shapeid = shapeid;
	b.colorid = colorid;
	b.damage = damage;
	return b;
}
Bullet initBulletAccelarate(Vec2d p, double v, double a, int degree, int shapeid = 1, int colorid = 1, int damage = 1){
	
	double cos0 = cos(degree / 180.0*PI);
	double sin0 = sin(degree / 180.0*PI);
	Bullet b;
	b.moveid = 2;
	b.p = p;
	b.v = Vec2d(v*cos0, v*sin0);
	b.a = Vec2d(a*cos0, a*sin0);
	b.shapeid = shapeid;
	b.colorid = colorid;
	b.damage = damage;
	b.gphandle = getBulletHandle(shapeid);
	return b;
}
void initBulletHandles() {
	LoadDivGraph("graphic/bullet1.png", 3, 3, 1, 100, 100, BulletHandles, 0);
}
int getBulletHandle(int shapeid) {
	int handle;
	switch(shapeid)
	{
	case 1:
		handle = BulletHandles[0];//�ےe
		break;
	case 2:
		handle = BulletHandles[1];//���e
		break;
	case 3:
		handle = BulletHandles[2];//���e
		break;
	default:
		break;
	}
	return handle;
}

void calcBullet(Bullet *b) {
	switch (b->moveid)
	{
	case 1:
		b->p += b->v;//���W�̍X�V
		break;
	case 2:
		b->p += b->v;//���W�̍X�V
		b->v += b->a;//���x�̍X�V
		break;
	default:
		break;
	}
}
void calcEnemyBullet() {
	BulletList *itr = ebHead;//�w�b�h�̃|�C���^���R�s�[
	while (itr != NULL) {//�����܂ŉ�
		calcBullet(&(itr->b));
		//�͈͓�
		if (Btwn(-20, itr->b.p.x, MX + 20) && Btwn(-20, itr->b.p.y, MY + 20)) {//��ʊO+20�̂Ƃ�
			itr = itr->next;//�i�߂�
		}
		else {//�͈͊O
			itr = delEnemyBullet(itr);//�Y���̒e��PBullet�������
		}
	}	
}
void calcPlayerBullet() {
	BulletList *itr = pbHead;//�w�b�h�̃|�C���^���R�s�[
	while (itr != NULL) {//�����܂ŉ�
		calcBullet(&(itr->b));
		//�͈͓�
		if (Btwn(-20, itr->b.p.x, MX + 20) && Btwn(-20, itr->b.p.y, MY + 20)) {//��ʊO+20�̂Ƃ�
			itr = itr->next;//�i�߂�
		}
		else {//�͈͊O
			itr = delPlayerBullet(itr);//�Y���̒e��PBullet�������
		}
	}
}
void drawEnemyBullet() {
	int sum = 0;//���X�g�̗v�f��
	BulletList *itr= ebHead;//�w�b�h�̃|�C���^���R�s�[
	while (itr != NULL) {//�����܂ŉ�
		Bullet b = itr->b;
		DrawRotaGraph(b.p.x, b.p.y, 1, 0, b.gphandle, 1);
		itr = itr->next;//�i�߂�
		sum++;
	}
	printfDx("eBullet=%d\n", sum);//�v�f����\��
	
}
void drawPlayerBullet() {
	int sum = 0;//���X�g�̗v�f��
	BulletList *itr = pbHead;//�w�b�h�̃|�C���^���R�s�[
	while (itr != NULL) {//�����܂ŉ�
		Bullet b = itr->b;
		DrawRotaGraph(b.p.x, b.p.y, 1, 0, b.gphandle, 1);
		itr = itr->next;//�i�߂�
		sum++;
	}
	printfDx("pBullet=%d\n", sum);//�v�f����\��

}


//�v���C���[�̒e�̒ǉ�(���X�g�̖����ɒǉ�)
void addPlayerBullet(Bullet b) {
	// �V����node�f�[�^
	BulletList *pnode = (BulletList *)malloc(sizeof(BulletList));
	pnode->b = b;//�V���ȃm�[�h�Ƀf�[�^������
	pushBack(&pnode, &pbHead, &pbTail);//�����ɒǉ�

}
//�e�̍폜
//�����F�폜����m�[�h���w���|�C���^
//�ߒl�F���̃|�C���^
BulletList* delPlayerBullet(BulletList *p) {
	return deleteNode(&p, &pbHead, &pbTail);
}
//���X�g�S�̂�����
void delAllPlayerBullet() {
	deleteAllNode(&pbHead, &pbTail);
}
//�G�̒e�̒ǉ�(���X�g�̖����ɒǉ�)
void addEnemyBullet(Bullet b) {
	// �V����node�f�[�^
	BulletList *enode = (BulletList *)malloc(sizeof(BulletList));
	enode->b = b;//�V���ȃm�[�h�Ƀf�[�^������
	pushBack(&enode, &ebHead, &ebTail);//�����ɒǉ�

}
//�e�̍폜
//�����F�폜����m�[�h���w���|�C���^
//�ߒl�F���̃|�C���^
BulletList* delEnemyBullet(BulletList *p) {
	return deleteNode(&p, &ebHead, &ebTail);
}
//���X�g�S�̂�����
void delAllEnemyBullet() {
	deleteAllNode(&ebHead, &ebTail);
}
