#pragma once


class Vec2d {
public:
	double x, y;
	Vec2d();//コンストラクタ(何もしない)
	Vec2d(double x, double y);//コンストラクタ(x,y)
	Vec2d operator + (Vec2d other);//加算
	void operator += (Vec2d other);//加算と代入
	Vec2d operator - (Vec2d other);//減算
	void operator -= (Vec2d other);//減算と代入
	double operator * (Vec2d other);//内積
	Vec2d operator * (double k);//定数倍
	double dist(Vec2d other);//距離
	double mag();//大きさ
	Vec2d rot(double rad);//回転結果を返す
	double distToLine(Vec2d p0, Vec2d p1);//点と線分(直線)の距離計算
	Vec2d ClossToLine(Vec2d p0, Vec2d p1);//点の垂線と直線の交点の計算
	void setPolar(double v, double dig);//極座標の入力をx-y座標系にして代入
};