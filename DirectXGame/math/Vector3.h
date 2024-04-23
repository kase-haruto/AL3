#pragma once
struct Matrix4x4;

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final{
    float x;
    float y;
    float z;

    static  float Length(const Vector3& v);

    static Vector3 Normalize(const Vector3& v);

    static float Norm(Vector3 v);

    static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

    Vector3 operator*(const float& scalar) const;
    Vector3 operator*=(const float& scalar);
    Vector3 operator*(const Vector3& other) const;
    Vector3 operator*=(const Vector3& other);

    Vector3 operator/(const float& scalar) const;
    Vector3 operator/=(const float& scalar);
    Vector3 operator/(const Vector3& other)const;
    Vector3 operator/=(const Vector3& other);

    // ベクトルの加算
    Vector3 operator+(const float& scalar) const;
    Vector3 operator+=(const float& scalar);
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator+=(const Vector3& other);

    // ベクトルの減算
    Vector3 operator-(const float& scalar) const;
    Vector3 operator-=(const float& scalar);
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator-=(const Vector3& other);
};