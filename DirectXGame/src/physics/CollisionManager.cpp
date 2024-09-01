#include "CollisionManager.h"
#include "Vector3.h"
#include "MyFunc.h"
#ifdef _DEBUG
#include "GlobalVariables.h"
#endif // _DEBUG

CollisionManager::CollisionManager(){
#ifdef _DEBUG
    const char* groupName = "Collision";
    GlobalVariables* globalVariables = GlobalVariables::GetInstance();
    globalVariables->CreateGroup(groupName);
    globalVariables->AddItem(groupName, "isDrawCollider", isDrawCollider_);
#endif // _DEBUG
}

void CollisionManager::ApplyGlobalVariables(){
#ifdef _DEBUG
    GlobalVariables* globalVariables = GlobalVariables::GetInstance();
    const char* groupName = "Collision";

    isDrawCollider_ = globalVariables->GetValue<bool>(groupName, "isDrawCollider");
#endif // _DEBUG
}

void CollisionManager::Character2Stronghold(Actor* character, BaseStronghold* stronghold){
    Vector3 charcterPos = character->GetCenterPos();
    Vector3 strongholdPos = stronghold->GetCenterPos();
    float characterRadius = character->GetRadius();
    float strongholdRadius = stronghold->GetRadius();

    Vector3 sub = charcterPos - strongholdPos;
    float distance = Length(sub);

    if (distance <= strongholdRadius + characterRadius){
        stronghold->ReduceLife();
    }
}

void CollisionManager::Player2Stronghold(Player* player, EnemyStronghold* enemyStronghold){
    Vector3 playerPos = player->GetCenterPos();
    Vector3 strongholdPos = enemyStronghold->GetCenterPos();
    float playerRadius = player->GetRadius();
    float strongholdRadius = enemyStronghold->GetRadius();
    Vector3 sub = playerPos - strongholdPos;
    float distance = Length(sub);

    if (!enemyStronghold->GetProtectEnemy()->GetIsAlive()){
        if (distance <= strongholdRadius + playerRadius){
            enemyStronghold->SetIsTaked(true);
        }
    }
}

void CollisionManager::Enemy2Stronghold(Enemy* enemy, PlayerStronghold* stronghold){
    Vector3 enemyPos = enemy->GetCenterPos();
    Vector3 strongholdPos = stronghold->GetCenterPos();
    float enemyRadius = enemy->GetRadius();
    float strongholdRadius = stronghold->GetRadius();
    Vector3 sub = enemyPos - strongholdPos;
    float distance = Length(sub);

    if (!stronghold->GetPlayerPtr()->GetIsAlive()){
        if (distance <= strongholdRadius + enemyRadius){
            stronghold->SetIsTaked(true);
        }
    }
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

    Vector3 sub = colliderB_pos - colliderA_pos;
    float distance = Length(sub);

    if (distance <= colliderA_radius + colliderB_radius){
        colliderA->OnCollision(colliderB);
        colliderB->OnCollision(colliderA);
    }
}

void CollisionManager::CheckAllCollidion(){
#ifdef _DEBUG
    ApplyGlobalVariables();
#endif // _DEBUG

    for (auto itrA = colliders_.begin(); itrA != colliders_.end(); ++itrA){
        Collider* colliderA = *itrA;

        for (auto itrB = std::next(itrA); itrB != colliders_.end(); ++itrB){
            Collider* colliderB = *itrB;

            CheckCollisionPair(colliderA, colliderB);
        }
    }
}

void CollisionManager::AddCollider(Collider* collider){
    colliders_.push_back(collider);
}

void CollisionManager::RemoveCollider(Collider* collider){
    colliders_.remove(collider);  // リストから削除
}

void CollisionManager::UpdateWorldTransform(){
    if (!isDrawCollider_){
        return;
    }

    for (const auto& collider : colliders_){
        collider->UpdateTransform();
    }
}

void CollisionManager::Draw(const ViewProjection& viewProjection){
    if (!isDrawCollider_){
        return;
    }

    for (const auto& collider : colliders_){
        collider->Draw(debugModel_.get(), viewProjection);
    }
}
