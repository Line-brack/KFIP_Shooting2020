//�ҏW�F�c��c��
#include "DxLib.h"
#include <math.h>
#include "Shooting.h"



/*�ȉ��ɒ萔�̒�`(define)*/

/*�ȉ��ɍ\���̂̒�`*/
enum Scene {
	start,
	end,
	stage1,
	ending
};
typedef struct{
	int circle;
}Graphic;
//���X�g�̃|�C���^
PBullet *pbHead = NULL;//�v���C���[�̒e�̋󃊃X�g(�擪)���쐬
PBullet *pbTail = NULL;//�v���C���[�̒e�̋󃊃X�g(����)���쐬
Elist *eHead = NULL;//�G�̋󃊃X�g(�擪)���쐬
Elist *eTail = NULL;//�G�̋󃊃X�g(����)���쐬
EBullet *ebHead = NULL;//�G�e�̋󃊃X�g(�擪)���쐬
EBullet *ebTail = NULL;//�G�e�̋󃊃X�g(����)���쐬
/*�\���̂̃|�C���^*/
Key *keys;//�L�[�\���̂̃|�C���^
Graphic *gphandle;//�摜�n���h���\����

/*�ȉ��Ɋ֐��̃v���g�^�C�v�錾������*/
Player initializePlayer();
void updateKeys();
//head->...->tail�̖�����node��ǉ�
template<typename T>
void pushBack(T **node, T ** head, T **tail);
//node��head-tail�̃��X�g�������(node�̎��̃m�[�h��Ԃ�)
template<typename T>
T* deleteNode(T **node, T **head, T **tail);
//head-tail�̃��X�g�̃m�[�h�����ׂď���
template<typename T>
void deleteAllNode(T **head, T **tail);
template<typename T, typename U>
int onCollisionCircle(T obj1, U obj2);
void collisionEnemyAndPlayerShot();
void collisionPlayerAndEnemy(Player *p);
void collisionPlayerAndEnemyShot(Player *p);
void drawHPBar(double x, double y, double hp_per,int vertical);
void loadGraphHandles();
void drawHPCircle(double x, double y, double hp_per, double size_per);
void drawUI(Player p);
void gameStage1(int count, Player *p, int *fairy1, int *fairy2);
void gameEnd(Player *p);

/*���C���֐�*/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE); // �E�B���h�E���[�h�ɐݒ�
	DxLib_Init(); // DX���C�u��������������
	SetDrawScreen(DX_SCREEN_BACK);//�`���𗠉�ʂɂ���

	keys = (Key*)malloc(sizeof(Key));//�L�[���͂̏�Ԃ��i�[����\����
	GetHitKeyStateAll(keys->nowKeys);//�ŏ��̃L�[���͂̏�Ԃ�������
	Scene scene = start;
	/*�ȉ��ɏ���������������*/
	Player p = initializePlayer();//�v���C���[�̏�����
	loadGraphHandles();//�摜�n���h���̃��[�h
	int fairy[4];//�d���̉摜�n���h��
	LoadDivGraph("graphic/fairy.png", 4, 2, 2, 100, 100, fairy);
	int fairy1[2] = { fairy[0],fairy[1] };//�F�̗d��
	int fairy2[2] = { fairy[2],fairy[3] };//�ԐF�̗d��


	int count = 0;//�X�e�[�W�J�E���g
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
			if (keys->nowKeys[KEY_INPUT_SPACE])scene = stage1;
			break;
		case stage1:
			gameStage1(count,&p,fairy1,fairy2);
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
		count++;
		ScreenFlip();//����ʂ�\��ʂɔ��f
	}
	DxLib_End(); // DX���C�u�����I������
	return 0;
}

