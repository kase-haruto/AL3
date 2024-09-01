#include "AttackEnemy.h"
#include "MyFunc.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#include <cmath>
#include"CollisionManager.h"

void AttackEnemy::Initialize(const std::vector<Model*>& models){
    Enemy::Initialize(models);
    life_ = 1;
}

void AttackEnemy::Update(){
#ifdef _DEBUG
    ImGui::Begin("enemy");
    ImGui::DragFloat3("armPos", &partsTransform_[int(Parts::arm)]->translation_.x, 0.01f);
    ImGui::DragFloat3("rotation_", &partsTransform_[int(Parts::arm)]->rotation_.x, 0.01f);
    ImGui::Text("life:%d", life_);
    ImGui::End();
#endif // _DEBUG

    if (isAlive_){
        coolTime_--;
        if (coolTime_ <= 0 && target_->GetIsAlive()){
            // ターゲットとの距離を計算
            Vector3 toTarget = target_->GetWorldPosition() - worldTransform_.translation_;
            float distanceToTarget = toTarget.Length(); // ターゲットまでの距離を計算
            float attackThreshold = 10; // 攻撃を開始する距離の閾値

            if (distanceToTarget <= attackThreshold){
                // ターゲットが近ければ攻撃を開始
                isAttack_ = true;
            }
        }

        if (life_ <= 0){
            isAlive_ = false;
        }

        if (isAttack_){
            // 攻撃時の行動
            Attack();
        } else{
            // 通常時の行動
            Enemy::ArmWave();

            if (target_->GetIsAlive()){
                MoveForTarget();
            } else{
                MoveForStronghold();
            }
        }

        for (const auto& transform : partsTransform_){
            transform->UpdateMatrix();
        }
        worldTransform_.UpdateMatrix();
    }
}



void AttackEnemy::Draw(const ViewProjection& viewProjection){
    if (isAlive_){
        Enemy::Draw(viewProjection);
    }
}

void AttackEnemy::OnCollision([[maybe_unused]] Collider* other){
    // 衝突時の処理をここに追加
}

Vector3 AttackEnemy::GetCenterPos() const{
    const Vector3 offset = {0.0f, 1.0f, 0.0f};
    Vector3 worldPos = Matrix4x4::Transform(offset, worldTransform_.matWorld_);
    return worldPos;
}

void AttackEnemy::SetTarget(Player* pos){
    target_ = pos;
}

void AttackEnemy::MoveForTarget(){
    if (!target_) return;

    // ターゲットへのベクトルを計算
    Vector3 toTarget = target_->GetCenterPos() - worldTransform_.translation_;
    toTarget.y = 0.0f; // Y成分を無視して水平面内のベクトルにする

    float distanceToTarget = toTarget.Length(); // ターゲットまでの距離を計算
    float moveThreshold = 10.0f; // 移動を開始する閾値（必要に応じて調整）

    // 一定距離以上離れている場合のみ移動
    if (distanceToTarget > moveThreshold){
        toTarget = Normalize(toTarget); // ベクトルを正規化

        float t = 0.05f; // 補間係数
        velocity_ = Normalize(velocity_); // 現在の速度ベクトルを正規化
        velocity_.y = 0.0f; // Y成分を無視
        velocity_ = Slerp(velocity_, toTarget, t) * speed_;

        // 回転を更新 (Y軸回りの回転のみ)
        worldTransform_.rotation_.y = std::atan2(-velocity_.x, -velocity_.z);

        // 移動
        worldTransform_.translation_ += velocity_;
    }
}

void AttackEnemy::MoveForStronghold(){
    if (!strongholdPtr) return;

    // 拠点へのベクトルを計算
    Vector3 toStronghold = strongholdPtr->GetWorldPosition() - worldTransform_.translation_;
    toStronghold.y = 0.0f; // Y成分を無視して水平面内のベクトルにする
    toStronghold = Normalize(toStronghold); // ベクトルを正規化

    float t = 0.05f; // 補間係数
    velocity_ = Normalize(velocity_); // 現在の速度ベクトルを正規化
    velocity_.y = 0.0f; // Y成分を無視
    velocity_ = Slerp(velocity_, toStronghold, t) * speed_;

    // 回転を更新 (Y軸回りの回転のみ)
    worldTransform_.rotation_.y = std::atan2(-velocity_.x, -velocity_.z);

    // 移動
    worldTransform_.translation_ += velocity_;
}

void AttackEnemy::Attack(){
    auto& translation = worldTransform_.translation_;
    auto& rotation = worldTransform_.rotation_;

    // ターゲットの位置
    Vector3 targetPosition = target_->GetCenterPos();

    // ターゲットへの方向ベクトルを計算
    Vector3 direction = targetPosition - translation;
    direction.Normalize();  // 方向ベクトルを正規化

    // ジャンプのピークを定義
    static const float jumpPeak = 2.0f;  // 最大ジャンプ高さ
    static const float jumpDuration = 1.0f;  // ジャンプにかかる時間
    static float jumpTime = 0.0f;

    // ジャンプ中のY位置の計算 (ジャンプのピークを頂点とした放物線運動)
    float jumpY = jumpPeak * (1.0f - std::powf((2.0f * jumpTime / jumpDuration) - 1.0f, 2));

    // X軸、Z軸の移動を考慮した位置更新
    translation += direction * 0.1f;  // ターゲットの方向に向かって移動
    translation.y = jumpY;  // Y軸のジャンプ運動

    // ターゲットに向かって回転
    rotation.y = std::atan2(-direction.x, -direction.z);

    // ジャンプ時間の更新
    jumpTime += 0.016f;  // 約60FPSの場合の1フレーム分の時間

    // ジャンプモーションが終了した場合
    if (jumpTime >= jumpDuration){
        // 本体の位置と回転をリセット
        translation.y = 0.0f; // ジャンプ後にY座標をリセット
        rotation.y = 0.0f;
        isAttack_ = false;
        coolTime_ = 180;
        jumpTime = 0.0f;  // ジャンプ時間をリセット
    }
}



void AttackEnemy::SetPlayerStronghold(PlayerStronghold* stronghold){
    strongholdPtr = stronghold;
}