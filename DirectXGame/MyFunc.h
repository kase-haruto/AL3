#pragma once
#include"Vector3.h"
#include"Matrix4x4.h"

/// <summary>
/// 内積
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
float Dot(const Vector3& v1,const Vector3& v2);
/// <summary>
/// ベクトルの距離
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
float Length(const Vector3& v);
/// <summary>
/// 線形補完
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
float Lerp(float v1, float v2, float t);
/// <summary>
/// 球面線形補完
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);
/// <summary>
/// 正規化
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
Vector3 Normalize(const Vector3& v);
/// <summary>
/// ベクトルの大きさ
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
float Norm(Vector3 v);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);