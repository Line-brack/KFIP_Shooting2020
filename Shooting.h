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


//���[���h�J�E���^�[
void initWldCounter();




