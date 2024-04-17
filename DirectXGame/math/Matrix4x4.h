#pragma once
#include"Vector3.h"
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
	// 平行移動行列
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	// 拡大縮小行列
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	// 回転行列
	static Matrix4x4 MakeRotateXMatrix(float& theta);
	static Matrix4x4 MakeRotateYMatrix(float& theta);
	static Matrix4x4 MakeRotateZMatrix(float& theta);
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	static Matrix4x4 Inverse(const Matrix4x4& m);

	// 座標系変換
	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	// 正射影行列
	static Matrix4x4 MakeOrthographicMatrix(float l, float t, float r, float b, float nearClip, float farClip);

	// 透視投影行列
	static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	// ビューポート変換行列
	static Matrix4x4 MakeViewportMatrix(float l, float t, float w, float h, float minDepth, float maxDepth);
};
