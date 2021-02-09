#include "DxLib.h"
#include "Vec2d.h"
#include <math.h>
#define PI acos(-1)
//�R���X�g���N�^(x,y)
Vec2d::Vec2d(double x, double y) {
	this->x = x, this->y = y;
}
//�R���X�g���N�^
Vec2d::Vec2d() {}
//+���Z�q�̃x�N�g�����Z
Vec2d Vec2d::operator + (Vec2d other) {
	return Vec2d(this->x + other.x, this->y + other.y);
}
//+=���Z�q�̃x�N�g�����Z�Ƒ��
void Vec2d::operator += (Vec2d other) {
	this->x += other.x;
	this->y += other.y;
}
//-���Z�q�̃x�N�g�����Z
Vec2d Vec2d::operator - (Vec2d other) {
	return Vec2d(this->x - other.x, this->y - other.y);
}
//-=���Z�q�̃x�N�g�����Z�Ƒ��
void Vec2d::operator -= (Vec2d other) {
	this->x -= other.x;
	this->y -= other.y;
}
//*���Z�q�̃x�N�g���̓���(�E��Vec2d�^)
double Vec2d::operator * (Vec2d other) {
	return this->x*other.x + this->y*other.y;
}
//*���Z�q�̃x�N�g���̒萔�{(�E��double�^)
Vec2d Vec2d::operator * (double k) {
	return Vec2d(k*this->x, k*this->y);
}
//���̃x�N�g���Ƃ̋���
double Vec2d::dist(Vec2d other) {
	return hypot(other.x - this->x, other.y - this->y);
}
//�x�N�g���̑傫��
double Vec2d::mag() {
	return hypot(this->x, this->y);
}
//rad��]�������ʂ�Ԃ�
Vec2d Vec2d::rot(double rad) {
	return Vec2d(this->x*cos(rad) - this->y*sin(rad), this->x*sin(rad) + this->y*cos(rad));
}
//�ɍ��W�̒l������x-y���W�n�ɒ����đ��(v:�Ȕ��a,dig:�Ίp(��))
void Vec2d::setPolar(double v, double dig) {
	this->x = v * cos(dig / 180 * PI);
	this->y = v * sin(dig / 180 * PI);
}
//�_�Ɛ����̍ŒZ����,(���̒l�Ő����ɉ������Ƃ��̌�_���Ȃ�)
double Vec2d::distToLine(Vec2d p0, Vec2d p1) {
	if (round(p0.y - p1.y) == 0) {//y=k�̏ꍇ
		double dist = 0;
		dist = fabs(p0.y - this->y);
		if (Btwn(p0.x, this->x, p1.x))//�����͈͓̔�
			return dist;
		return -dist;//�����͈̔͊O
	}
	if (round(p0.x - p1.x) == 0) {//x=k�̏ꍇ
		double dist = 0;
		dist = fabs(p0.x - this->x);
		if (Btwn(p0.y, this->y, p1.y))//�����͈͓̔�
			return dist;
		return -dist;//�����͈̔͊O
	}
	double a, b, _a, _b;
	Vec2d c;
	a = (p1.y - p0.y) / (p1.x - p0.x);//�����̌X��
	b = p1.y - a * p1.x;//�����̐ؕ�
	_a = -1 / a;//�����ɐ����Ȓ����̌X��
	_b = this->y - _a * this->x;//�_����̐����̐ؕ�
	c.x = (_b - b) / (a - _a);//�����Ɛ����̌�_��x
	c.y = _a * c.x + _b;//�����Ɛ����̌�_��y
	if (Btwn(p0.x, c.x, p1.x) && Btwn(p0.y, c.y, p1.y))//��_�������͈͓̔�
		return this->dist(c);
	return -this->dist(c);//��_�������͈̔͊O
}
//�_�ƒ����̓_�𐂒��ɉ������Ƃ��̌�_,
Vec2d Vec2d::ClossToLine(Vec2d p0, Vec2d p1) {
	double a, b, _a, _b;
	Vec2d c;
	a = (p1.y - p0.y) / (p1.x - p0.x);
	b = p1.y - a * p1.x;
	_a = -1 / a;
	_b = this->y - _a * this->x;
	c.x = (_b - b) / (a - _a);
	c.y = _a * c.x + _b;
	return c;//��_�̈ʒu�x�N�g����Ԃ�
}
// p1����p2�͈̔͂�p������Ƃ�1��Ԃ�
int Btwn(int p1, int p, int p2) {
	if (p2>p1 && (p1 <= p) && (p <= p2))
		return 1;
	if (p2 < p1 && (p2 <= p) && (p <= p1))
		return 1;
	return 0;
}