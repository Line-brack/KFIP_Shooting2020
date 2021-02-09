//�ҏW�F�c��c��
#include "DxLib.h"
#include <math.h>
#include "Bullet.h"
#include "Shooting.h"



/*�ȉ��ɒ萔�̒�`(define)*/

/*�ȉ��ɍ\���̂̒�`*/


//���X�g�̃|�C���^

Elist *eHead = NULL;//�G�̋󃊃X�g(�擪)���쐬
Elist *eTail = NULL;//�G�̋󃊃X�g(����)���쐬

/*�\���̂̃|�C���^*/
Key *keys;//�L�[�\���̂̃|�C���^



WldCounter *counter=NULL;

/*���C���֐�*/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE); // �E�B���h�E���[�h�ɐݒ�
	DxLib_Init(); // DX���C�u��������������
	SetDrawScreen(DX_SCREEN_BACK);//�`���𗠉�ʂɂ���
	
	keys = (Key*)malloc(sizeof(Key));//�L�[���͂̏�Ԃ��i�[����\����
	GetHitKeyStateAll(keys->nowKeys);//�ŏ��̃L�[���͂̏�Ԃ�������
	Scene scene = start;
	/*�ȉ��ɏ���������������*/
	//�摜�n���h���̈ꊇ���[�h
	loadGraphs();
	Player p = initializePlayer();//�v���C���[�̏�����
	initWldCounter();

	
				  /*���C�����[�v*/
	while (1) {
		if (ProcessMessage() != 0) {//���b�Z�[�W����
			break;//�~�{�^���Ń��[�v�I��
		}
		ClearDrawScreen();//��ʂ�����
		clsDx();//���[�v���ƂɑO�̌��ʂ�����
		
		switch (scene) {
		case start:
			//gameStart();
			DrawString(CX, CY, "�X�y�[�X�Ŏn�܂�", COLOR::white);
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
			DrawString(CX, CY, "GameOver...�X�y�[�X�ŏI��", COLOR::white);
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
		ScreenFlip();//����ʂ�\��ʂɔ��f
	}
	DxLib_End(); // DX���C�u�����I������
	return 0;
}

void gameStage1(Player *p) {
	/*�ȉ��ɕ`�揈��������*/
	drawEnemy();//�G�̕`�揈��
	drawPlayer(p);
	drawPlayerBullet();
	drawEnemyBullet();
	/*�ȉ��ɍX�V(+�␳)����������*/
	calcEnemy();//�G�̌v�Z����
	movePlayer(p);//�v���C���[�̓���
	calcPlayerBullet();
	calcEnemyBullet();
	int cnt[] = { 10,100,100,200,300,300,400,500,500 };
	double x[] = { 0,MX,0,MX,0,MX,0,MX,0 };
	double y[] = { 0,10,10,20,20,30,40,40,50 };
	using namespace STG1;
	EnemyPtn ePtn[] = {fairyL1,fairyR1,fairyL1,fairyR1,fairyL1,fairyR1,fairyL1,fairyR1,fairyL1 };
	
	int n = 9;
	genEnemies(cnt,x,y,ePtn,n);

	//��������e�𐶂ݏo������
	createPlayerShot(p);
	collisionEnemyAndPlayerShot();//�G-�v���C���[�e�̃R���W��������
	collisionPlayerAndEnemyShot(p);//�v���C���[-�G�e�̃R���W��������
	collisionPlayerAndEnemy(p);//�v���C���[-�G�̃R���W��������
	if (p->hp < 0) p->hp = 0;//�v���C���[��HP��-�ɂ��Ȃ�
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
	delAllEnemy();//�ꊇ�t���[
	delAllPlayerBullet();//�ꊇ�t���[
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
//�G�̐����֐�(�z��f�[�^���琶��(cnt�͏��������̃��X�g��z��))
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
//�摜���̏�����(handle�͍X�V���Ȃ�)
Graphic initGraph(const char *path, int width, int height, int numSliceX, int numSliceY) {
	Graphic g = { path,width,height,numSliceX,numSliceY };
	return g;
}
//Graphic�����[�h���ăn���h�����擾����֐�
void getHandle(Graphic *g) {
	const int num = g->numSliceX*g->numSliceY;
	//�T�C�Y���̉摜�n���h�����m��
	g->handle = (int *)malloc(sizeof(int)*num);
	if (num == 1) {
		g->handle[0] = LoadGraph(g->path);
	}
	else {
		LoadDivGraph(g->path, num, g->numSliceX, g->numSliceY, g->width / g->numSliceX, g->height / g->numSliceY, g->handle);
	}
}
//�摜�̈ꊇ���[�h
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

//HP�o�[�̕`��
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
//�~�`HP�̕`��
void drawHPCircle(double x, double y, double hp_per,double size_per) {
	int graph = GRAPHIC::circleGage.handle[0];
	DrawCircleGauge(x, y, 100, graph, 0, size_per, 1, 0);
	SetDrawBright(0, 255, 0);
	DrawCircleGauge(x, y, hp_per * 100, graph, 0, size_per, 1, 0);
	SetDrawBright(255, 255, 255);
}
void drawUI(Player p) {
	DrawBox(MX, 0, 640, MY, GetColor(200,100,0),1);//�g�̕`��
	drawHPBar(HPBAR_X, HPBAR_Y, (double)p.hp / PLAYER::hp,0);
}

//�~�̓����蔻��
template<typename T,typename U>
int onCollisionCircle(T obj1, U obj2) {
	//2�_�Ԃ̋���
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
				e->e.ptn.hp -= 3;//�GHP�����炷
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
			p->hp -= 1;//�v���C���[HP�����炷
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
			p->hp -= 2;//�v���C���[HP�����炷
			e->e.ptn.hp -= 10;
		}
		e = e->next;
	}
}
Player initializePlayer() {
	using namespace PLAYER;
	//x,y,size_x,size_y,v,exrate,interval,scount,index,(graph)
	const int scount = 0, index = 1;//�B���p�����[�^�̏�����
									//�v���C���[�̍\���̂Ə�����
	Player p = { x,y,hp,r,s_x,s_y ,v,exrate,interval, scount ,index };
	using namespace GRAPHIC;
	p.graph = player.handle;
	return p;
}
//�L�[�\���̂̍X�V
void updateKeys() {
	memcpy(keys->preKeys, keys->nowKeys, sizeof(keys->nowKeys));//�O�̃L�[�̉�����Ԃ��X�V
	GetHitKeyStateAll(keys->nowKeys);//���݂̃L�[�̉�����Ԃ��擾
	for (int i = 0; i < 256; i++)//�S�ẴL�[�ɂ���
		keys->state[i] = (KeyState)(keys->nowKeys[i] - keys->preKeys[i]);//�p���X�̌��o���������u��,�������u�Ԃ���ԂƂ��Ċi�[
}

