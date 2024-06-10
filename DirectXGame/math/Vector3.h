#pragma once
#include <functional>

struct Matrix4x4;

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final{
    float x;
    float y;
    float z;


    Vector3 operator*(const float& scalar) const;
    Vector3 operator*=(const float& scalar);
    Vector3 operator*(const Vector3& other) const;
    Vector3 operator*=(const Vector3& other);
    friend Vector3 operator*(float scalar, const Vector3& v);

    Vector3 operator/(const float& scalar) const;
    Vector3 operator/=(const float& scalar);
    Vector3 operator/(const Vector3& other)const;
    Vector3 operator/=(const Vector3& other);
    friend Vector3 operator/(float scalar, const Vector3& v);

    // ベクトルの加算
    Vector3 operator+(const float& scalar) const;
    Vector3 operator+=(const float& scalar);
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator+=(const Vector3& other);
    friend Vector3 operator+(float scalar, const Vector3& v);

    // ベクトルの減算
    Vector3 operator-(const float& scalar) const;
    Vector3 operator-=(const float& scalar);
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator-=(const Vector3& other);
    friend Vector3 operator-(float scalar, const Vector3& v);

    bool operator==(const Vector3& other) const{
        return x == other.x && y == other.y && z == other.z;
    }
};

struct Vector3Hasher{
    std::size_t operator()(const Vector3& v) const{
        return std::hash<float>()(v.x) ^ std::hash<float>()(v.y) ^ std::hash<float>()(v.z);
    }
};