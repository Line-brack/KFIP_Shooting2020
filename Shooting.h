#pragma once

/*�ȉ��ɒ萔�̒�`(define)*/
#define MX 400//�ő�x
#define MY 480//�ő�y
#define CX MX/2//���S��x
#define CY MY/2//���S��y
#define PI acos(-1)//�~������
/*HP�o�[�̒����̐ݒ�*/
#define HPBAR_XSIZE 10
#define HPBAR_YSIZE 100
#define HPBAR_X MX+50
#define HPBAR_Y 450

enum Scene {
	start,
	end,
	stage1,
	ending
};


namespace PLAYER {
	//�v���C���[�̏����l�̐ݒ�
	static const double x = CX;
	static const double y = CY;
	static const double v = 3;
	static const int hp = 10;
	static const int r = 3;
	static const double interval = 5;
	//�ȉ��͊�{�ς��Ȃ�
	static const int s_x = 60;
	static const int s_y = 100;
	static const double exrate = 0.5;
}
namespace COLOR {
	static const int white = GetColor(255, 255, 255);
	static const int red = GetColor(255, 0, 0);
	static const int green = GetColor(0, 255, 0);
	static const int blue = GetColor(0, 0, 255);
	static const int gray = GetColor(128, 128, 128);
}
namespace BULLET {
	enum BulletType {
		constant,
		accelarate
	};
}
namespace MOVE {
	enum MoveType {
		constant,
		accelarate,
		stop
	};
}
/*�摜�f�[�^*/
typedef struct {
	const char *path;
	const int width, height;
	const int numSliceX, numSliceY;
	int *handle;
}Graphic;
//�摜���̏�����(handle�͍X�V���Ȃ�)
Graphic initGraph(const char *path, int width, int height, int numSliceX, int numSliceY);

namespace GRAPHIC {
	Graphic player = initGraph("graphic/player_dot_rev.png", 180, 300, 3, 3);
	Graphic fairy = initGraph("graphic/fairy.png", 200, 200, 2, 2);
	Graphic circleGage = initGraph("graphic/img_en.png", 224, 224, 1, 1);
	int fairyB[2];
	int fairyR[2];
}

/*�ȉ��ɍ\���̂̒�`*/
//����p�^�[���̍\����
typedef struct {
	MOVE::MoveType ptn;
	double vx, vy, ax, ay;
	int dt;
}MovePtn;
//�e�̃p�^�[���̍\����
typedef struct {
	BULLET::BulletType ptn;
	double vx, vy, ax, ay;
	int damage, color;
}BulletPtn;
//�G�p�^�[���̍\����
typedef struct {
	MovePtn move;
	BulletPtn bullet;
	int hp;
	int *gpHandle;
	double sizeX, sizeY;
	double exRate;
}EnemyPtn;
MovePtn initMoveConstant(double v, int degree);
MovePtn initMoveAccelarate(double v, double a, int degree);
MovePtn initMoveStop(int dt);
BulletPtn initBulletConstant(double v, int degree, int color = COLOR::white, int damage = 1);
BulletPtn initBulletAccelarate(double v, double a, int degree, int color = COLOR::white, int damage = 1);
EnemyPtn initEnemy(int hp, MovePtn mv, BulletPtn bl, Graphic enemy, double exRate = 0.5);
//stage1�̃p�^�[��
namespace STG1 {
	//�����̃p�^�[��
	MovePtn mvL1 = initMoveConstant(1, 45);
	MovePtn mvR1 = initMoveConstant(2, 180-45);
	MovePtn mv2 = initMoveAccelarate(5, 0.1, 90);
	//�e�̃p�^�[��
	BulletPtn bl1 = initBulletConstant(1, 90);
	BulletPtn bl2 = initBulletAccelarate(1, -0.03, -90);
	//�G�̃p�^�[��
	EnemyPtn fairyL1 = initEnemy(100, mvL1, bl1, GRAPHIC::fairy);
	EnemyPtn fairyR1 = initEnemy(100, mvR1, bl2, GRAPHIC::fairy);
}
//�v���C���[�̍\����
typedef struct {
	double x, y;//���W
	int hp;//�̗�
	int r;//���a
	int size_x, size_y;//�摜�̑傫��
	double v;//4�����̑���
	double exRate;//�g�嗦
	int interval;//�V���b�g�̊Ԋu
	int sCount;//�V���b�g��ł��n�߂�J�E���g
	int index;//�摜�n���h���̃C���f�b�N�X
	int *graph;//�摜�n���h��
}Player;

//�e�̍\����
typedef struct {
	BulletPtn ptn;
	double x,y,r;
}Bullet;