void gameStage1(int count,Player *p,int *fairy1,int *fairy2) {
	/*�ȉ��ɕ`�揈��������*/
	drawEnemy(count);//�G�̕`�揈��
	drawPlayer(p);
	drawPlayerBullet();
	drawEnemyBullet();
	/*�ȉ��ɍX�V(+�␳)����������*/
	calcEnemy();//�G�̌v�Z����
	movePlayer(p, count);//�v���C���[�̓���
	calcPlayerBullet();
	calcEnemyBullet();
	//�G��2�ʂ���
	Enemy e1 = { 0,0,1,1,100,30,100,100,0.7,fairy1 };
	Enemy e2 = { MX,MY,-1,-1,100,30,100,100,0.7,fairy2 };
	if (count % 60 == 0) {
		addEnemy(e1);
	}
	else if (count % 60 == 30) {
		addEnemy(e2);
	}

	//��������e�𐶂ݏo������
	createPlayerShot(p, count);
	createEnemyShot(*p, count);
	collisionEnemyAndPlayerShot();//�G-�v���C���[�e�̃R���W��������
	collisionPlayerAndEnemyShot(p);//�v���C���[-�G�e�̃R���W��������
	collisionPlayerAndEnemy(p);//�v���C���[-�G�̃R���W��������
	if (p->hp < 0) p->hp = 0;//�v���C���[��HP��-�ɂ��Ȃ�
	drawUI(*p);
}
void gameEnd(Player *p) {
	delAllEnemy();//�ꊇ�t���[
	delAllPlayerBullet();//�ꊇ�t���[
	delAllEnemyBullet();
	free(keys);
	free(p->graph);
	free(gphandle);
}
void loadGraphHandles() {
	using namespace GRAPHIC;
	gphandle =(Graphic*) malloc(sizeof(Graphic));
	gphandle->circle = LoadGraph(circle);
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
	DrawCircleGauge(x, y, 100, gphandle->circle, 0, size_per, 1, 0);
	SetDrawBright(0, 255, 0);
	DrawCircleGauge(x, y, hp_per * 100, gphandle->circle, 0, size_per, 1, 0);
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
				e->e.hp -= 3;//�GHP�����炷
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
			e->e.hp -= 10;
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
	p.graph = (int*)malloc(sizeof(int) * 9);
	LoadDivGraph(path, 9, 3, 3, s_x, s_y, p.graph);
	return p;
}
//�L�[�\���̂̍X�V
void updateKeys() {
	memcpy(keys->preKeys, keys->nowKeys, sizeof(keys->nowKeys));//�O�̃L�[�̉�����Ԃ��X�V
	GetHitKeyStateAll(keys->nowKeys);//���݂̃L�[�̉�����Ԃ��擾
	for (int i = 0; i < 256; i++)//�S�ẴL�[�ɂ���
		keys->state[i] = (KeyState)(keys->nowKeys[i] - keys->preKeys[i]);//�p���X�̌��o���������u��,�������u�Ԃ���ԂƂ��Ċi�[
}

/*�ȉ��Ɋ֐��̒�`������*/
template<typename T>
void pushBack(T **node, T ** head, T **tail) {
	(*node)->next = NULL;//�ǉ��m�[�h�̎���NULL

	if (*head == NULL) //��̃��X�g�̏ꍇ
		*head = *node;//�w�b�h�͒ǉ��m�[�h���w��
	if (*tail)//�����̃m�[�h�����ɂ���Ƃ�
		(*tail)->next = *node;//�O�m�[�h�̐��ǉ��m�[�h��
	(*node)->before = *tail;//�ǉ��m�[�h�̑O��O�m�[�h��
	*tail = *node;//Tail�͍Ō�̃m�[�h���w��
}
template<typename T>
T* deleteNode(T **node, T **head, T **tail) {
	T * pReturn = (*node)->next;//���̃m�[�h���w���|�C���^�̗p��
	if ((*node)->before)//�m�[�h���擪�łȂ��Ƃ�
		(*node)->before->next = pReturn;//��O�̎������̃m�[�h�ɂ���
	else
		*head = pReturn;//�w�b�h�̎w���m�[�h�����ɂ���

	if (pReturn)//p���������w���Ȃ��Ƃ�
		pReturn->before = (*node)->before;//���̃m�[�h����2�O�̃m�[�h���Ȃ�
	else//p���������w���Ƃ�
		*tail = (*node)->before;//�e�[���͑O�̃m�[�h���w��

	free(*node);//�m�[�h������
	return pReturn;//���̃|�C���^��Ԃ�

}
template<typename T>
void deleteAllNode(T **head, T **tail) {
	T *p, *pnext;//�|�C���^��2�p��
	pnext = *head;//�w�b�h�̃m�[�h���w��
	while (pnext) {//�m�[�h���Ȃ��Ȃ�܂ŌJ��Ԃ�
		p = pnext;//p��i�߂�
		pnext = p->next;//pnext��i�߂�
		free(p);//p�̃m�[�h���J��
	}
	*head = NULL;//�k��
	*tail = NULL;//�k��
}

