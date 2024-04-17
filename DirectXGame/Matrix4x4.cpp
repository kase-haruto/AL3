#include"Matrix4x4.h"
#include<cmath>
#include<assert.h>
#include"Vector4.h"

float cot(float angle) { return 1 / std::tan(angle); }

// 平行移動行列
Matrix4x4 Matrix4x4::MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, translate.x, translate.y, translate.z, 1};
	return result;
}

// 拡大縮小行列
Matrix4x4 Matrix4x4::MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = {scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, 1};
	return result;
}

// 回転行列
Matrix4x4 Matrix4x4::MakeRotateXMatrix(float& theta) {
	Matrix4x4 result = {1, 0, 0, 0, 0, std::cos(theta), std::sin(theta), 0, 0, -std::sin(theta), std::cos(theta), 0, 0, 0, 0, 1};

	return result;
}

Matrix4x4 Matrix4x4::MakeRotateYMatrix(float& theta) {
	Matrix4x4 result = {std::cos(theta), 0, -std::sin(theta), 0, 0, 1, 0, 0, std::sin(theta), 0, std::cos(theta), 0, 0, 0, 0, 1};
	return result;
}

Matrix4x4 Matrix4x4::MakeRotateZMatrix(float& theta) {
	Matrix4x4 result = {std::cos(theta), std::sin(theta), 0, 0, -std::sin(theta), std::cos(theta), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	return result;
}

Matrix4x4 Matrix4x4::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 affineMatrix;
	Vector3 rotation = rotate;
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotation.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotation.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotation.z);
	Matrix4x4 rotateMatrix = Matrix4x4::Multiply(Matrix4x4::Multiply(rotateXMatrix, rotateYMatrix), rotateZMatrix);

	affineMatrix = Matrix4x4::Multiply(Matrix4x4::Multiply(scaleMatrix, rotateMatrix), translateMatrix);

	return affineMatrix;
}

Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& m) {
	Matrix4x4 result;

	// 行列Aを複製する
	Matrix4x4 temp = m;

	// ガウス・ジョルダン法を用いて逆行列を計算
	for (int i = 0; i < 4; ++i) {
		// 対角要素を1にする
		float divisor = temp.m[i][i];
		for (int j = 0; j < 4; ++j) {
			temp.m[i][j] /= divisor;
			result.m[i][j] /= divisor;
		}

		// 対角要素以外を0にする
		for (int k = 0; k < 4; ++k) {
			if (k != i) {
				float multiplier = temp.m[k][i];
				for (int j = 0; j < 4; ++j) {
					temp.m[k][j] -= temp.m[i][j] * multiplier;
					result.m[k][j] -= result.m[i][j] * multiplier;
				}
			}
		}
	}

	return result;
}

// 座標系変換
Vector3 Matrix4x4::Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result = {0, 0, 0};

	// 同次座標系への変換
	// 変換行列を適用
	Vector4 homogeneousCoordinate(
	    vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0],
	    vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1],
	    vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2],
	    vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3]);

	// 同次座標系から3次元座標系に戻す
	float w = homogeneousCoordinate.w;
	assert(w != 0.0f); // wが0でないことを確認
	result.x = homogeneousCoordinate.x / w;
	result.y = homogeneousCoordinate.y / w;
	result.z = homogeneousCoordinate.z / w;

	return result;
}

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	return result;
}

// 正射影行列
Matrix4x4 Matrix4x4::MakeOrthographicMatrix(float l, float t, float r, float b, float nearClip, float farClip) {
	Matrix4x4 result;
	result = {2 / (r - l), 0, 0, 0, 0, 2 / (t - b), 0, 0, 0, 0, 1 / (farClip - nearClip), 0, (l + r) / (l - r), (t + b) / (b - t), nearClip / (nearClip - farClip), 1};
	return result;
}

// 透視投影行列
Matrix4x4 Matrix4x4::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result;
	result = {1 / aspectRatio * cot(fovY / 2), 0, 0, 0, 0, cot(fovY / 2), 0, 0, 0, 0, farClip / (farClip - nearClip), 1, 0, 0, -nearClip * farClip / (farClip - nearClip), 0};
	return result;
}

// ビューポート変換行列
Matrix4x4 Matrix4x4::MakeViewportMatrix(float l, float t, float w, float h, float minDepth, float maxDepth) {
	Matrix4x4 result;
	result = {w / 2, 0, 0, 0, 0, -h / 2, 0, 0, 0, 0, maxDepth - minDepth, 0, l + (w / 2), t + (h / 2), minDepth, 1};
	return result;
}