//���e�̍\���̃��X�g
typedef struct PlayerBullet {
	Bullet s;//�v���C���[�̃V���b�g�f�[�^
	struct PlayerBullet *before;//�O�̃f�[�^�ւ̃|�C���^
	struct PlayerBullet *next;//���̃f�[�^�ւ̃|�C���^
}PBullet;

//�G�e�̃��X�g
typedef struct EnemyBullet {
	Bullet s;//�e�f�[�^
	struct EnemyBullet *before;//�O�̃f�[�^�ւ̃|�C���^
	struct EnemyBullet *next;//���̃f�[�^�ւ̃|�C���^
}EBullet;

//�G�̍\����
typedef struct {
	double x, y;//���W
	EnemyPtn ptn;//�p�^�[��
	double r;//���a
}Enemy;

//�G�̍\���̃��X�g
typedef struct EnemyList {
	Enemy e;//�G�f�[�^
	struct EnemyList *before;//�O�̃f�[�^�ւ̃|�C���^
	struct EnemyList *next;//���̃f�[�^�ւ̃|�C���^
}Elist;
//WorldCounter
typedef struct {
	int count;//�X�e�[�W�̃J�E���^
	int index;//���n��G�f�[�^�̃C���f�N�X
}WldCounter;


//�������u��(pushNow),�������u��(pullNow),����ȊO�̏��(other)
enum KeyState {
	pushNow = 1,
	pullNow = -1,
	other = 0
};
typedef struct _keys {
	char preKeys[256];//�O�̃L�[�̉������(1or0)
	char nowKeys[256];//���݂̃L�[�̉������(1or0)
	KeyState state[256];//���݂̃L�[�̏��
}Key;




/*�ȉ��Ɋ֐��̃v���g�^�C�v�錾������*/
/*���X�g�n*/
//�ėp
//head->...->tail�̖�����node��ǉ�
template<typename T>
void pushBack(T **node, T ** head, T **tail);
//node��head-tail�̃��X�g�������(node�̎��̃m�[�h��Ԃ�)
template<typename T>
T* deleteNode(T **node, T **head, T **tail);
//head-tail�̃��X�g�̃m�[�h�����ׂď���
template<typename T>
void deleteAllNode(T **head, T **tail);
//�G
void addEnemy(Enemy e);
Elist* delEnemy(Elist *p);
void delAllEnemy();
//�G�e
void addEnemyBullet(Bullet b);
EBullet* delEnemyBullet(EBullet *p);
void delAllEnemyBullet();
//�v���C���[�e
void addPlayerBullet(Bullet b);
PBullet* delPlayerBullet(PBullet *p);
void delAllPlayerBullet();

/*�`��n*/
void drawHPBar(double x, double y, double hp_per, int vertical);
void drawHPCircle(double x, double y, double hp_per, double size_per);
void drawUI(Player p);
void drawPlayer(Player *p);
void drawPlayerBullet();
void drawEnemy();
void drawEnemyBullet();
/*�X�V�n*/
void movePlayer(Player *p);
void calcPlayerBullet();
void calcEnemy();
void calcEnemyBullet();
/*�ėp�֐�*/
int Btwn(int p1, int p, int p2);
void rotate2d(double *x, double *y, double deg);
int isInWall(double x, double y, double blank);
double getAngle(Bullet b, Player p);
/*create�n*/
void createPlayerShot(Player *p);
void createEnemyShot(Enemy e, double r);
int genEnemies(int *cnt, double *x, double *y, EnemyPtn *ePtn, int n);
/*���@*/
Player initializePlayer();
/*�G*/
/*�L�[����*/
void updateKeys();
/*�����蔻��*/
template<typename T, typename U>
int onCollisionCircle(T obj1, U obj2);
void collisionEnemyAndPlayerShot();
void collisionPlayerAndEnemy(Player *p);
void collisionPlayerAndEnemyShot(Player *p);
/*�Q�[���i�s*/
void gameStage1(Player *p);
void gameEnd(Player *p);
/*�摜*/

Graphic initGraph(const char *path, int width, int height, int numSliceX, int numSliceY);
void getHandle(Graphic *g);
void loadGraphs();
/*�p�^�[��*/
//����
MovePtn initMoveConstant(double v, int degree);
MovePtn initMoveAccelarate(double v, double a, int degree);
MovePtn initMoveStop(int dt);
//�e
BulletPtn initBulletConstant(double v, int degree, int color, int damage);
BulletPtn initBulletAccelarate(double v, double a, int degree, int color, int damage);
//�G
EnemyPtn initEnemy(int hp, MovePtn mv, BulletPtn bl, Graphic enemy, double exRate);
//���[���h�J�E���^�[
void initWldCounter();




