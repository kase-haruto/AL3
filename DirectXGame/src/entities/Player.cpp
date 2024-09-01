#include "Player.h"
#include "PlayerRootBehavior.h"
#include "PlayerAttackBehavior.h"
#include "PlayerJumpBehavior.h"
#include "PlayerDashBehavior.h"
#include <cmath>
#include"TextureManager.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include"Enemy.h"
#include "GlobalVariables.h"
#include "LockOn.h"
#include "WeaponBase.h"
#include <iostream>
#include"Audio.h"

#pragma optimize("", off) // 最適化を抑制

Player::Player() {
    partsTransform_.resize(static_cast< int >(Parts::partsCount));
    // 初期化済みのvectorにunique_ptrを配置
    for (auto& part : partsTransform_){
        part = std::make_unique<WorldTransform>();
    }

#ifdef _DEBUG
    const char* groupName = "Player";
    GlobalVariables* globalVariables = GlobalVariables::GetInstance();
    globalVariables->CreateGroup(groupName);

    globalVariables->AddItem(groupName, "Head Translation", partsTransform_[static_cast< int >(Parts::head)]->translation_);
    globalVariables->AddItem(groupName, "ArmL Translation", partsTransform_[static_cast< int >(Parts::L_arm)]->translation_);
    globalVariables->AddItem(groupName, "ArmR Translation", partsTransform_[static_cast< int >(Parts::R_arm)]->translation_);
#endif // _DEBUG
}


Player::~Player(){}

void Player::Initialize(const std::vector<Model*>& models){
    // モデルとワールドトランスフォームの初期化
    Actor::Initialize(models);
    // 各パーツのtransformの初期化
    PartsTransformInit();

    //uiの初期化
    InitializeUi();

    maxLife_ = 15;
    life_ = maxLife_;

    //サウンドデータの読み込み
    hitSoundHandle_ = Audio::GetInstance()->LoadWave("hit.mp3");

    // 通常行動をセットしておく
    ChangeState(std::make_unique<PlayerRootBehavior>(this));
    // 衝突判定の識別ID
    Collider::SetTypeID(static_cast< uint32_t >(CollisionTypeIdDef::kPlayer));
}

void Player::InitializeUi(){
    hpSpriteHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");
    hpBar_.reset(Sprite::Create(hpSpriteHandle_, {70.0f,50.0f}, {1, 1, 1, 1}, {0.0f,0.0f}));
    Vector2 size {500.0f,50.0f};
    hpBar_->SetSize(size);

    iconSpriteHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");
    icon_.reset(Sprite::Create(iconSpriteHandle_, {10,50}));
    Vector2 textureSize {50,50};
    icon_->SetSize(textureSize);
}

void Player::PartsTransformInit(){
    // パーツごとのトランスフォームの初期化
    for (int i = 0; i < static_cast< int >(Parts::partsCount); ++i){
        partsTransform_[i]->Initialize();
    }

    partsTransform_[static_cast< int >(Parts::head)]->translation_ = Vector3 {0.0f, 2.75f, 0.0f};
    partsTransform_[static_cast< int >(Parts::L_arm)]->translation_ = Vector3 {-1.0f, 2.0f, 0.0f};
    partsTransform_[static_cast< int >(Parts::R_arm)]->translation_ = Vector3 {1.0f, 2.0f, 0.0f};

    partsTransform_[static_cast< int >(Parts::body)]->parent_ = &worldTransform_;
    partsTransform_[static_cast< int >(Parts::head)]->parent_ = partsTransform_[static_cast< int >(Parts::body)].get();
    partsTransform_[static_cast< int >(Parts::L_arm)]->parent_ = partsTransform_[static_cast< int >(Parts::body)].get();
    partsTransform_[static_cast< int >(Parts::R_arm)]->parent_ = partsTransform_[static_cast< int >(Parts::body)].get();

    for (const auto& transform : partsTransform_){
        if (transform){
            transform->UpdateMatrix();
        } else{
            std::cerr << "Transform is nullptr!" << std::endl;
        }
    }
}


