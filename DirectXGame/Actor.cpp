#include "Actor.h"

void Actor::Draw (ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_,
				  viewProjection,
				  textuerHandle_
	);
}