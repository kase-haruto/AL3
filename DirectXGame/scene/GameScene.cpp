#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include<cmath>

GameScene::GameScene(){}

GameScene::~GameScene(){ delete debugCamera_, model_; }

void GameScene::Initialize(){

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	model_ = Model::Create();

	player_ = std::make_unique<Player>();
	player_->Init(model_);

	enemy_ = std::make_unique<Enemy>();
	enemy_->SetPlayer(player_.get());
	enemy_->Init(model_);

	// デバッグ用のカメラ
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するプロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update(){
	player_->Update();

	if (enemy_){
		if (input_->TriggerKey(DIK_SPACE)){
			enemy_->SetIsMove(true);
		}
		enemy_->Update();

	}

	CheckAllCollisions();


#ifdef _DEBUG

	// デバッグ用のカメラ
	debugCamera_->Update();

	if (isDebugCameraActive_){
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else{
		viewProjection_.UpdateMatrix();
	}

	//カメラの切り替え
	if (input_->TriggerKey(DIK_RETURN)){
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif // _DEBUG

}

void GameScene::Draw(){

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	enemy_->Draw(viewProjection_);

	player_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions(){
	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list< std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

	//コライダーのリスト
	std::list<Collider*> colliders_;
	colliders_.push_back(player_.get());
	colliders_.push_back(enemy_.get());
	for (auto& playerB : playerBullets){
		colliders_.push_back(playerB.get());
	}
	for (auto& enemyB : enemyBullets){
		colliders_.push_back(enemyB.get());
	}


	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA){
		Collider* colA = *itrA;
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		//イテレータBはAの次の要素から回す
		for (; itrB != colliders_.end(); ++itrB){
			Collider* colB = *itrB;

			CheckCollisionPair(colA, colB);
		}
	}

}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB){

	Vector3 colA_Pos = colliderA->GetWorldPosition();
	Vector3 colB_Pos = colliderB->GetWorldPosition();
	float colA_rad = colliderA->GetRadius();
	float colB_rad = colliderB->GetRadius();
	float distance = CheckDistance(colA_Pos, colB_Pos);

	// 衝突フィルタリング
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0){
		return;
	}

	//球と球の当たり判定
	if (IsOnCollision(distance, colA_rad, colB_rad)){
		colliderA->OnCollision();
		colliderB->OnCollision();
	}

}

float GameScene::CheckDistance(Vector3 v1, Vector3 v2){
	float distanceX, distanceY, distanceZ;
	distanceX = v2.x - v1.x;
	distanceY = v2.y - v1.y;
	distanceZ = v2.z - v1.z;
	return std::sqrt(distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ);
}

bool GameScene::IsOnCollision(float distance, float radius1, float radius2){
	if (distance <= radius1 + radius2){
		return true;
	}
	return false;
}


