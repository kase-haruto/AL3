#include "EnemyManager.h"
#include "EnemyStronghold.h"
#include"Player.h"

void EnemyManager::Initialize(){
    ////////////////////////////////////////////////////////////////////
    //		protectEnemy	(拠点を守る敵)
    ////////////////////////////////////////////////////////////////////

    // protectEnemyModels_ を一度だけ初期化し、複数の敵で共有
    protectEnemyModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("enemy", true)));
    protectEnemyModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("enemy_arm", true)));

    std::vector<Model*> rawModelPtrs;
    for (const auto& model : protectEnemyModels_){
        rawModelPtrs.push_back(model.get());
    }

    // protectEnemies_ に複数の ProtectEnemy インスタンスを追加
    for (int i = 0; i < stronghold_.size(); ++i){
        auto protectEnemy = std::make_unique<ProtectEnemy>();

        // 初期化
        protectEnemy->Initialize(rawModelPtrs);

        // 座標のセット
        protectEnemy->SetPos(stronghold_[i]->GetWorldTransform().translation_);

        // 敵の拠点に敵をセット
        stronghold_[i]->SetProtectEnemy(protectEnemy.get());

        // トランスフォームの更新
        protectEnemy->UpdateMatrix();

        protectEnemy->SetPlayer(player_);

        protectEnemy->SetStronghold(stronghold_[i].get());

        // protectEnemies_ ベクターに追加
        protectEnemies_.push_back(std::move(protectEnemy));

        // allEnemies_ リストに追加
        allEnemies_.push_back(std::move(protectEnemies_.back()));
    }

    ////////////////////////////////////////////////////////////////////
    //		trackingEnemy	(プレイヤーを追従する敵)
    ////////////////////////////////////////////////////////////////////
}

void EnemyManager::Update(){
    for (const auto& enemy : allEnemies_){
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

void EnemyManager::AddEnemy(std::unique_ptr<Enemy>&& enemy){
    allEnemies_.push_back(std::move(enemy));
}

void EnemyManager::SetPlayer(Player* player){
    player_ = player;
}
