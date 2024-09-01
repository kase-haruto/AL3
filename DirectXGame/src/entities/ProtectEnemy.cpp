#include "ProtectEnemy.h"

#ifdef _DEBUG
#include<imgui.h>
#endif // _DEBUG
#include"EnemyStronghold.h"
#include<numbers>
#include"Player.h"
#include"MyFunc.h"

void ProtectEnemy::Initialize(const std::vector<Model*>& models){
	Enemy::Initialize(models);
	life_ = 15;
}

void ProtectEnemy::Update(){

	if (life_ <= 0){
		isAlive_ = false;
	}

    if (stronghold_){
        Vector3 strongholdPos = stronghold_->GetWorldPosition();
        Vector3 offsetPos {10.0f, 0.0f, -10.0f};  // stronghold からの相対位置

        if (player_&&player_->GetIsAlive()&&player_->GetIsActive()){
            Vector3 playerPos = player_->GetWorldPosition();
            float distanceToPlayer = Length(playerPos - worldTransform_.translation_);

            const float followThreshold = 15.0f;  // プレイヤーを追跡し始める距離
            const float stopThreshold = 8.0f;     // プレイヤーに近づきすぎたら停止する距離

            // タイマーを更新
            timer_++; // deltaTime_ はフレームの経過時間を表す変数

            if (timer_ >= chargeInterval_){
                // タイマーが3秒以上経過したら突進する
                Vector3 directionToPlayer = Normalize(playerPos - worldTransform_.translation_);
                const float chargeSpeed = 5.0f; // 突進の速度
                worldTransform_.translation_ += directionToPlayer * chargeSpeed;

                // 進行方向を向くように回転を更新
                float targetRotationY = std::atan2(-directionToPlayer.x, -directionToPlayer.z);
                worldTransform_.rotation_.y = targetRotationY;

                // タイマーをリセット
                timer_ = 0.0f;
            } else{
                Vector3 direction;
                if (distanceToPlayer < stopThreshold){
                    // プレイヤーが近すぎる場合は停止
                    direction = Vector3(0.0f, 0.0f, 0.0f);
                } else if (distanceToPlayer < followThreshold){
                    // プレイヤーの方に向かって移動
                    direction = Normalize(playerPos - worldTransform_.translation_);
                    const float moveSpeed = 0.2f;
                    worldTransform_.translation_ += direction * moveSpeed;

                    // 進行方向を向くように回転を更新
                    float targetRotationY = std::atan2(-direction.x, -direction.z);
                    worldTransform_.rotation_.y = targetRotationY;
                } else{
                    // stronghold のそばにいる
                    direction = Normalize((strongholdPos + offsetPos) - worldTransform_.translation_);
                    const float moveSpeed = 0.1f;
                    worldTransform_.translation_ += direction * moveSpeed;

                    // プレイヤーの方向を向くように回転を更新
                    Vector3 directionToPlayer = Normalize(playerPos - worldTransform_.translation_);
                    float targetRotationY = std::atan2(-directionToPlayer.x, -directionToPlayer.z);
                    worldTransform_.rotation_.y = targetRotationY;
                }
            }
        } else{
            // プレイヤーがいない場合は強制的に stronghold のそばにいる
            Vector3 directionToStronghold = Normalize((strongholdPos + offsetPos) - worldTransform_.translation_);
            const float moveSpeed = 0.1f;
            worldTransform_.translation_ += directionToStronghold * moveSpeed;

            // 進行方向を向くように回転を更新
            float targetRotationY = std::atan2(-directionToStronghold.x, -directionToStronghold.z);
            worldTransform_.rotation_.y = targetRotationY;
        }
    }



	ArmWave();
	
	worldTransform_.UpdateMatrix();

	for (const auto& transform : partsTransform_){
		transform->UpdateMatrix();
	}
}

void ProtectEnemy::Draw(const ViewProjection& viewProjection){
	if (isAlive_ ){
		Enemy::Draw(viewProjection);
	}
}

void ProtectEnemy::UpdateResultScene(){
	Enemy::ArmWave();

	if (worldTransform_.translation_.z >= -36.0f){
		Move();
	}

	worldTransform_.UpdateMatrix();
	for (auto& partsTransform:partsTransform_){
		partsTransform->UpdateMatrix();
	}
}

void ProtectEnemy::OnCollision([[maybe_unused]] Collider* other){

}

Vector3 ProtectEnemy::GetCenterPos()const {
	return Enemy::GetCenterPos();
}

void ProtectEnemy::SetStronghold(EnemyStronghold* stronghold){
	stronghold_ = stronghold;
}

void ProtectEnemy::SetPlayer(Player* player){
	player_ = player;
}

