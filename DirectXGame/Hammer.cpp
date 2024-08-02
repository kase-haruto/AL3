#include "Hammer.h"
#include"CollisionTypeIdDef.h"
#include"Enemy.h"
#include<cassert>
void Hammer::Initialize(Model* model){
	WeaponBase::Initialize(model);
}

void Hammer::Draw(const ViewProjection& viewProjection){
	WeaponBase::Draw(viewProjection);
}

void Hammer::OnCollision([[maybe_unused]]Collider* other ){
	/*
	* ========================================
	* エフェクトが未実装のため、コメントアウトしておく
	* ========================================
	* 
	//衝突相手の識別idを取得
	uint32_t typeID = other->GetTypeId();
	//衝突相手が敵なら
	if (typeID==static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)){
		Enemy* enemy = static_cast< Enemy* >(other);
		uint32_t serialNumber = enemy->GetSerialNumber();

		//衝突履歴があれば何もせずに抜ける
		if (contactRecord_.CheckRecord(serialNumber)){
			return;
		}
		//接触履歴に登録
		contactRecord_.AddRecord(serialNumber);

		//敵の位置にエフェクトを発生
	}	
	*/
	
}

Vector3 Hammer::GetCenterPos() const{
	const Vector3 offset = {0.0f,1.5f,0.0f};
	Vector3 worldPos = Matrix4x4::Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}