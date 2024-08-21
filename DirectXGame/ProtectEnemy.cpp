#include "ProtectEnemy.h"

void ProtectEnemy::Initialize(const std::vector<Model*>& models){
	Enemy::Initialize(models);
}

void ProtectEnemy::Update(){
	Enemy::Update();
}

void ProtectEnemy::Draw(const ViewProjection& viewProjection){
	Enemy::Draw(viewProjection);
}

void ProtectEnemy::OnCollision([[maybe_unused]] Collider* other){

}

Vector3 ProtectEnemy::GetCenterPos()const {
	return Enemy::GetCenterPos();
}