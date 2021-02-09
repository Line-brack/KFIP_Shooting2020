//編集：田上慶治
#include "DxLib.h"
#include <math.h>
#include "Bullet.h"
#include "Shooting.h"



/*以下に定数の定義(define)*/

/*以下に構造体の定義*/


//リストのポインタ

Elist *eHead = NULL;//敵の空リスト(先頭)を作成
Elist *eTail = NULL;//敵の空リスト(末尾)を作成

/*構造体のポインタ*/
Key *keys;//キー構造体のポインタ



WldCounter *counter=NULL;

/*メイン関数*/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE); // ウィンドウモードに設定
	DxLib_Init(); // DXライブラリ初期化処理
	SetDrawScreen(DX_SCREEN_BACK);//描画先を裏画面にする
	
	keys = (Key*)malloc(sizeof(Key));//キー入力の状態を格納する構造体
	GetHitKeyStateAll(keys->nowKeys);//最初のキー入力の状態を初期化
	Scene scene = start;
	/*以下に初期化処理を書く*/
	//画像ハンドルの一括ロード
	loadGraphs();
	Player p = initializePlayer();//プレイヤーの初期化
	initWldCounter();

	
				  /*メインループ*/
	while (1) {
		if (ProcessMessage() != 0) {//メッセージ処理
			break;//×ボタンでループ終了
		}
		ClearDrawScreen();//画面を消す
		clsDx();//ループごとに前の結果を消す
		
		switch (scene) {
		case start:
			//gameStart();
			DrawString(CX, CY, "スペースで始まる", COLOR::white);
			if (keys->nowKeys[KEY_INPUT_SPACE]) {
				initWldCounter();
				scene = stage1; }
			break;
		case stage1:
			
			gameStage1(&p);
			if (p.hp == 0)scene = ending;
			break;
		case ending:
			//gameEnding();
			DrawString(CX, CY, "GameOver...スペースで終了", COLOR::white);
			if (keys->nowKeys[KEY_INPUT_SPACE])scene = end;
			
			break;
		case end:
			gameEnd(&p);
			break;
		}
		if (scene == end)
			break;
		updateKeys();
		counter->count++;
		ScreenFlip();//裏画面を表画面に反映
	}
	DxLib_End(); // DXライブラリ終了処理
	return 0;
}

void gameStage1(Player *p) {
	/*以下に描画処理を書く*/
	drawEnemy();//敵の描画処理
	drawPlayer(p);
	drawPlayerBullet();
	drawEnemyBullet();
	/*以下に更新(+補正)処理を書く*/
	calcEnemy();//敵の計算処理
	movePlayer(p);//プレイヤーの動作
	calcPlayerBullet();
	calcEnemyBullet();
	int cnt[] = { 10,100,100,200,300,300,400,500,500 };
	double x[] = { 0,MX,0,MX,0,MX,0,MX,0 };
	double y[] = { 0,10,10,20,20,30,40,40,50 };
	using namespace STG1;
	EnemyPtn ePtn[] = {fairyL1,fairyR1,fairyL1,fairyR1,fairyL1,fairyR1,fairyL1,fairyR1,fairyL1 };
	
	int n = 9;
	genEnemies(cnt,x,y,ePtn,n);

	//ここから弾を生み出す処理
	createPlayerShot(p);
	collisionEnemyAndPlayerShot();//敵-プレイヤー弾のコリジョン処理
	collisionPlayerAndEnemyShot(p);//プレイヤー-敵弾のコリジョン処理
	collisionPlayerAndEnemy(p);//プレイヤー-敵のコリジョン処理
	if (p->hp < 0) p->hp = 0;//プレイヤーのHPを-にしない
	drawUI(*p);
}

