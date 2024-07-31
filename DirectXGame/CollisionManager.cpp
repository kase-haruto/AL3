#include "CollisionManager.h"
#include"Vector3.h"
#include"MyFunc.h"
#include"GlobalVariables.h"

CollisionManager::CollisionManager(){
	const char* groupName = "Collision";
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "isDrawCollider", isDrawCollider_);
}

void CollisionManager::ApplyGlobalVariables(){
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Collision";

	isDrawCollider_ = globalVariables->GetValue<bool>(groupName, "isDrawCollider");
}

void CollisionManager::Initialize(){
	debugModel_.reset(Model::CreateFromOBJ("collider", true));

}

void CollisionManager::Reset(){
	colliders_.clear();
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB){
	Vector3 colliderA_pos = colliderA->GetCenterPos();
	Vector3 colliderB_pos = colliderB->GetCenterPos();
	float colliderA_radius = colliderA->GetRadius();
	float colliderB_radius = colliderB->GetRadius();
	//差分ベクトル
	Vector3 sub = colliderB_pos - colliderA_pos;
	float distance = Length(sub);
	//球同士の衝突判定
	if (distance <= colliderA_radius + colliderB_radius){
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}

void CollisionManager::CheckAllCollidion(){
	ApplyGlobalVariables();

	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end();++itrA){
		Collider* colliderA = *itrA;

		//イテレータBはイテレータAの次の要素から回す
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end();++itrB){
			Collider* colliderB = *itrB;

			//ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::AddCollider(Collider* collider){
	colliders_.push_back(collider);
}

void CollisionManager::UpdateWorldTransform(){
	if (!isDrawCollider_){
		return;
	}
	//すべてのコライダーのトランスフォームの更新
	for (const auto& collider:colliders_){
		collider->UpdateTransform();
	}
}

void CollisionManager::Draw(const ViewProjection& viewPro){
	if (!isDrawCollider_){
		return;
	}
	for (const auto& collider:colliders_){
		collider->Draw(debugModel_.get(), viewPro);
	}
}