void drawEnemyBullet() {
	int sum = 0;//���X�g�̗v�f��
	EBullet *itr = ebHead;//�w�b�h�̃|�C���^���R�s�[
	while (itr != NULL) {//�����܂ŉ�
						 //�~�̕`��
		DrawCircle(itr->s.x, itr->s.y, itr->s.r, itr->s.color, 1);
		itr = itr->next;//�i�߂�
		sum++;
	}
	printfDx("eBullet=%d\n", sum);//�v�f����\��
}
void calcEnemyBullet() {
	EBullet *itr = ebHead;
	while (itr != NULL) {//�����܂�
		itr->s.x += itr->s.vx;//x�����̈ړ�
		itr->s.y += itr->s.vy;//y�����̈ړ�

							  //�͈͓�
		if (Btwn(-20, itr->s.x, MX + 20) && Btwn(-20, itr->s.y, MY + 20)) {//��ʊO+20�̂Ƃ�
			itr = itr->next;//�i�߂�
		}
		else {//�͈͊O
			itr = delEnemyBullet(itr);//�Y���̒e��EBullet�������
		}


	}
}
void createEnemyShot(Player p, int count) {
	Bullet b = { 0, 0, 4, 0, 5 ,GetColor(255,255,255) };//�e�f�[�^
	if (count % 60 == 0) {//(0,30...)�̃^�C�~���O
		Elist *itr = eHead;
		int j;
		//�S�Ă̓G�Ń��[�v
		while (itr != NULL) {
			b.x = itr->e.x, b.y = itr->e.y;//�G�̍��W�ɏd�˂�
			double v = hypot(b.vx, b.vy);
			double rad = getAngle(b, p) / 180 * PI;
			b.vx = v * cos(rad), b.vy = v * sin(rad);
			addEnemyBullet(b);//�e��ǉ�

			itr = itr->next;
		}
	}
}
//�e����v���C���[�ւ̊p�x��x���@�ŕԂ�
double getAngle(Bullet b, Player p) {
	double rad = atan2(p.y - b.y, p.x - b.x);
	return 180 * rad / PI;
}
//�G�̒e�̒ǉ�(���X�g�̖����ɒǉ�)
void addEnemyBullet(Bullet b) {
	//�V����node�f�[�^
	EBullet *pnode = (EBullet *)malloc(sizeof(EBullet));
	pnode->s = b;//�V���ȃm�[�h�Ƀf�[�^������
	pushBack(&pnode, &ebHead, &ebTail);//�����ɒǉ�

}

//�G�e�̍폜
//�����F�폜����m�[�h���w���|�C���^
//�ߒl�F���̃|�C���^
EBullet* delEnemyBullet(EBullet *p) {
	return deleteNode(&p, &ebHead, &ebTail);
}

//�G�e���X�g�S�̂�����
void delAllEnemyBullet() {
	deleteAllNode(&ebHead, &ebTail);
}




void drawPlayerBullet() {
	int sum = 0;//���X�g�̗v�f��
	PBullet *itr = pbHead;//�w�b�h�̃|�C���^���R�s�[
	while (itr != NULL) {//�����܂ŉ�
						 //�~�̕`��
		DrawCircle(itr->s.x, itr->s.y, itr->s.r, itr->s.color, 1);
		itr = itr->next;//�i�߂�
		sum++;
	}
	printfDx("pBullet=%d\n", sum);//�v�f����\��
}
void calcPlayerBullet() {
	PBullet *itr = pbHead;
	while (itr != NULL) {//�����܂�
		itr->s.x += itr->s.vx;//x�����̈ړ�
		itr->s.y += itr->s.vy;//y�����̈ړ�
							  //�͈͓�
		if (Btwn(-20, itr->s.x, MX + 20) && Btwn(-20, itr->s.y, MY + 20)) {//��ʊO+20�̂Ƃ�
			itr = itr->next;//�i�߂�
		}
		else {//�͈͊O
			itr = delPlayerBullet(itr);//�Y���̒e��PBullet�������
		}

	}
}

void createPlayerShot(Player *p, int count) {
	int isPush = keys->state[KEY_INPUT_Z] == pushNow;//z����������
	int isPull = keys->state[KEY_INPUT_Z] == pullNow;//z�𗣂�����
	int isEnoughInterval = p->sCount + p->interval <= count;//�A�ł��Ă��Ȃ���
	if (isPull)p->sCount = count;//���˃^�C�~���O�̊���X�V
	if (isPush&&isEnoughInterval) p->sCount = count;//���˃^�C�~���O�̊���X�V

	if (!keys->nowKeys[KEY_INPUT_Z]) return;//z�L�[����������
	if (!((count - p->sCount) % p->interval == 0)) return;//�C���^�[�o�����Ȃ�

														  //�ȉ��̓V���b�g�̌���Ɛ���
	Bullet b1 = { p->x - 10, p->y, 0, -5, 5 ,GetColor(255,0,0) };//�e�f�[�^��p��
	Bullet b2 = { p->x + 10, p->y, 0, -5, 5 ,GetColor(255,0,0) };//�e�f�[�^��p��
	addPlayerBullet(b1);//�e��ǉ�
	addPlayerBullet(b2);//�e��ǉ�
}