EnemyPtn initEnemy(int hp, MovePtn mv, BulletPtn bl, Graphic enemy, double exRate) {
	EnemyPtn e = { mv,bl,hp };
	e.sizeX = enemy.width / (double)enemy.numSliceX *exRate;
	e.sizeY = enemy.width / (double)enemy.numSliceX *exRate;
	e.exRate = exRate;
	return e;
}
void gameEnd(Player *p) {
	delAllEnemy();//一括フリー
	delAllPlayerBullet();//一括フリー
	delAllEnemyBullet();
	free(keys);
	free(p->graph);
}
MovePtn initMoveConstant(double v, int degree) {
	using namespace MOVE;
	double cos0 = cos(degree / 180.0*PI);
	double sin0 = sin(degree / 180.0*PI);
	MovePtn move = { constant,v*cos0,v*sin0 };
	return move;
}
MovePtn initMoveAccelarate(double v, double a, int degree) {
	using namespace MOVE;
	double cos0 = cos(degree / 180.0*PI);
	double sin0 = sin(degree / 180.0*PI);
	MovePtn move = { accelarate,v*cos0,v*sin0 ,a*cos0,a*sin0 };
	return move;
}
MovePtn initMoveStop(int dt) {
	using namespace MOVE;
	MovePtn move = { stop };
	move.dt = dt;
	return move;
}
void initWldCounter() {
	if (!counter) {
		counter = (WldCounter*)malloc(sizeof(WldCounter));
	}
	counter->count = 0, counter->index = 0;
}
//敵の生成関数(配列データから生成(cntは小さい順のリストを想定))
int genEnemies(int *cnt, double *x, double *y, EnemyPtn *ePtn, int n = 1) {
	printfDx("index=%d,count=%d\n", cnt[counter->index], counter->count);
	if (n == counter->index)return 0;
	while (cnt[counter->index] == counter->count) {
		int i = counter->index;
		Enemy e = { x[i],y[i],ePtn[i],ePtn[i].sizeX };
		addEnemy(e);
		counter->index++;
		if (n == counter->index)break;
	}
	return 1;

}
//画像情報の初期化(handleは更新しない)
Graphic initGraph(const char *path, int width, int height, int numSliceX, int numSliceY) {
	Graphic g = { path,width,height,numSliceX,numSliceY };
	return g;
}
//Graphicをロードしてハンドルを取得する関数
void getHandle(Graphic *g) {
	const int num = g->numSliceX*g->numSliceY;
	//サイズ分の画像ハンドルを確保
	g->handle = (int *)malloc(sizeof(int)*num);
	if (num == 1) {
		g->handle[0] = LoadGraph(g->path);
	}
	else {
		LoadDivGraph(g->path, num, g->numSliceX, g->numSliceY, g->width / g->numSliceX, g->height / g->numSliceY, g->handle);
	}
}
//画像の一括ロード
void loadGraphs() {
	using namespace GRAPHIC;
	getHandle(&player);
	getHandle(&fairy);
	getHandle(&circleGage);
	fairyB[0] = fairy.handle[0];
	fairyB[1] = fairy.handle[1];
	fairyR[0] = fairy.handle[2];
	fairyR[1] = fairy.handle[3];
	using namespace STG1;
	fairyL1.gpHandle = fairyB;
	fairyR1.gpHandle = fairyR;
}

//HPバーの描画
void drawHPBar(double x, double y, double hp_per,int vertical=1) {
	if (vertical) {
		DrawBox(x, y - HPBAR_YSIZE, x + HPBAR_XSIZE, y, COLOR::gray, 1);
		DrawBox(x, y - (HPBAR_YSIZE * hp_per), x + HPBAR_XSIZE, y, COLOR::green, 1);
	}
	else {
		DrawBox(x, y - HPBAR_XSIZE, x + HPBAR_YSIZE, y, COLOR::gray, 1);
		DrawBox(x, y - HPBAR_XSIZE, x + (HPBAR_YSIZE * hp_per), y, COLOR::green, 1);
	}
}
//円形HPの描画
void drawHPCircle(double x, double y, double hp_per,double size_per) {
	int graph = GRAPHIC::circleGage.handle[0];
	DrawCircleGauge(x, y, 100, graph, 0, size_per, 1, 0);
	SetDrawBright(0, 255, 0);
	DrawCircleGauge(x, y, hp_per * 100, graph, 0, size_per, 1, 0);
	SetDrawBright(255, 255, 255);
}
void drawUI(Player p) {
	DrawBox(MX, 0, 640, MY, GetColor(200,100,0),1);//枠の描画
	drawHPBar(HPBAR_X, HPBAR_Y, (double)p.hp / PLAYER::hp,0);
}

//円の当たり判定
template<typename T,typename U>
int onCollisionCircle(T obj1, U obj2) {
	//2点間の距離
	double dist = hypot(obj2.x - obj1.x, obj2.y - obj1.y);
	if (dist <= (obj1.r + obj2.r))
		return TRUE;
	return FALSE;
}

