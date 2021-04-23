#include "../include/Vector2D.h"

Vector2D::Vector2D() {
	x = y = 0;
}

Vector2D::Vector2D(float x, float y) {
	this->x = x; 
	this->y = y; 
}

Vector2D& Vector2D::add(const Vector2D& v) {
	this->x += v.x;
	this->y += v.y;
	return *this; 
}
Vector2D& Vector2D::sub(const Vector2D& v) {
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}
Vector2D& Vector2D::mul(const Vector2D& v) {
	this->x *= v.x;
	this->y *= v.y;
	return *this;
}
Vector2D& Vector2D::div(const Vector2D& v) {
	this->x /= v.x;
	this->y /= v.y;
	return *this;
}

Vector2D& operator+(Vector2D& v1, Vector2D& v2) {
	return v1.add(v2);
}
Vector2D& operator-(Vector2D& v1, Vector2D& v2) {
	return v1.sub(v2);
}
Vector2D& operator*(Vector2D& v1, Vector2D& v2) {
	return v1.mul(v2);
}
Vector2D& operator/(Vector2D& v1, Vector2D& v2) {
	return v1.div(v2);
}

Vector2D& Vector2D::operator+=(const Vector2D& v) {
	return this->add(v);
}
Vector2D& Vector2D::operator-=(const Vector2D& v) {
	return this->sub(v);
}
Vector2D& Vector2D::operator*=(const Vector2D& v) {
	return this->mul(v);
}
Vector2D& Vector2D::operator/=(const Vector2D& v) {
	return this->div(v);
}

Vector2D& Vector2D::operator*(const int& i) {
	this->x *= i;
	this->y *= i;
	return *this; 
}

Vector2D& Vector2D::zero() {
	this->x = 0;
	this->y = 0;
	return *this; 
}
