#include "DxLib.h"
#include <math.h>
#include "Shooting.h"
#include <vector>
typedef struct {
	Enemy *body;
	int size;
}EnemyArray;

EnemyArray initEnemyArray() {
	EnemyArray ea;
	ea.size = 0;
	ea.body = NULL;
	return ea;
}
void delEnemyList(EnemyArray *ea){
	free(ea->body);//�������m�ۂ����Ă��邽�߁A�J�����Ȃ���΂Ȃ�Ȃ��I
}
int appendEnemies(EnemyArray *ea,Enemy *body) {
	int size_a = sizeof(body) / sizeof(Enemy);//�ǉ��v�f�̃T�C�Y
	ea->size+=size_a;//���X�g�̃T�C�Y�𑝂₷
	ea->body = (Enemy*)realloc(ea->body, sizeof(Enemy)*ea->size);//���T�C�Y
	if (ea->body == NULL) return -1;
	for(int i=0;i<size_a;i++)
		ea->body[ea->size - 1+i] = body[i];//�����ɗv�f��ǉ�
	return 0;
}

EnemyArray orderStage1() {
	
	int cnt = 0;//0����n�߂�
	EnemyArray e_arr = initEnemyArray();
	/*
	//�����̃p�^�[��
	MovePtn mvL1 = initMoveConstant(1, 45);
	MovePtn mvR1 = initMoveConstant(2, 180 - 45);
	MovePtn mv2 = initMoveAccelarate(5, 0.1, 90);
	//�e�̃p�^�[��
	BulletPtn bl1 = initBulletConstant(1, 90);
	BulletPtn bl2 = initBulletAccelarate(1, -0.03, -90);
	//�G�̃p�^�[��
	EnemyPtn fairyL1 = initEnemy(100, mvL1, bl1, GRAPHIC::fairy);
	EnemyPtn fairyR1 = initEnemy(100, mvR1, bl2, GRAPHIC::fairy);
	Enemy e1 = { 0,0,fairyL1,10 };
	appendEnemies(&e_arr, &e1);
	*/
	return e_arr;

}