//���+blank���ɂ��邩
int isInWall(double x, double y, double blank) {
	return Btwn(-blank, x, MX + blank) && Btwn(-blank, y, MY + blank);
}

//�G�̕`�揈��
void drawEnemy(int count) {
	int sum = 0;//���X�g�̗v�f��
	Elist *itr = eHead;//�w�b�h�̃|�C���^���R�s�[
	while (itr != NULL) {//�����܂ŉ�
						 //�摜�̕`��
		if (count % 30 < 15) {
			DrawRotaGraph(itr->e.x, itr->e.y, itr->e.exRate, 0, itr->e.graph[0], 1);
		}
		else {
			DrawRotaGraph(itr->e.x, itr->e.y, itr->e.exRate, 0, itr->e.graph[1], 1);
		}
		drawHPCircle(itr->e.x, itr->e.y, itr->e.hp / 100.0,0.4);
		itr = itr->next;//�i�߂�
		sum++;
	}
	printfDx("Enemy=%d\n", sum);//�v�f����\��
}
//�G�̌v�Z����
void calcEnemy() {
	Elist *itr = eHead;
	while (itr != NULL) {//�����܂�
		itr->e.x += itr->e.vx;//x�����̈ړ�
		itr->e.y += itr->e.vy;//y�����̈ړ�

		if (itr->e.hp>0 && isInWall(itr->e.x, itr->e.y, 100)) {//�����Ă��邩�͈͓��ɂ���ꍇ
			itr = itr->next;//�i�߂�
		}
		else {//hp��0�����܂��͉�ʂ͈̔͊O�ɂ���Ƃ�
			itr = delEnemy(itr);//�Y���̓G��Elist�������
		}

	}
}


//�G�̒ǉ�(���X�g�̖����ɒǉ�)
void addEnemy(Enemy e) {
	// �V����node�f�[�^
	Elist *pnode = (Elist *)malloc(sizeof(Elist));
	pnode->e = e;//�V���ȃm�[�h�Ƀf�[�^������
	pushBack(&pnode, &eHead, &eTail);//�����ɒǉ�

}

//�G�̍폜
//�����F�폜����m�[�h���w���|�C���^
//�ߒl�F���̃|�C���^
Elist* delEnemy(Elist *p) {
	return deleteNode(&p, &eHead, &eTail);
}

//���X�g�S�̂�����
void delAllEnemy() {
	deleteAllNode(&eHead, &eTail);
}

//�v���C���[�̒e�̒ǉ�(���X�g�̖����ɒǉ�)
void addPlayerBullet(Bullet b) {
	// �V����node�f�[�^
	PBullet *pnode = (PBullet *)malloc(sizeof(PBullet));
	pnode->s = b;//�V���ȃm�[�h�Ƀf�[�^������
	pushBack(&pnode, &pbHead, &pbTail);//�����ɒǉ�

}

//�e�̍폜
//�����F�폜����m�[�h���w���|�C���^
//�ߒl�F���̃|�C���^
PBullet* delPlayerBullet(PBullet *p) {
	return deleteNode(&p, &pbHead, &pbTail);

}

//���X�g�S�̂�����
void delAllPlayerBullet() {
	deleteAllNode(&pbHead, &pbTail);
}

// p1����p2�͈̔͂�p������Ƃ�1��Ԃ�
int Btwn(int p1, int p, int p2) {
	if (p2>p1 && (p1 <= p) && (p <= p2))
		return 1;
	if (p2 < p1 && (p2 <= p) && (p <= p1))
		return 1;
	return 0;
}
void movePlayer(Player *p, int count) {
	int index, interval = 15;//�摜�̃C���f�b�N�X�ƃA�j���[�V�����̊Ԋu
							 //�t���[�����Ƃ̃A�j���[�V�����摜�̊���w��
	if (count%interval < interval / 3)
		index = 0;
	else if (count%interval < interval / 3 * 2)
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