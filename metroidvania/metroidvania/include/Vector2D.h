#pragma once



class Vector2D {
public:
	float x; 
	float y; 

	Vector2D();
	Vector2D(float x, float y);

	Vector2D& add(const Vector2D& v);
	Vector2D& sub(const Vector2D& v);
	Vector2D& mul(const Vector2D& v);
	Vector2D& div(const Vector2D& v);

	friend Vector2D& operator+(Vector2D& v1, Vector2D& v2);
	friend Vector2D& operator-(Vector2D& v1, Vector2D& v2);
	friend Vector2D& operator*(Vector2D& v1, Vector2D& v2);
	friend Vector2D& operator/(Vector2D& v1, Vector2D& v2);

	Vector2D& operator+=(const Vector2D& v);
	Vector2D& operator-=(const Vector2D& v);
	Vector2D& operator*=(const Vector2D& v);
	Vector2D& operator/=(const Vector2D& v);

	Vector2D& operator*(const int& i);
	Vector2D& zero();
};