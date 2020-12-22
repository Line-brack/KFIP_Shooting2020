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
	static const char* path = "graphic/player_dot_rev.png";
}
namespace COLOR {
	static const int white = GetColor(255, 255, 255);
	static const int red = GetColor(255, 0, 0);
	static const int green = GetColor(0, 255, 0);
	static const int blue = GetColor(0, 0, 255);
	static const int gray = GetColor(128, 128, 128);
}
namespace GRAPHIC {
	static const char* circle = "graphic/img_en.png";
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

//�e�̍\����
typedef struct {
	double x, y;//���W
	double vx, vy;//���x
	double r;//���a
	int color;//�F
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
	double vx, vy;//���x
	int hp;//�q�b�g�|�C���g
	double r;//���a
	int size_x, size_y;//�摜�̑傫��
	double exRate;//�g�嗦
	int *graph;//�摜�n���h��(2�R�}��)
}Enemy;

//�G�̍\���̃��X�g
typedef struct EnemyList {
	Enemy e;//�G�f�[�^
	struct EnemyList *before;//�O�̃f�[�^�ւ̃|�C���^
	struct EnemyList *next;//���̃f�[�^�ւ̃|�C���^
}Elist;



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
void drawPlayer(Player *p);
void drawPlayerBullet();
void drawEnemy(int count);
void drawEnemyBullet();
/*�X�V�n*/
void movePlayer(Player *p,int count);
void calcPlayerBullet();
void calcEnemy();
void calcEnemyBullet();
/*�ėp�֐�*/
int Btwn(int p1, int p, int p2);
void rotate2d(double *x, double *y, double deg);
int isInWall(double x, double y, double blank);
double getAngle(Bullet b, Player p);
/*create�n*/
void createPlayerShot(Player *p, int count);
void createEnemyShot(Player p, int count);