//�e����v���C���[�ւ̊p�x��x���@�ŕԂ�
double getAngle(Bullet b, Player p) {
	double rad = atan2(p.y - b.y, p.x - b.x);
	return 180 * rad / PI;
}






void createPlayerShot(Player *p) {
	int isPush = keys->state[KEY_INPUT_Z] == pushNow;//z����������
	int isPull = keys->state[KEY_INPUT_Z] == pullNow;//z�𗣂�����
	int isEnoughInterval = p->sCount + p->interval <= counter->count;//�A�ł��Ă��Ȃ���
	if (isPull)p->sCount = counter->count;//���˃^�C�~���O�̊���X�V
	if (isPush&&isEnoughInterval) p->sCount = counter->count;//���˃^�C�~���O�̊���X�V

	if (!keys->nowKeys[KEY_INPUT_Z]) return;//z�L�[����������
	if (!((counter->count - p->sCount) % p->interval == 0)) return;//�C���^�[�o�����Ȃ�

	//�ȉ��̓V���b�g�̌���Ɛ���
	BulletPtn ptn = initBulletConstant(10, -90, COLOR::red, 1);
	Bullet b1 = { ptn,p->x - 10, p->y,  5  };//�e�f�[�^��p��
	Bullet b2 = { ptn,p->x + 10, p->y,  5  };//�e�f�[�^��p��
	addPlayerBullet(b1);//�e��ǉ�
	addPlayerBullet(b2);//�e��ǉ�
}


//���+blank���ɂ��邩
int isInWall(double x, double y, double blank) {
	return Btwn(-blank, x, MX + blank) && Btwn(-blank, y, MY + blank);
}