void Player::Update(){
#ifdef _DEBUG
    ImGui::Begin("player");

    if (weapon_){
        auto weaponRotate = weapon_->GetRotation();
        ImGui::DragFloat3("weaponRotate", &weaponRotate.x, 0.01f);
        weapon_->SetRotation(weaponRotate);

        auto weaponTranslation = weapon_->GetTranslation();
        ImGui::DragFloat3("weaponTranslation", &weaponTranslation.x, 0.01f);
        weapon_->SetTranslation(weaponTranslation);
    }
    ImGui::DragFloat3("playerTranslation", &worldTransform_.translation_.x, 0.01f);
    ImGui::DragFloat3("playerRotation", &worldTransform_.rotation_.x, 0.01f);
    ImGui::Text("life:%d", life_);

    ImGui::End();
#endif // _DEBUG

    if (currentState_){
        currentState_->Update();
    }


    UpdateHPBar();

    //操作されていなければ自動
    if (!isActive_){
        UpdateAutoMode();
    }
    
    //======================================
    //      武器の更新
    //======================================
    if (weapon_){
        weapon_->Update();
        if (!isAttack_){
            weapon_->SetRotation({0.0f, 0.0f, 0.0f});
        }
    }

    // 旋回を滑らかにする
    worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, targetAngle, 0.1f);

    UpdateTransform();
}

void Player::UpdateHPBar(){
    if (isAlive_){
        // life_ の範囲を 0.0f ～ 1.0f に正規化
        float normalizedLife = static_cast< float >(life_) / static_cast< float >(maxLife_);

        Vector2 originalHPBarSize;
        Vector2 iconPosition;
        Vector2 iconSize;

        if (isActive_){
            // アクティブキャラクターの場合の設定
            originalHPBarSize = {500.0f, 50.0f};
            hpBar_->SetPosition({70.0f, 50.0f});
            iconPosition = {10.0f, 50.0f};
            iconSize = {50.0f, 50.0f};
        } else{
            // 非アクティブキャラクターの場合の設定
            originalHPBarSize = {250.0f, 25.0f};
            hpBar_->SetPosition({70.0f, 130.0f});
            iconPosition = {10.0f, 130.0f};
            iconSize = {25.0f, 25.0f};
        }

        // HPバーのサイズを計算
        Vector2 targetSize = {originalHPBarSize.x * normalizedLife, originalHPBarSize.y};

        // HPバーのサイズを線形補完
        Vector2 currentSize = hpBar_->GetSize();
        Vector2 newSize;
        newSize.x = Lerp(currentSize.x, targetSize.x, 0.1f);
        newSize.y = originalHPBarSize.y; // 高さは一定に保つ

        // HPバーのサイズを更新
        hpBar_->SetSize(newSize);

        // アイコンの位置とサイズを設定
        icon_->SetPosition(iconPosition);
        icon_->SetSize(iconSize);
    }
}


void Player::UpdateTransform(){
    // transformの更新
    Actor::Update();
    for (const auto& transform : partsTransform_){
        transform->UpdateMatrix();
    }
}




void Player::Draw(const ViewProjection& viewProjection){
	if (isAlive_){
		for (int i = 0; i < static_cast< int >(Parts::partsCount); ++i){
			if (partsTransform_[i]){
				std::cout << "Drawing part " << i << std::endl;
				models_[i]->Draw(*partsTransform_[i], viewProjection);
			} else{
				std::cerr << "partsTransform_[" << i << "] is nullptr!" << std::endl;
			}
		}

		if (isAttack_){
			weapon_->Draw(viewProjection);
		}
	}
}


void Player::DrawUiSprite(){
    hpBar_->Draw();
    icon_->Draw();
}


#pragma optimize("", on) // 最適化を再度有効化