void collisionEnemyAndPlayerShot() {
	Elist *e = eHead;
	PBullet *pb;
	while (e != NULL) {
		pb = pbHead;
		while (pb != NULL) {
			if (onCollisionCircle(e->e, pb->s)) {
				e->e.ptn.hp -= 3;//敵HPを減らす
				pb = delPlayerBullet(pb);
			}
			else
				pb = pb->next;
		}
		e = e->next;
	}
}
void collisionPlayerAndEnemyShot(Player *p) {
	EBullet *eb = ebHead;
	while (eb != NULL) {
		if (onCollisionCircle(*p, eb->s)) {
			p->hp -= 1;//プレイヤーHPを減らす
			eb = delEnemyBullet(eb);
		}
		else
			eb = eb->next;
	}
}
void collisionPlayerAndEnemy(Player *p) {
	Elist *e = eHead;
	while (e != NULL) {
		if (onCollisionCircle(e->e, *p)) {
			p->hp -= 2;//プレイヤーHPを減らす
			e->e.ptn.hp -= 10;
		}
		e = e->next;
	}
}
Player initializePlayer() {
	using namespace PLAYER;
	//x,y,size_x,size_y,v,exrate,interval,scount,index,(graph)
	const int scount = 0, index = 1;//隠しパラメータの初期化
									//プレイヤーの構造体と初期化
	Player p = { x,y,hp,r,s_x,s_y ,v,exrate,interval, scount ,index };
	using namespace GRAPHIC;
	p.graph = player.handle;
	return p;
}
//キー構造体の更新
void updateKeys() {
	memcpy(keys->preKeys, keys->nowKeys, sizeof(keys->nowKeys));//前のキーの押下状態を更新
	GetHitKeyStateAll(keys->nowKeys);//現在のキーの押下状態を取得
	for (int i = 0; i < 256; i++)//全てのキーについて
		keys->state[i] = (KeyState)(keys->nowKeys[i] - keys->preKeys[i]);//パルスの検出→押した瞬間,離した瞬間を状態として格納
}

//弾からプレイヤーへの角度を度数法で返す
double getAngle(Bullet b, Player p) {
	double rad = atan2(p.y - b.y, p.x - b.x);
	return 180 * rad / PI;
}






void createPlayerShot(Player *p) {
	int isPush = keys->state[KEY_INPUT_Z] == pushNow;//zを押したか
	int isPull = keys->state[KEY_INPUT_Z] == pullNow;//zを離したか
	int isEnoughInterval = p->sCount + p->interval <= counter->count;//連打していないか
	if (isPull)p->sCount = counter->count;//発射タイミングの基準を更新
	if (isPush&&isEnoughInterval) p->sCount = counter->count;//発射タイミングの基準を更新

	if (!keys->nowKeys[KEY_INPUT_Z]) return;//zキーを押すかつ
	if (!((counter->count - p->sCount) % p->interval == 0)) return;//インターバル内なら

	//以下はショットの決定と生成
	BulletPtn ptn = initBulletConstant(10, -90, COLOR::red, 1);
	Bullet b1 = { ptn,p->x - 10, p->y,  5  };//弾データを用意
	Bullet b2 = { ptn,p->x + 10, p->y,  5  };//弾データを用意
	addPlayerBullet(b1);//弾を追加
	addPlayerBullet(b2);//弾を追加
}


//画面+blank内にいるか
int isInWall(double x, double y, double blank) {
	return Btwn(-blank, x, MX + blank) && Btwn(-blank, y, MY + blank);
}

//敵の描画処理
void drawEnemy() {
	int sum = 0;//リストの要素数
	Elist *itr = eHead;//ヘッドのポインタをコピー
	while (itr != NULL) {//末尾まで回す
						 //画像の描画
		if (counter->count % 30 < 15) {
			DrawRotaGraph(itr->e.x, itr->e.y, itr->e.ptn.exRate, 0, itr->e.ptn.gpHandle[0], 1);
		}
		else {
			DrawRotaGraph(itr->e.x, itr->e.y, itr->e.ptn.exRate, 0, itr->e.ptn.gpHandle[1], 1);
		}
		drawHPCircle(itr->e.x, itr->e.y, itr->e.ptn.hp / 100.0,0.4);
		itr = itr->next;//進める
		sum++;
	}
	printfDx("Enemy=%d\n", sum);//要素数を表示
}
//敵の計算処理
void calcEnemy() {
	Elist *itr = eHead;
	while (itr != NULL) {//末尾まで
		using namespace MOVE;
		Enemy *e = &itr->e;
		//動きの処理
		switch (e->ptn.move.ptn) {
		case accelarate:
			e->ptn.move.vx += e->ptn.move.ax;
			e->ptn.move.vy += e->ptn.move.ay;
		case constant:
			e->x += e->ptn.move.vx;
			e->y += e->ptn.move.vy;
			break;
		case stop:
			break;
		}
		//弾の生成
		if (counter->count % 30 == 0) {
			createEnemyShot(*e, 10);
		}
		if (itr->e.ptn.hp>0 && isInWall(itr->e.x, itr->e.y, 100)) {//生きているかつ範囲内にいる場合
			itr = itr->next;//進める
		}
		else {//hpが0か負または画面の範囲外にいるとき
			itr = delEnemy(itr);//該当の敵をElistから消す
		}

	}
}
void createEnemyShot(Enemy e,double r) {
	Bullet b = {e.ptn.bullet, e.x,e.y,r };
	addEnemyBullet(b);
}

