#include "CollisionManager.h"
#include<cmath>

CollisionManager* CollisionManager::GetInstance(){
	static CollisionManager* instance;
	if (!instance){
		instance = new CollisionManager();
	}
	return instance;
}

CollisionManager::CollisionManager(){

}

CollisionManager::~CollisionManager(){

}

void CollisionManager::Update(){
	CheckAllCollisions();
}

void CollisionManager::CheckAllCollisions(){
	for (auto itrA = colliders_.begin(); itrA != colliders_.end(); ++itrA){
		Collider* colA = *itrA;
		for (auto itrB = std::next(itrA); itrB != colliders_.end(); ++itrB){
			Collider* colB = *itrB;
			//衝突判定
			CheckCollisionPair(colA, colB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB){
	// 衝突フィルタリング
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0){
		return;
	}

	// 距離の計算
	Vector3 colA_Pos = colliderA->GetWorldPosition();
	Vector3 colB_Pos = colliderB->GetWorldPosition();
	float distance = CheckDistance(colB_Pos , colA_Pos);

	// 球と球の当たり判定
	if (IsOnCollision(distance, colliderA->GetRadius(), colliderB->GetRadius())){
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}

float CollisionManager::CheckDistance(Vector3 v1, Vector3 v2){
	float distanceX, distanceY, distanceZ;
	distanceX = v2.x - v1.x;
	distanceY = v2.y - v1.y;
	distanceZ = v2.z - v1.z;
	return std::sqrt(distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ);
}

bool CollisionManager::IsOnCollision(float distance, float radius1, float radius2){
	return distance <= (radius1 + radius2);
}

void CollisionManager::RemoveCollider(Collider* collider){
	// Collider ポインタをコライダーリストから削除する
	auto it = std::find(colliders_.begin(), colliders_.end(), collider);
	if (it != colliders_.end()){
		colliders_.erase(it);
	}
}

void CollisionManager::SetCollider(Collider* collider){
	colliders_.push_back(collider); 
}