#include"WorldTransform.h"

void WorldTransform::UpdateMatrix () {
	//アフィン行列の作成
	matWorld_ = Matrix4x4::MakeAffineMatrix(scale_, rotation_, translation_);
	//定数バッファに転送する
	TransferMatrix();
}