#include "Player.h"
#include "PlayerRootBehavior.h"
#include "PlayerAttackBehavior.h"
#include "PlayerJumpBehavior.h"
#include "PlayerDashBehavior.h"
#include <cmath>
#include <imgui.h>
#include"GlobalVariables.h"
#include "LockOn.h"
#include"WeaponBase.h"
#include"CollisionTypeIdDef.h"
Player::Player() : currentState_(nullptr), isAttack_(false), viewPorjection_(nullptr){
	partsTransform_.resize(static_cast< int >(Parts::partsCount));
	partsTransform_[static_cast< int >(Parts::body)] = std::make_unique<WorldTransform>();
	partsTransform_[static_cast< int >(Parts::head)] = std::make_unique<WorldTransform>();
	partsTransform_[static_cast< int >(Parts::L_arm)] = std::make_unique<WorldTransform>();
	partsTransform_[static_cast< int >(Parts::R_arm)] = std::make_unique<WorldTransform>();

	const char* groupName = "Player";
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	globalVariables->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "Head Translation", partsTransform_[static_cast< int >(Parts::head)]->translation_);
	globalVariables->AddItem(groupName, "ArmL Translation", partsTransform_[static_cast< int >(Parts::L_arm)]->translation_);
	globalVariables->AddItem(groupName, "ArmR Translation", partsTransform_[static_cast< int >(Parts::R_arm)]->translation_);
}

Player::~Player(){}

void Player::Initialize(const std::vector<Model*>& models){
    //モデルとワールドトランスフォームの初期化
    Actor::Initialize(models);
    //各パーツのtransformの初期化
    PartsTransformInit();
    //通常行動をセットしておく
    ChangeState(std::make_unique<PlayerRootBehavior>(this));
    //衝突判定の識別id
    Collider::SetTypeID(static_cast< uint32_t >(CollisionTypeIdDef::kPlayer));
}

void Player::PartsTransformInit(){
    for (auto& partTransform : partsTransform_){
        partTransform->Initialize();
    }

    partsTransform_[static_cast< int >(Parts::head)]->translation_ = Vector3 {0.0f, 2.75f, 0.0f};
    partsTransform_[static_cast< int >(Parts::L_arm)]->translation_ = Vector3 {-1.0f, 2.0f, 0.0f};
    partsTransform_[static_cast< int >(Parts::R_arm)]->translation_ = Vector3 {1.0f, 2.0f, 0.0f};

    partsTransform_[static_cast< int >(Parts::body)]->parent_ = &worldTransform_;
    auto body = partsTransform_[static_cast< int >(Parts::body)].get();
    partsTransform_[static_cast< int >(Parts::head)]->parent_ = body;
    partsTransform_[static_cast< int >(Parts::L_arm)]->parent_ = body;
    partsTransform_[static_cast< int >(Parts::R_arm)]->parent_ = body;
}

void Player::Update(){

#ifdef _DEBUG
    ImGui::Begin("player");
    auto weaponRotate = weapon_->GetRotation();
    ImGui::DragFloat3("weaponRotate",&weaponRotate.x,0.01f);
    weapon_->SetRotation(weaponRotate);

    auto weaponTranslation = weapon_->GetTranslation();
    ImGui::DragFloat3("weaponTranslation", &weaponTranslation.x, 0.01f);
    weapon_->SetTranslation(weaponTranslation);

    ImGui::DragFloat3("playerTranslation", &worldTransform_.translation_.x, 0.01f);

    ImGui::End();
#endif // _DEBUG


    if (currentState_){
        currentState_->Update();
    }

    //======================================
    //      武器の更新
    //======================================
    if (weapon_){
        weapon_->Update();
    }

    //旋回を滑らかにする
    worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, targetAngle, 0.1f);

    //transformの更新
    Actor::Update();
    for (const auto& transform : partsTransform_){
        transform->UpdateMatrix();
    }
}

