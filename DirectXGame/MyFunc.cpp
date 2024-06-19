#include "MyFunc.h"
#include<cmath>
#include<algorithm>
#include<numbers>
#include<numbers>

float Dot(const Vector3& v1, const Vector3& v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Length(const Vector3& v){
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Normalize(const Vector3& v){
	float length = Length(v);
	if (length != 0.0f){
		return {v.x / length, v.y / length, v.z / length};
	} else{
		return {0.0f, 0.0f, 0.0f}; // ゼロベクトルを返す
	}
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t){
	return v1 + (v2 - v1) * t;
}

float Lerp(float v1, float v2, float t){
	return v1 + (v2 - v1) * t;
}

float LerpShortAngle(float a, float b, float t){
	const float TWO_PI = 2.0f * ( float )std::numbers::pi; // 2π (6.283185307179586)
	const float PI = ( float ) std::numbers::pi;            // π (3.141592653589793)

	// 角度差分を求める
	float diff = b - a;

	// 角度を[-π, π]に補正する
	diff = fmod(diff, TWO_PI);
	if (diff > PI){
		diff -= TWO_PI;
	} else if (diff < -PI){
		diff += TWO_PI;
	}

	// Lerpを使用して補間
	return Lerp(a, a + diff, t);

}

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t){
	Vector3 result;
	Vector3 unit_v1 = Normalize(v1);
	Vector3 unit_v2 = Normalize(v2);

	float dot = Dot(unit_v1, unit_v2);
	dot = std::clamp(dot, 0.0f, 1.0f);
	float theta = acos(dot);
	float sinTheta = sin(theta);
	float sinThetaFrom = sin((1 - t) * theta);
	float sinThetaTo = sin(t * theta);


	//ゼロ除算を防ぐ
	if (sinTheta < 1.0e-5){
		result = unit_v1;
	} else{
		result = (sinThetaFrom / sinTheta) * unit_v1 + (sinThetaTo / sinTheta) * unit_v2;
	}
	float length1 = Length(v1);
	float length2 = Length(v2);
	float length = Lerp(length1, length2, t);

	return result * length;
}


float Norm(Vector3 v){
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m){
	Vector3 result {
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]};
	return result;
}

float DegToRad(float degree){
	return degree * (float)std::numbers::pi / 180.0f;
}