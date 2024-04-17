#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	static float Length(const Vector3& v);

	static Vector3 Normalize(const Vector3& v);

	static float Norm(Vector3 v);

	Vector3 operator*(const float& scalar) const;

	Vector3& operator*=(float scalar);

	Vector3 operator/(const float& divisor) const;

	Vector3& operator/=(float divisor);

	// ベクトルの加算
	Vector3 operator+(const Vector3& other) const;
	Vector3 operator+=(const Vector3& other);

	// ベクトルの減算
	Vector3 operator-(const Vector3& other) const;

	Vector3 operator-=(const Vector3& other);

	// ベクトルの乗算
	Vector3 operator*(const Vector3& other) const;

	Vector3 operator*=(const Vector3& other);
};