#include "EnemyStation.h"
#include "AttackEnemy.h"
#include "Model.h"
#include <vector>

void EnemyStation::Initialize(){
    modelStation_.reset(Model::CreateFromOBJ("enemyStation", false));
    worldTransform_.Initialize();

    spawnTimer_ = kLimitTime_; // 初期化時にタイマーをリセット

    worldTransform_.UpdateMatrix();
}

void EnemyStation::Update(){
    if (spawnTimer_ > 0){
        spawnTimer_--;
    } else{
        SpawnEnemy(); // タイマーが0になったらスポーンさせる
        spawnTimer_ = kLimitTime_; // タイマーのリセット
    }

    worldTransform_.UpdateMatrix();
}

void EnemyStation::Draw(const ViewProjection& viewProjection){
    modelStation_->Draw(worldTransform_, viewProjection);
}

void EnemyStation::SetPosition(const Vector3& pos){
    worldTransform_.translation_ = pos;
    worldTransform_.UpdateMatrix(); // 位置を設定後に行列を更新
}

void EnemyStation::SetPlayerStrongholdPtr(PlayerStronghold* stronghold){
    playerStrongholdPtr_ = stronghold;
}

void EnemyStation::SetEnemyManager(EnemyManager* enemyManager){
    enemyManager_ = enemyManager;
}

void EnemyStation::SpawnEnemy(){
    // モデルの作成
    std::vector<Model*> models;
    models.push_back(Model::CreateFromOBJ("enemy", true));
    models.push_back(Model::CreateFromOBJ("enemy_arm", true));

    // 新しい敵をスポーン
    auto newEnemy = std::make_unique<AttackEnemy>();
    newEnemy->Initialize(models);

    // 敵のスポーン位置を設定 (EnemyStationの位置に設定)
    Vector3 spawnPosition = worldTransform_.translation_;
    newEnemy->SetPos(spawnPosition);

    // 初期位置を設定した後にワールド行列を更新
    newEnemy->UpdateMatrix();

    // プレイヤーを設定
    newEnemy->SetTarget(playerStrongholdPtr_->GetPlayerPtr());
    newEnemy->SetPlayerStronghold(playerStrongholdPtr_);

    // 敵をマネージャーに追加
    enemyManager_->AddEnemy(std::move(newEnemy));
}
