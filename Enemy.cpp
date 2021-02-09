#include "Vec2d.h"
#include "Enemy.h"
#include "DxLib.h"
#include <math.h>
#include "list.h"
#define PI acos(-1)

EnemyList *eHead = NULL;
EnemyList *eTail = NULL;

//動き+敵種類の初期化
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
		e->gpHandle = 0;//仮
		e->exRate = 0.5;
		e->sizeX = 10;
		e->sizeY = 10;
		e->r = 10;
			break;
		default:
			break;
	}
}
//弾幕パターンの初期化
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
	if (t%e.atk.atkspan != 0)return;//余り0以外のときは以下を実行しない
	//弾を生成する
	switch (e.atk.attackid) {
	case 1:
		break;
	}
}
//計算処理(個別)
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
//描画処理(個別)
void drawEnemy(Enemy *e) {
	double rad = atan2(e->v.y, e->v.x);
	DrawRotaGraph(e->p.x, e->p.y, e->exRate, rad, e->gpHandle, 1);
}

//敵の追加(リストの末尾に追加)
void addEnemy(Enemy e) {
	// 新しいnodeデータ
	EnemyList *pnode = (EnemyList *)malloc(sizeof(EnemyList));
	pnode->e = e;//新たなノードにデータを入れる
	pushBack(&pnode, &eHead, &eTail);//末尾に追加

}

//敵の削除
//引数：削除するノードを指すポインタ
//戻値：次のポインタ
EnemyList* delEnemy(EnemyList *e) {
	return deleteNode(&e, &eHead, &eTail);
}

//リスト全体を消去
void delAllEnemy() {
	deleteAllNode(&eHead, &eTail);
}
