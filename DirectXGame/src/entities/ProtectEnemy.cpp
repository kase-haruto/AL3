#include "ProtectEnemy.h"

#ifdef _DEBUG
#include<imgui.h>
#endif // _DEBUG


void ProtectEnemy::Initialize(const std::vector<Model*>& models){
	Enemy::Initialize(models);
	life_ = 8;
}

void ProtectEnemy::Update(){

	if (isAlive_){
	Enemy::Update();
	}
}

void ProtectEnemy::Draw(const ViewProjection& viewProjection){
	if (isAlive_ ){
		Enemy::Draw(viewProjection);
	}
}

void ProtectEnemy::OnCollision([[maybe_unused]] Collider* other){

}

Vector3 ProtectEnemy::GetCenterPos()const {
	return Enemy::GetCenterPos();
}