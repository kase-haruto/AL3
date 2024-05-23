#include"WorldTransform.h"

void WorldTransform::UpdateMatrix (bool isTransfer) {
	//アフィン行列の作成
	matWorld_ = Matrix4x4::MakeAffineMatrix(scale_, rotation_, translation_);
	
	if (parent_){
		matWorld_  = Matrix4x4::Multiply(matWorld_,parent_->matWorld_);	
	}

	if (isTransfer){
		//定数バッファに転送する
		TransferMatrix();
	}
}