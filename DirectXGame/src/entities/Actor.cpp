#include "Actor.h"

void Actor::Initialize(const std::vector<Model*>& models){
	models_ = models;
	worldTransform_.Initialize();

	//可視化用コライダーのトランスフォームの初期化
	Collider::Initialize();
}

void Actor::Update(){
    if (isAlive_){
        if (life_<=0){
            isAlive_ = false;
        }
        //行列の更新
        worldTransform_.UpdateMatrix();
    }

}

void Actor::Draw(const ViewProjection& viewProjection){
	for (Model* model : models_){
		model->Draw(worldTransform_, viewProjection);
	}
}

Vector3 Actor::GetCenterPos()const{
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Actor::UpdateMatrix(){
	worldTransform_.UpdateMatrix();
}


///================================
///	アクセッサ
///================================
const WorldTransform& Actor::GetWorldTransform(){ return worldTransform_; }

void Actor::OnCollision([[maybe_unused]] Collider* other){
    // this が nullptr でないことを確認
    if (this == nullptr){
        return; // ここに到達することは通常ないはずです
    }

    // 衝突相手が存在するか確認
    if (other == nullptr){
        return;
    }

    // 衝突相手のActorを取得
    Actor* otherActor = dynamic_cast< Actor* >(other);
    if (otherActor == nullptr){
        return; // dynamic_cast が失敗する場合を考慮
    }

    if (otherActor->GetIsAlive()){
        // 自分と相手の位置を取得
        Vector3 myPosition = this->GetWorldPosition();
        Vector3 otherPosition = otherActor->GetWorldPosition();

        // 相手から自分へのベクトルを計算
        Vector3 knockbackDirection = myPosition - otherPosition;

        // Y軸を反転させる場合
        knockbackDirection.y = -knockbackDirection.y;

        knockbackDirection.Normalize();

        // ノックバックの強さを設定
        float knockbackStrength = 1.0f;  // 適宜調整

        // ノックバックのベクトルを計算
        Vector3 knockbackVector = knockbackDirection * knockbackStrength;

        // 自分の位置を更新
        Vector3 newTranslation = GetTranslation() + knockbackVector;
        SetTranslation(newTranslation);

        // 行列を更新
        UpdateMatrix();
    }
}

Vector3 Actor::GetWorldPosition()const{
	Vector3 wPos;
	wPos.x = worldTransform_.matWorld_.m[3][0];
	wPos.y = worldTransform_.matWorld_.m[3][1];
	wPos.z = worldTransform_.matWorld_.m[3][2];
	return wPos;
}

void Actor::SetTranslation(const Vector3& translation){ worldTransform_.translation_ = translation; }
void Actor::SetTranslationX(const float translation){ worldTransform_.translation_.x = translation; }
void Actor::SetTranslationY(const float translation){ worldTransform_.translation_.y = translation; }
void Actor::SetTranslationZ(const float translation){ worldTransform_.translation_.z = translation; }
void Actor::SetRotation(const Vector3& rotation){ worldTransform_.rotation_ = rotation; }
void Actor::SetRotationX(const float rotation){ worldTransform_.rotation_.x = rotation; }
void Actor::SetRotationY(const float rotation){ worldTransform_.rotation_.y = rotation; }
void Actor::SetRotationZ(const float rotation){ worldTransform_.rotation_.z = rotation; }

Vector3 Actor::GetTranslation()const{ return worldTransform_.translation_; }
Vector3 Actor::GetRotation()const{ return worldTransform_.rotation_; }

void Actor::SetPos(const Vector3& pos){ worldTransform_.translation_ = pos; }


uint32_t Actor::GetColor()const{ return color_; }
void Actor::SetColor(const uint32_t& color){ color_ = color; }