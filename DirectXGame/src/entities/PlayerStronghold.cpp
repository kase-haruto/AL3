#include "PlayerStronghold.h"

void PlayerStronghold::Initialize(Model* model){
	BaseStronghold::Initialize(model);
}

void PlayerStronghold::Update(){
	
	worldTransform_.UpdateMatrix();
}

void PlayerStronghold::Draw(const ViewProjection& viewProjection){
	BaseStronghold::Draw(viewProjection);
}