//�G�̕`�揈��
void drawEnemy() {
	int sum = 0;//���X�g�̗v�f��
	Elist *itr = eHead;//�w�b�h�̃|�C���^���R�s�[
	while (itr != NULL) {//�����܂ŉ�
						 //�摜�̕`��
		if (counter->count % 30 < 15) {
			DrawRotaGraph(itr->e.x, itr->e.y, itr->e.ptn.exRate, 0, itr->e.ptn.gpHandle[0], 1);
		}
		else {
			DrawRotaGraph(itr->e.x, itr->e.y, itr->e.ptn.exRate, 0, itr->e.ptn.gpHandle[1], 1);
		}
		drawHPCircle(itr->e.x, itr->e.y, itr->e.ptn.hp / 100.0,0.4);
		itr = itr->next;//�i�߂�
		sum++;
	}
	printfDx("Enemy=%d\n", sum);//�v�f����\��
}
//�G�̌v�Z����
void calcEnemy() {
	Elist *itr = eHead;
	while (itr != NULL) {//�����܂�
		using namespace MOVE;
		Enemy *e = &itr->e;
		//�����̏���
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
		//�e�̐���
		if (counter->count % 30 == 0) {
			createEnemyShot(*e, 10);
		}
		if (itr->e.ptn.hp>0 && isInWall(itr->e.x, itr->e.y, 100)) {//�����Ă��邩�͈͓��ɂ���ꍇ
			itr = itr->next;//�i�߂�
		}
		else {//hp��0�����܂��͉�ʂ͈̔͊O�ɂ���Ƃ�
			itr = delEnemy(itr);//�Y���̓G��Elist�������
		}

	}
}
void createEnemyShot(Enemy e,double r) {
	Bullet b = {e.ptn.bullet, e.x,e.y,r };
	addEnemyBullet(b);
}

//�G�̌v�Z����
void calcEnemyBullet() {
	EBullet *itr = ebHead;
	while (itr != NULL) {//�����܂�
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
			itr = itr->next;//�i�߂�
		}
		else {//��ʂ͈̔͊O�ɂ���Ƃ�
			itr = delEnemyBullet(itr);//�Y���̓G��Elist�������
		}

	}
}



// p1����p2�͈̔͂�p������Ƃ�1��Ԃ�
int Btwn(int p1, int p, int p2) {
	if (p2>p1 && (p1 <= p) && (p <= p2))
		return 1;
	if (p2 < p1 && (p2 <= p) && (p <= p1))
		return 1;
	return 0;
}
void movePlayer(Player *p) {
	int index, interval = 15;//�摜�̃C���f�b�N�X�ƃA�j���[�V�����̊Ԋu
							 //�t���[�����Ƃ̃A�j���[�V�����摜�̊���w��
	if (counter->count%interval < interval / 3)
		index = 0;
	else if (counter->count%interval < interval / 3 * 2)
		index = 3;
	else {
		index = 6;
	}
	//�L�[����
	bool isR = keys->nowKeys[KEY_INPUT_RIGHT];
	bool isL = keys->nowKeys[KEY_INPUT_LEFT];
	bool isU = keys->nowKeys[KEY_INPUT_UP];
	bool isD = keys->nowKeys[KEY_INPUT_DOWN];
	bool isDiag = (isR - isL) && (isD - isU);
	double v = p->v;
	if (isDiag)v *= cos(PI / 4);//�΂߂Ō���
	double vx = v*(isR - isL);
	double vy = v*(isD - isU);
	printfDx("v=%lf\n", v);
	//�C���f�b�N�X�̌���
	if (isR) {
		p->index = index;//���̂܂�
	}
	else if (isL) {
		p->index = index + 2;//2�񂸂炷
	}
	else {
		p->index = index + 1;//1�񂸂炷
	}
	//�ړ�
	p->x += vx, p->y += vy;
	//����l�Ɖ����l�̏���
	int minX = p->size_x / 2 * p->exRate;
	int maxX = MX - p->size_x / 2 * p->exRate;
	int minY = p->size_y / 2 * p->exRate;
	int maxY = MY - p->size_y / 2 * p->exRate;
	if (p->y > maxY)//���[���z���Ă���
		p->y = maxY;
	else if (p->y < minY)//��[���z���Ă���
		p->y = minY;
	if (p->x > maxX)//�E�[���z���Ă���
		p->x = maxX;
	else if (p->x < minX)//���[���z���Ă���
		p->x = minX;

}
//�v���C���[�̕`��
void drawPlayer(Player *p) {
	DrawRotaGraph(p->x, p->y, p->exRate, 0, p->graph[p->index], 1);
}
//2d�x�N�g��(x,y)�����v����deg����]����
//�����Fx,y�̃|�C���^,��]�p�xdeg
void rotate2d(double *x, double *y, double deg) {
	double rad = deg / 180.0 * PI;//�x���@���ʓx�@
	double x0 = *x, y0 = *y;//�X�V�O��x,y��x0,y0�Ƃ��Ċi�[
	*x = x0 * cos(rad) - y0 * sin(rad);//x�̉�]
	*y = x0 * sin(rad) + y0 * cos(rad);//y�̉�]
}