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
	free(ea->body);//メモリ確保をしているため、開放しなければならない！
}
int appendEnemies(EnemyArray *ea,Enemy *body) {
	int size_a = sizeof(body) / sizeof(Enemy);//追加要素のサイズ
	ea->size+=size_a;//リストのサイズを増やす
	ea->body = (Enemy*)realloc(ea->body, sizeof(Enemy)*ea->size);//リサイズ
	if (ea->body == NULL) return -1;
	for(int i=0;i<size_a;i++)
		ea->body[ea->size - 1+i] = body[i];//末尾に要素を追加
	return 0;
}

EnemyArray orderStage1() {
	
	int cnt = 0;//0から始める
	EnemyArray e_arr = initEnemyArray();
	/*
	//動きのパターン
	MovePtn mvL1 = initMoveConstant(1, 45);
	MovePtn mvR1 = initMoveConstant(2, 180 - 45);
	MovePtn mv2 = initMoveAccelarate(5, 0.1, 90);
	//弾のパターン
	BulletPtn bl1 = initBulletConstant(1, 90);
	BulletPtn bl2 = initBulletAccelarate(1, -0.03, -90);
	//敵のパターン
	EnemyPtn fairyL1 = initEnemy(100, mvL1, bl1, GRAPHIC::fairy);
	EnemyPtn fairyR1 = initEnemy(100, mvR1, bl2, GRAPHIC::fairy);
	Enemy e1 = { 0,0,fairyL1,10 };
	appendEnemies(&e_arr, &e1);
	*/
	return e_arr;

}