#include "Vec2d.h"
#include "Bullet.h"
#include "list.h"
#include "DxLib.h"
#include <math.h>
#include "Shooting.h"
#define PI acos(-1)


BulletList *ebHead = NULL;//敵弾の空リスト(先頭)を作成
BulletList *ebTail = NULL;//敵弾の空リスト(末尾)を作成
BulletList *pbHead = NULL;//自弾の空リスト(先頭)を作成
BulletList *pbTail = NULL;//自弾の空リスト(末尾)を作成
int *BulletHandles;//弾用の画像ハンドル


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
		handle = BulletHandles[0];//丸弾
		break;
	case 2:
		handle = BulletHandles[1];//光弾
		break;
	case 3:
		handle = BulletHandles[2];//星弾
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
		b->p += b->v;//座標の更新
		break;
	case 2:
		b->p += b->v;//座標の更新
		b->v += b->a;//速度の更新
		break;
	default:
		break;
	}
}
void calcEnemyBullet() {
	BulletList *itr = ebHead;//ヘッドのポインタをコピー
	while (itr != NULL) {//末尾まで回す
		calcBullet(&(itr->b));
		//範囲内
		if (Btwn(-20, itr->b.p.x, MX + 20) && Btwn(-20, itr->b.p.y, MY + 20)) {//画面外+20のとき
			itr = itr->next;//進める
		}
		else {//範囲外
			itr = delEnemyBullet(itr);//該当の弾をPBulletから消す
		}
	}	
}
void calcPlayerBullet() {
	BulletList *itr = pbHead;//ヘッドのポインタをコピー
	while (itr != NULL) {//末尾まで回す
		calcBullet(&(itr->b));
		//範囲内
		if (Btwn(-20, itr->b.p.x, MX + 20) && Btwn(-20, itr->b.p.y, MY + 20)) {//画面外+20のとき
			itr = itr->next;//進める
		}
		else {//範囲外
			itr = delPlayerBullet(itr);//該当の弾をPBulletから消す
		}
	}
}
void drawEnemyBullet() {
	int sum = 0;//リストの要素数
	BulletList *itr= ebHead;//ヘッドのポインタをコピー
	while (itr != NULL) {//末尾まで回す
		Bullet b = itr->b;
		DrawRotaGraph(b.p.x, b.p.y, 1, 0, b.gphandle, 1);
		itr = itr->next;//進める
		sum++;
	}
	printfDx("eBullet=%d\n", sum);//要素数を表示
	
}
void drawPlayerBullet() {
	int sum = 0;//リストの要素数
	BulletList *itr = pbHead;//ヘッドのポインタをコピー
	while (itr != NULL) {//末尾まで回す
		Bullet b = itr->b;
		DrawRotaGraph(b.p.x, b.p.y, 1, 0, b.gphandle, 1);
		itr = itr->next;//進める
		sum++;
	}
	printfDx("pBullet=%d\n", sum);//要素数を表示

}


//プレイヤーの弾の追加(リストの末尾に追加)
void addPlayerBullet(Bullet b) {
	// 新しいnodeデータ
	BulletList *pnode = (BulletList *)malloc(sizeof(BulletList));
	pnode->b = b;//新たなノードにデータを入れる
	pushBack(&pnode, &pbHead, &pbTail);//末尾に追加

}
//弾の削除
//引数：削除するノードを指すポインタ
//戻値：次のポインタ
BulletList* delPlayerBullet(BulletList *p) {
	return deleteNode(&p, &pbHead, &pbTail);
}
//リスト全体を消去
void delAllPlayerBullet() {
	deleteAllNode(&pbHead, &pbTail);
}
//敵の弾の追加(リストの末尾に追加)
void addEnemyBullet(Bullet b) {
	// 新しいnodeデータ
	BulletList *enode = (BulletList *)malloc(sizeof(BulletList));
	enode->b = b;//新たなノードにデータを入れる
	pushBack(&enode, &ebHead, &ebTail);//末尾に追加

}
//弾の削除
//引数：削除するノードを指すポインタ
//戻値：次のポインタ
BulletList* delEnemyBullet(BulletList *p) {
	return deleteNode(&p, &ebHead, &ebTail);
}
//リスト全体を消去
void delAllEnemyBullet() {
	deleteAllNode(&ebHead, &ebTail);
}