void Player::Draw(const ViewProjection& viewProjection){
    models_[static_cast< int >(Parts::head)]->Draw(*partsTransform_[static_cast< int >(Parts::head)], viewProjection);
    models_[static_cast< int >(Parts::body)]->Draw(*partsTransform_[static_cast< int >(Parts::body)], viewProjection);
    models_[static_cast< int >(Parts::L_arm)]->Draw(*partsTransform_[static_cast< int >(Parts::L_arm)], viewProjection);
    models_[static_cast< int >(Parts::R_arm)]->Draw(*partsTransform_[static_cast< int >(Parts::R_arm)], viewProjection);

    if (isAttack_){
        weapon_->Draw(viewProjection);
    }
}

void Player::MoveInDirection(float speed){
    velocity_ = direction_ * speed;

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

void Player::ChangeState(std::unique_ptr<PlayerBaseBehavior> newState){
    currentState_ = std::move(newState);
    if (currentState_){
        currentState_->Initialize();
    }
}

void Player::ApplyGlobalVariables(){
    GlobalVariables* globalVariables = GlobalVariables::GetInstance();
    const char* groupName = "Player";

    partsTransform_[static_cast< int >(Parts::head)]->translation_ = globalVariables->GetValue<Vector3>(groupName, "Head Translation");
    partsTransform_[static_cast< int >(Parts::L_arm)]->translation_ = globalVariables->GetValue<Vector3>(groupName, "ArmL Translation");
    partsTransform_[static_cast< int >(Parts::R_arm)]->translation_ = globalVariables->GetValue<Vector3>(groupName, "ArmR Translation");
}

void Player::SetViewProjection(const ViewProjection* viewProjection){ viewPorjection_ = viewProjection; }

bool Player::HasLockOnTarget()const{ return lockOn_->ExistTarget() ? true : false; }

void Player::OnCollision([[maybe_unused]] Collider* other){
  //  ChangeState(std::make_unique<PlayerJumpBehavior>(this));
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

//============================================================================================================================================
//								平行移動

void Player::SetHeadTranslation(const Vector3& translation){ partsTransform_[static_cast< int >(Parts::head)]->translation_ = translation; }
void Player::Set_L_ArmTranslation(const Vector3& translation){ partsTransform_[static_cast< int >(Parts::L_arm)]->translation_ = translation; }
void Player::Set_R_ArmTranslation(const Vector3& translation){ partsTransform_[static_cast< int >(Parts::R_arm)]->translation_ = translation; }
void Player::SetBodyTranslation(const Vector3& translation){ partsTransform_[static_cast< int >(Parts::body)]->translation_ = translation; }

void Player::SetHeadTranslationX(const float translationX){ partsTransform_[static_cast< int >(Parts::head)]->translation_.x = translationX; }
void Player::SetHeadTranslationY(const float translationY){ partsTransform_[static_cast< int >(Parts::head)]->translation_.y = translationY; }
void Player::SetHeadTranslationZ(const float translationZ){ partsTransform_[static_cast< int >(Parts::head)]->translation_.z = translationZ; }

void Player::SetBodyTranslationX(const float translationX){ partsTransform_[static_cast< int >(Parts::body)]->translation_.x = translationX; }
void Player::SetBodyTranslationY(const float translationY){ partsTransform_[static_cast< int >(Parts::body)]->translation_.y = translationY; }
void Player::SetBodyTranslationZ(const float translationZ){ partsTransform_[static_cast< int >(Parts::body)]->translation_.z = translationZ; }

void Player::Set_L_ArmTranslationX(const float translationX){ partsTransform_[static_cast< int >(Parts::L_arm)]->translation_.x = translationX; }
void Player::Set_L_ArmTranslationY(const float translationY){ partsTransform_[static_cast< int >(Parts::L_arm)]->translation_.y = translationY; }
void Player::Set_L_ArmTranslationZ(const float translationZ){ partsTransform_[static_cast< int >(Parts::L_arm)]->translation_.z = translationZ; }

void Player::Set_R_ArmTranslationX(const float translationX){ partsTransform_[static_cast< int >(Parts::R_arm)]->translation_.x = translationX; }
void Player::Set_R_ArmTranslationY(const float translationY){ partsTransform_[static_cast< int >(Parts::R_arm)]->translation_.y = translationY; }
void Player::Set_R_ArmTranslationZ(const float translationZ){ partsTransform_[static_cast< int >(Parts::R_arm)]->translation_.z = translationZ; }

void Player::SetWeaponTranslation(const Vector3& translation){ weapon_->SetTranslation(translation); }
void Player::SetWeaponTranslationX(const float translation){ weapon_->SetTranslationX(translation); }
void Player::SetWeaponTranslationY(const float translation){ weapon_->SetTranslationY(translation); }
void Player::SetWeaponTranslationZ(const float translation){ weapon_->SetTranslationZ(translation); }

//============================================================================================================================================
//								回転

void Player::SetHeadRotation(const Vector3& rotation){ partsTransform_[static_cast< int >(Parts::head)]->rotation_ = rotation; }
void Player::SetHeadRotationX(const float rotationX){ partsTransform_[static_cast< int >(Parts::head)]->rotation_.x = rotationX; }
void Player::SetHeadRotationY(const float rotationY){ partsTransform_[static_cast< int >(Parts::head)]->rotation_.y = rotationY; }
void Player::SetHeadRotationZ(const float rotationZ){ partsTransform_[static_cast< int >(Parts::head)]->rotation_.z = rotationZ; }

void Player::SetBodyRotation(const Vector3& rotation){ partsTransform_[static_cast< int >(Parts::body)]->rotation_ = rotation; }
void Player::SetBodyRotationX(const float rotationX){ partsTransform_[static_cast< int >(Parts::body)]->rotation_.x = rotationX; }
void Player::SetBodyRotationY(const float rotationY){ partsTransform_[static_cast< int >(Parts::body)]->rotation_.y = rotationY; }
void Player::SetBodyRotationZ(const float rotationZ){ partsTransform_[static_cast< int >(Parts::body)]->rotation_.z = rotationZ; }

void Player::Set_L_ArmRotation(const Vector3& rotation){ partsTransform_[static_cast< int >(Parts::L_arm)]->rotation_ = rotation; }
void Player::Set_L_ArmRotationX(const float rotationX){ partsTransform_[static_cast< int >(Parts::L_arm)]->rotation_.x = rotationX; }
void Player::Set_L_ArmRotationY(const float rotationY){ partsTransform_[static_cast< int >(Parts::L_arm)]->rotation_.y = rotationY; }
void Player::Set_L_ArmRotationZ(const float rotationZ){ partsTransform_[static_cast< int >(Parts::L_arm)]->rotation_.z = rotationZ; }

void Player::Set_R_ArmRotation(const Vector3& rotation){ partsTransform_[static_cast< int >(Parts::R_arm)]->rotation_ = rotation; }
void Player::Set_R_ArmRotationX(const float rotationX){ partsTransform_[static_cast< int >(Parts::R_arm)]->rotation_.x = rotationX; }
void Player::Set_R_ArmRotationY(const float rotationY){ partsTransform_[static_cast< int >(Parts::R_arm)]->rotation_.y = rotationY; }
void Player::Set_R_ArmRotationZ(const float rotationZ){ partsTransform_[static_cast< int >(Parts::R_arm)]->rotation_.z = rotationZ; }

void Player::SetWeaponRotation(const Vector3& rotation){ weapon_->SetRotation(rotation); }
void Player::SetWeaponRotationX(const float rotation){ weapon_->SetRotationX(rotation); }
void Player::SetWeaponRotationY(const float rotation){ weapon_->SetRotationY(rotation); }
void Player::SetWeaponRotationZ(const float rotation){ weapon_->SetRotationZ(rotation); }

void Player::SetWeapon(WeaponBase* weapon){ 
    weapon_ = weapon;
    weapon_->GetWorldTransform().parent_ = &worldTransform_;
}

void Player::SetLockOn(const LockOn* lockOn){
    lockOn_ = lockOn;
}

#pragma endregion setter