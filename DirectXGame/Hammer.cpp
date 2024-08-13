#include "Hammer.h"
#include<cassert>
#include<imgui.h>
#include"Enemy.h"
#include"AttackEffectParticle.h"
#include"MyFunc.h"

void Hammer::Initialize(Model* model){
    WeaponBase::Initialize(model);
    Collider::SetRadius(3.0f);

    Collider::SetTypeID(static_cast< uint32_t >(CollisionTypeIdDef::kPlayerWeapon));

}

void Hammer::Update(){

    // アクティブなエフェクトを更新
    for (auto& effect : activeEffects){
        effect->Update();
    }

    // 不要なエフェクトを削除
    activeEffects.erase(
        std::remove_if(activeEffects.begin(), activeEffects.end(),
        [] (const std::unique_ptr<AttackEffectParticle>& effect){
            return effect->IsEffectFinished(); // エフェクトが終了した場合削除
        }),
        activeEffects.end()
            );

    worldTransform_.UpdateMatrix();
}

void Hammer::Draw(const ViewProjection& viewProjection){
    WeaponBase::Draw(viewProjection);

    // アクティブなエフェクトを描画
    for (auto& effect : activeEffects){
        effect->Draw(viewProjection);
    }
}

void Hammer::OnCollision([[maybe_unused]] Collider* other){
    // 衝突相手の種別IDを取得
    uint32_t typeID = other->GetTypeID();

    // 衝突相手が敵なら
    if (typeID == static_cast< uint32_t >(CollisionTypeIdDef::kEnemy)){
        Enemy* enemy = static_cast< Enemy* >(other);
        uint32_t serialNumber = enemy->GetSerialNumber();

        //接触履歴があれば何もせず抜ける
        if (contactRecord_.CheckRecord(serialNumber)){
            return;
        }

        //履歴に登録
        contactRecord_.AddRecord(serialNumber);

        // 敵の位置にエフェクトを発生
        if (enemy){

            // AttackEffectParticle を生成し、敵の位置にエフェクトを再生
            auto effect = std::make_unique<AttackEffectParticle>();
            effect->Initialize(&enemy->GetWorldTransform());

            // エフェクトをリストに追加して管理
            activeEffects.push_back(std::move(effect));
        }
    }
}

Vector3 Hammer::GetCenterPos() const{
    const Vector3 offset = {0.0f,9.0f,0.0f};
    Vector3 worldPos = Matrix4x4::Transform(offset, worldTransform_.matWorld_);
    return worldPos;
}