//敵の計算処理
void calcEnemyBullet() {
	EBullet *itr = ebHead;
	while (itr != NULL) {//末尾まで
		using namespace BULLET;
		Bullet *b = &itr->s;
		switch (b->ptn.ptn) {
		case accelarate:
			b->ptn.vx += b->ptn.ax;
			b->ptn.vy += b->ptn.ay;
		case constant:
			b->x += b->ptn.vx;
			b->y += b->ptn.vy;
			break;
		}
		if (isInWall(itr->s.x, itr->s.y, 20)) {
			itr = itr->next;//進める
		}
		else {//画面の範囲外にいるとき
			itr = delEnemyBullet(itr);//該当の敵をElistから消す
		}

	}
}



// p1からp2の範囲にpがあるとき1を返す
int Btwn(int p1, int p, int p2) {
	if (p2>p1 && (p1 <= p) && (p <= p2))
		return 1;
	if (p2 < p1 && (p2 <= p) && (p <= p1))
		return 1;
	return 0;
}
void movePlayer(Player *p) {
	int index, interval = 15;//画像のインデックスとアニメーションの間隔
							 //フレームごとのアニメーション画像の基準を指定
	if (counter->count%interval < interval / 3)
		index = 0;
	else if (counter->count%interval < interval / 3 * 2)
		index = 3;
	else {
		index = 6;
	}
	//キー入力
	bool isR = keys->nowKeys[KEY_INPUT_RIGHT];
	bool isL = keys->nowKeys[KEY_INPUT_LEFT];
	bool isU = keys->nowKeys[KEY_INPUT_UP];
	bool isD = keys->nowKeys[KEY_INPUT_DOWN];
	bool isDiag = (isR - isL) && (isD - isU);
	double v = p->v;
	if (isDiag)v *= cos(PI / 4);//斜めで減衰
	double vx = v*(isR - isL);
	double vy = v*(isD - isU);
	printfDx("v=%lf\n", v);
	//インデックスの決定
	if (isR) {
		p->index = index;//そのまま
	}
	else if (isL) {
		p->index = index + 2;//2列ずらす
	}
	else {
		p->index = index + 1;//1列ずらす
	}
	//移動
	p->x += vx, p->y += vy;
	//上限値と下限値の処理
	int minX = p->size_x / 2 * p->exRate;
	int maxX = MX - p->size_x / 2 * p->exRate;
	int minY = p->size_y / 2 * p->exRate;
	int maxY = MY - p->size_y / 2 * p->exRate;
	if (p->y > maxY)//下端を越えている
		p->y = maxY;
	else if (p->y < minY)//上端を越えている
		p->y = minY;
	if (p->x > maxX)//右端を越えている
		p->x = maxX;
	else if (p->x < minX)//左端を越えている
		p->x = minX;

}
//プレイヤーの描画
void drawPlayer(Player *p) {
	DrawRotaGraph(p->x, p->y, p->exRate, 0, p->graph[p->index], 1);
}
//2dベクトル(x,y)を時計回りにdeg°回転する
//引数：x,yのポインタ,回転角度deg
void rotate2d(double *x, double *y, double deg) {
	double rad = deg / 180.0 * PI;//度数法→弧度法
	double x0 = *x, y0 = *y;//更新前のx,yをx0,y0として格納
	*x = x0 * cos(rad) - y0 * sin(rad);//xの回転
	*y = x0 * sin(rad) + y0 * cos(rad);//yの回転
}