void Player::MoveInDirection(float speed){
	velocity_ = direction_ * speed;

	if (viewPorjection_){
		Vector3 rotate = viewPorjection_->rotation_;

		Matrix4x4 matRotateY = Matrix4x4::MakeRotateYMatrix(rotate.y);
		Matrix4x4 matRotateZ = Matrix4x4::MakeRotateZMatrix(rotate.z);
		Matrix4x4 matRotate = Matrix4x4::Multiply(matRotateY, matRotateZ);
		velocity_ = Matrix4x4::Transform(velocity_, matRotate);

		float horizontalDistance = sqrtf(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
		worldTransform_.rotation_.x = std::atan2(-velocity_.y, horizontalDistance);

		worldTransform_.translation_ += velocity_;

		targetAngle = std::atan2(velocity_.x, velocity_.z);
	}
	
}

void Player::ChangeState(std::unique_ptr<PlayerBaseBehavior> newState){
	currentState_ = std::move(newState);
	if (currentState_){
		currentState_->Initialize();
	}
}

void Player::ApplyGlobalVariables(){
#ifdef _DEBUG
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	partsTransform_[static_cast< int >(Parts::head)]->translation_ = globalVariables->GetValue<Vector3>(groupName, "Head Translation");
	partsTransform_[static_cast< int >(Parts::L_arm)]->translation_ = globalVariables->GetValue<Vector3>(groupName, "ArmL Translation");
	partsTransform_[static_cast< int >(Parts::R_arm)]->translation_ = globalVariables->GetValue<Vector3>(groupName, "ArmR Translation");
#endif // _DEBUG
}

void Player::SetViewProjection(const ViewProjection* viewProjection){ viewPorjection_ = viewProjection; }

bool Player::HasLockOnTarget()const{ return lockOn_->ExistTarget() ? true : false; }

void Player::OnCollision([[maybe_unused]] Collider* other){

    // 衝突相手の種別IDを取得
    uint32_t typeID = other->GetTypeID();
    if (typeID == static_cast< uint32_t >(CollisionTypeIdDef::kPlayerWeapon)){
        return;//早期リターン
    }

	Actor::OnCollision(other);
   

    // 衝突相手のActorを取得
    if (typeID == static_cast< uint32_t >(CollisionTypeIdDef::kEnemy)){
        Enemy* enemy = static_cast< Enemy* >(other);
        if (enemy->GetIsAlive()){
            if (isActive_){
                hitVoiceHandle_ = Audio::GetInstance()->PlayWave(hitSoundHandle_, false);
            }
            life_--;
        }
    }
}


void Player::UpdateAutoMode(){
    if (enemies_.empty()){
        return; // 敵がいない場合は処理を終了
    }

    Enemy* nearestEnemy = nullptr;
    float minDistance = 10000.0f; // 大きな固定値で初期化

    // 自分の現在位置を取得
    Vector3 playerPosition = worldTransform_.translation_;

    // 敵のリストをループして、一番近い敵を探す
    for (Enemy* enemy : enemies_){
        if (!enemy->GetIsAlive()){
            continue; // 生きていない敵はスキップ
        }

        // 敵との距離を計算
        Vector3 enemyPosition = enemy->GetWorldPosition();
        float distance = Length(enemyPosition - playerPosition);

        // 最も近い敵を更新
        if (distance < minDistance){
            minDistance = distance;
            nearestEnemy = enemy;
        }
    }

    // 一番近い敵が見つかり、距離が20以下かつ5以上であれば追跡する
    if (nearestEnemy && minDistance > 5.0f && minDistance <= 20.0f){
        Vector3 directionToEnemy = (nearestEnemy->GetWorldPosition() - playerPosition).Normalize();
        float trackingSpeed = 0.2f; // 追跡の速度を設定（適宜調整してください）
        worldTransform_.translation_ += directionToEnemy * trackingSpeed;

        // 追跡対象に向かって回転する
        targetAngle = std::atan2(directionToEnemy.x, directionToEnemy.z);
    }
}






///==========================================================
///ゲッター/セッター
///==========================================================

#pragma region
Vector3 Player::GetCenterPos()const{
	const Vector3 offset = {0.0f,1.5f,0.0f};
	Vector3 worldPos = Matrix4x4::Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

void Player::SetMaxLife(int max){
    maxLife_ = max;
    life_ = maxLife_;
}

void Player::SetEnemyLists(const std::list<std::unique_ptr<Enemy>>& allEnemies){
    enemies_.clear();  // 既存のリストをクリア

    // unique_ptrから生ポインタを取り出して enemies_ に追加
    for (const auto& enemy : allEnemies){
        enemies_.push_back(enemy.get());
    }
}


bool Player::GetIsAttack()const{ return isAttack_; }
Vector3 Player::GetVelocity()const{ return velocity_; }
Vector3 Player::GetDirection()const{ return direction_; }
float Player::GetTargetAngle()const{ return targetAngle; }

void Player::SetIsAttack(const bool isAttack){ isAttack_ = isAttack; }
void Player::SetVelocity(const Vector3& vel){ velocity_ = vel; }
void Player::SetDirection(const Vector3& dir){ direction_ = dir; }

const LockOn* Player::GetLockOn()const{ return lockOn_; }
#pragma endregion getter


#pragma region

void Player::SetPartTranslation(Parts part, const Vector3& translation){
    if (partsTransform_[static_cast< int >(part)]){
        partsTransform_[static_cast< int >(part)]->translation_ = translation;
    }
}

void Player::SetPartRotation(Parts part, const Vector3& rotation){
    if (partsTransform_[static_cast< int >(part)]){
        partsTransform_[static_cast< int >(part)]->rotation_ = rotation;
    }
}

void Player::SetPartTranslationX(Parts part, float translationX){
    if (partsTransform_[static_cast< int >(part)]){
        partsTransform_[static_cast< int >(part)]->translation_.x = translationX;
    }
}

void Player::SetPartTranslationY(Parts part, float translationY){
    if (partsTransform_[static_cast< int >(part)]){
        partsTransform_[static_cast< int >(part)]->translation_.y = translationY;
    }
}

void Player::SetPartTranslationZ(Parts part, float translationZ){
    if (partsTransform_[static_cast< int >(part)]){
        partsTransform_[static_cast< int >(part)]->translation_.z = translationZ;
    }
}

void Player::SetPartRotationX(Parts part, float rotationX){
    if (partsTransform_[static_cast< int >(part)]){
        partsTransform_[static_cast< int >(part)]->rotation_.x = rotationX;
    }
}

void Player::SetPartRotationY(Parts part, float rotationY){
    if (partsTransform_[static_cast< int >(part)]){
        partsTransform_[static_cast< int >(part)]->rotation_.y = rotationY;
    }
}

void Player::SetPartRotationZ(Parts part, float rotationZ){
    if (partsTransform_[static_cast< int >(part)]){
        partsTransform_[static_cast< int >(part)]->rotation_.z = rotationZ;
    }
}

// 頭部のセッター
void Player::SetHeadTranslation(const Vector3& translation){
    SetPartTranslation(Parts::head, translation);
}

void Player::SetHeadRotation(const Vector3& rotation){
    SetPartRotation(Parts::head, rotation);
}

void Player::SetHeadTranslationX(float translationX){
    SetPartTranslationX(Parts::head, translationX);
}

void Player::SetHeadTranslationY(float translationY){
    SetPartTranslationY(Parts::head, translationY);
}

void Player::SetHeadTranslationZ(float translationZ){
    SetPartTranslationZ(Parts::head, translationZ);
}

void Player::SetHeadRotationX(float rotationX){
    SetPartRotationX(Parts::head, rotationX);
}

void Player::SetHeadRotationY(float rotationY){
    SetPartRotationY(Parts::head, rotationY);
}

void Player::SetHeadRotationZ(float rotationZ){
    SetPartRotationZ(Parts::head, rotationZ);
}

// 他のパーツも同様に共通メソッドを呼び出す形に修正します。
void Player::SetBodyTranslation(const Vector3& translation){
    SetPartTranslation(Parts::body, translation);
}

void Player::SetBodyRotation(const Vector3& rotation){
    SetPartRotation(Parts::body, rotation);
}

void Player::SetBodyTranslationX(float translationX){
    SetPartTranslationX(Parts::body, translationX);
}

void Player::SetBodyTranslationY(float translationY){
    SetPartTranslationY(Parts::body, translationY);
}

void Player::SetBodyTranslationZ(float translationZ){
    SetPartTranslationZ(Parts::body, translationZ);
}

void Player::SetBodyRotationX(float rotationX){
    SetPartRotationX(Parts::body, rotationX);
}

void Player::SetBodyRotationY(float rotationY){
    SetPartRotationY(Parts::body, rotationY);
}

void Player::SetBodyRotationZ(float rotationZ){
    SetPartRotationZ(Parts::body, rotationZ);
}

// 腕（左）
void Player::Set_L_ArmTranslation(const Vector3& translation){
    SetPartTranslation(Parts::L_arm, translation);
}

void Player::Set_L_ArmRotation(const Vector3& rotation){
    SetPartRotation(Parts::L_arm, rotation);
}

void Player::Set_L_ArmTranslationX(float translationX){
    SetPartTranslationX(Parts::L_arm, translationX);
}

void Player::Set_L_ArmTranslationY(float translationY){
    SetPartTranslationY(Parts::L_arm, translationY);
}

void Player::Set_L_ArmTranslationZ(float translationZ){
    SetPartTranslationZ(Parts::L_arm, translationZ);
}

void Player::Set_L_ArmRotationX(float rotationX){
    SetPartRotationX(Parts::L_arm, rotationX);
}

void Player::Set_L_ArmRotationY(float rotationY){
    SetPartRotationY(Parts::L_arm, rotationY);
}

void Player::Set_L_ArmRotationZ(float rotationZ){
    SetPartRotationZ(Parts::L_arm, rotationZ);
}

// 腕（右）
void Player::Set_R_ArmTranslation(const Vector3& translation){
    SetPartTranslation(Parts::R_arm, translation);
}

void Player::Set_R_ArmRotation(const Vector3& rotation){
    SetPartRotation(Parts::R_arm, rotation);
}

void Player::Set_R_ArmTranslationX(float translationX){
    SetPartTranslationX(Parts::R_arm, translationX);
}

void Player::Set_R_ArmTranslationY(float translationY){
    SetPartTranslationY(Parts::R_arm, translationY);
}

void Player::Set_R_ArmTranslationZ(float translationZ){
    SetPartTranslationZ(Parts::R_arm, translationZ);
}

void Player::Set_R_ArmRotationX(float rotationX){
    SetPartRotationX(Parts::R_arm, rotationX);
}

void Player::Set_R_ArmRotationY(float rotationY){
    SetPartRotationY(Parts::R_arm, rotationY);
}

void Player::Set_R_ArmRotationZ(float rotationZ){
    SetPartRotationZ(Parts::R_arm, rotationZ);
}

// 武器のセッター
void Player::SetWeaponTranslation(const Vector3& translation){
    weapon_->SetTranslation(translation);
}

void Player::SetWeaponTranslationX(float translationX){
    weapon_->SetTranslationX(translationX);
}

void Player::SetWeaponTranslationY(float translationY){
    weapon_->SetTranslationY(translationY);
}

void Player::SetWeaponTranslationZ(float translationZ){
    weapon_->SetTranslationZ(translationZ);
}

void Player::SetWeaponRotation(const Vector3& rotation){
    weapon_->SetRotation(rotation);
}

void Player::SetWeaponRotationX(float rotationX){
    weapon_->SetRotationX(rotationX);
}

void Player::SetWeaponRotationY(float rotationY){
    weapon_->SetRotationY(rotationY);
}

void Player::SetWeaponRotationZ(float rotationZ){
    weapon_->SetRotationZ(rotationZ);
}

void Player::SetWeapon(WeaponBase* weapon){
	weapon_ = weapon;
	weapon_->GetWorldTransform().parent_ = &worldTransform_;
}

void Player::SetLockOn(const LockOn* lockOn){
	lockOn_ = lockOn;
}

#pragma endregion setter