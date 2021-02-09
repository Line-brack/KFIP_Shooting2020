#include "DxLib.h"
#include "Vec2d.h"
#include <math.h>
#define PI acos(-1)
//コンストラクタ(x,y)
Vec2d::Vec2d(double x, double y) {
	this->x = x, this->y = y;
}
//コンストラクタ
Vec2d::Vec2d() {}
//+演算子のベクトル加算
Vec2d Vec2d::operator + (Vec2d other) {
	return Vec2d(this->x + other.x, this->y + other.y);
}
//+=演算子のベクトル加算と代入
void Vec2d::operator += (Vec2d other) {
	this->x += other.x;
	this->y += other.y;
}
//-演算子のベクトル減算
Vec2d Vec2d::operator - (Vec2d other) {
	return Vec2d(this->x - other.x, this->y - other.y);
}
//-=演算子のベクトル減算と代入
void Vec2d::operator -= (Vec2d other) {
	this->x -= other.x;
	this->y -= other.y;
}
//*演算子のベクトルの内積(右にVec2d型)
double Vec2d::operator * (Vec2d other) {
	return this->x*other.x + this->y*other.y;
}
//*演算子のベクトルの定数倍(右にdouble型)
Vec2d Vec2d::operator * (double k) {
	return Vec2d(k*this->x, k*this->y);
}
//他のベクトルとの距離
double Vec2d::dist(Vec2d other) {
	return hypot(other.x - this->x, other.y - this->y);
}
//ベクトルの大きさ
double Vec2d::mag() {
	return hypot(this->x, this->y);
}
//rad回転した結果を返す
Vec2d Vec2d::rot(double rad) {
	return Vec2d(this->x*cos(rad) - this->y*sin(rad), this->x*sin(rad) + this->y*cos(rad));
}
//極座標の値を入れてx-y座標系に直して代入(v:曲半径,dig:偏角(°))
void Vec2d::setPolar(double v, double dig) {
	this->x = v * cos(dig / 180 * PI);
	this->y = v * sin(dig / 180 * PI);
}
//点と線分の最短距離,(負の値で垂直に下したときの交点がない)
double Vec2d::distToLine(Vec2d p0, Vec2d p1) {
	if (round(p0.y - p1.y) == 0) {//y=kの場合
		double dist = 0;
		dist = fabs(p0.y - this->y);
		if (Btwn(p0.x, this->x, p1.x))//線分の範囲内
			return dist;
		return -dist;//線分の範囲外
	}
	if (round(p0.x - p1.x) == 0) {//x=kの場合
		double dist = 0;
		dist = fabs(p0.x - this->x);
		if (Btwn(p0.y, this->y, p1.y))//線分の範囲内
			return dist;
		return -dist;//線分の範囲外
	}
	double a, b, _a, _b;
	Vec2d c;
	a = (p1.y - p0.y) / (p1.x - p0.x);//線分の傾き
	b = p1.y - a * p1.x;//線分の切片
	_a = -1 / a;//線分に垂直な直線の傾き
	_b = this->y - _a * this->x;//点からの垂線の切片
	c.x = (_b - b) / (a - _a);//垂線と線分の交点のx
	c.y = _a * c.x + _b;//垂線と線分の交点のy
	if (Btwn(p0.x, c.x, p1.x) && Btwn(p0.y, c.y, p1.y))//交点が線分の範囲内
		return this->dist(c);
	return -this->dist(c);//交点が線分の範囲外
}
//点と直線の点を垂直に下したときの交点,
Vec2d Vec2d::ClossToLine(Vec2d p0, Vec2d p1) {
	double a, b, _a, _b;
	Vec2d c;
	a = (p1.y - p0.y) / (p1.x - p0.x);
	b = p1.y - a * p1.x;
	_a = -1 / a;
	_b = this->y - _a * this->x;
	c.x = (_b - b) / (a - _a);
	c.y = _a * c.x + _b;
	return c;//交点の位置ベクトルを返す
}
// p1からp2の範囲にpがあるとき1を返す
int Btwn(int p1, int p, int p2) {
	if (p2>p1 && (p1 <= p) && (p <= p2))
		return 1;
	if (p2 < p1 && (p2 <= p) && (p <= p1))
		return 1;
	return 0;
}