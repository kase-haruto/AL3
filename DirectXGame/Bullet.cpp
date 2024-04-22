#include "Bullet.h"
#include<cassert>



void Bullet::Draw (const ViewProjection& viewProjection) {
	model_->Draw ( worldTransform_,
				   viewProjection,
				   textuerHandle_
				 );
}