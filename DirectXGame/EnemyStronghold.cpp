#include "EnemyStronghold.h"

void EnemyStronghold::Initialize(Model* model){
	//モデルを代入/worldTransformの初期化
	BaseStronghold::Initialize(model);
}

void EnemyStronghold::Update(){
	worldTransform_.UpdateMatrix();
}

void EnemyStronghold::Draw(const ViewProjection& viewProjection){
	BaseStronghold::Draw(viewProjection);
}