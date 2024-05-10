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
	//判定対象abの座標
	Vector3 posA, posB;
	float radiusA, radiusB;
	float distance;
	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list< std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の判定

	//自キャラの座標
	posA = player_->GetWorldPosition();
	radiusA = player_->GetRadius();
	//自キャラと敵弾すべての当たり判定
	for (auto& bullet : enemyBullets){
		posB = bullet->GetWorldPosition();
		radiusB = bullet->GetRadius();

		//衝突判定
		distance = CheckDistance(posA, posB);
		if (IsOnCollision(distance, radiusA, radiusB)){
			player_->OnCollision();
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵の当たり判定

	//敵の情報
	posA = enemy_->GetWorldPosition();
	radiusA = enemy_->GetRadius();
	for (auto& bullet : playerBullets){
		posB = bullet->GetWorldPosition();
		radiusB = bullet->GetRadius();

		//衝突判定
		distance = CheckDistance(posA, posB);
		if (IsOnCollision(distance, radiusA, radiusB)){
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の判定

	for (auto& playerBullet : playerBullets){
		posA = playerBullet->GetWorldPosition();
		radiusA = playerBullet->GetRadius();
		for (auto& enemyBullet :enemyBullets){
			posB = enemyBullet->GetWorldPosition();
			radiusB = enemyBullet->GetRadius();

			distance = CheckDistance(posA, posB);
			if (IsOnCollision(distance,radiusA,radiusB)){
				enemyBullet->OnCollision();
				playerBullet->OnCollision();
			}
		}
	}

#pragma endregion

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


