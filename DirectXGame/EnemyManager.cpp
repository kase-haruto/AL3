#include "EnemyManager.h"
#include"EnemyStronghold.h"

void EnemyManager::Initialize(){
	
	////////////////////////////////////////////////////////////////////
	//		protectEnemy	(拠点を守る敵)
	////////////////////////////////////////////////////////////////////

	// protectEnemies_ に複数の ProtectEnemy インスタンスを追加
	for (int i = 0; i < 3; ++i){
		auto protectEnemy = std::make_unique<ProtectEnemy>();

		// モデルの作成
		protectEnemyModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("enemy", true)));
		protectEnemyModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("enemy_arm", true)));
		std::vector<Model*> rawModelPtrs;
		for (const auto& model : protectEnemyModels_){
			rawModelPtrs.push_back(model.get());
		}

		// 初期化
		protectEnemy->Initialize(rawModelPtrs);

		//座標のセット
		protectEnemy->SetPos(stronghold_[i]->GetWorldTransform().translation_);

		//トランスフォームの更新
		protectEnemy->UpdateMatrix();

		// protectEnemies_ ベクターに追加
		protectEnemies_.push_back(std::move(protectEnemy));

		// リストに追加
		allEnemies_.push_back(std::move(protectEnemies_.back()));
	}
}


void EnemyManager::Update(){
	for (const auto& enemy:allEnemies_){
		enemy->Update();
	}
}

void EnemyManager::Draw(const ViewProjection& viewProjection){
	for (const auto& enemy : allEnemies_){
		enemy->Draw(viewProjection);
	}
}

const std::list<std::unique_ptr<Enemy>>& EnemyManager::GetAllEnemies() const{
	return allEnemies_;
}

void EnemyManager::SetStronghold(const std::vector<std::shared_ptr<EnemyStronghold>>& stronghold){
	stronghold_ = stronghold;
}