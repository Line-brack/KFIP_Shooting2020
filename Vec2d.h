#pragma once


class Vec2d {
public:
	double x, y;
	Vec2d();//�R���X�g���N�^(�������Ȃ�)
	Vec2d(double x, double y);//�R���X�g���N�^(x,y)
	Vec2d operator + (Vec2d other);//���Z
	void operator += (Vec2d other);//���Z�Ƒ��
	Vec2d operator - (Vec2d other);//���Z
	void operator -= (Vec2d other);//���Z�Ƒ��
	double operator * (Vec2d other);//����
	Vec2d operator * (double k);//�萔�{
	double dist(Vec2d other);//����
	double mag();//�傫��
	Vec2d rot(double rad);//��]���ʂ�Ԃ�
	double distToLine(Vec2d p0, Vec2d p1);//�_�Ɛ���(����)�̋����v�Z
	Vec2d ClossToLine(Vec2d p0, Vec2d p1);//�_�̐����ƒ����̌�_�̌v�Z
	void setPolar(double v, double dig);//�ɍ��W�̓��͂�x-y���W�n�ɂ��đ��
};