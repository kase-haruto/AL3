#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include<cmath>
#include"CollisionManager.h"
#include<imgui.h>
#include <fstream>
#include<cassert>

GameScene::GameScene(){}

GameScene::~GameScene(){
	delete model_, modelSkydome_;
}

void GameScene::Initialize(){

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するプロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// デバッグ用のカメラ
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	viewProjection_.Initialize();
	viewProjection_.UpdateMatrix();
	

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = std::make_unique<Skydome>(modelSkydome_);
	skydome_->Initialize();

	model_ = Model::Create();
	//レールカメラ
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize(viewProjection_.GetWorldPosition());
	player_ = std::make_unique<Player>();
	Vector3 playerPos = {0.0f,0.0f,20.0f};
	player_->Init(model_, playerPos);
	player_->SetParent(&railCamera_->GetWorldTransform());

	LoadEnemyPopData();
}

void GameScene::Update(){
	//=========================================================
	//	 playerの更新処理
	//=========================================================
	player_->Update(viewProjection_);
	player_->setScrollVal(railCamera_->GetScrollVal());

	//=========================================================
	//	 enemyの更新処理
	//=========================================================
	UpdateEnemyPopData();
	ImGui::Begin("Enemy");
	std::vector<Vector3> enemyScPositions;
	for (auto& enemy : enemies_){
		ImGui::Text("enemyPos:(%f,%f,%f)", enemy->GetWorldPosition().x, enemy->GetWorldPosition().y, enemy->GetWorldPosition().z);
		enemy->Update();
		//敵のスクリーン座標を取得
		enemyScPositions.push_back(WorldToScreen(enemy->GetWorldPosition(), viewProjection_));
	}
	player_->SetTargetPos(enemyScPositions);
	ImGui::End();
	EnemyBulletUpdate();


	//=========================================================
	//	 衝突判定
	//=========================================================
	CollisionManager::GetInstance()->Update();


	//=========================================================
	//	カメラの更新処理
	//=========================================================
	railCamera_->Update();

#ifdef _DEBUG

	if (isDebugCameraActive_){
		// デバッグ用のカメラ
		debugCamera_->Update();
		//情報の受け渡し
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		//転送
	} else{
		//情報の受け渡し
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		//更新と転送
	}
	viewProjection_.TransferMatrix();



#endif // _DEBUG

#ifdef _DEBUG

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


	//=========================================================
	//	天球の描画
	//=========================================================
	skydome_->Draw(viewProjection_);

	//=========================================================
	//	enemyの描画
	//=========================================================
	for (auto& enemy : enemies_){
		enemy->Draw(viewProjection_);
	}
	for (auto& bullet : enemyBullet_){
		bullet->Draw(viewProjection_);
	}

	//=========================================================
	//	playerの描画
	//=========================================================
	player_->Draw(viewProjection_);



	// 3Dオブジェクト描画後処理
	Model::PostDraw();
	railCamera_->DrawLine(viewProjection_);

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player_->DrawUi();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::EnemyBulletUpdate(){
	// 弾の更新
	for (auto& bullet : enemyBullet_){
		bullet->Update();
	}

	// 弾の更新
	enemyBullet_.remove_if([] (const std::unique_ptr<EnemyBullet>& bullet){
		if (bullet->GetIsDeth()){
			//衝突判定リストから削除
			CollisionManager::GetInstance()->RemoveCollider(bullet.get());
			return true;
		}
		return false;
						   });
}

void GameScene::SetEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet){
	enemyBullet_.push_back(std::move(enemyBullet));
}

void GameScene::LoadEnemyPopData(){
	//ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopData(){
	bool isWaiting = false;
	int32_t waitTimer = 0;
	//待機処理
	if (isWaiting){
		waitTimer--;
		if (waitTimer <= 0){
			//待機完了
			isWaiting = false;
		}
		return;
	}


	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)){
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0){
			//コメント行を飛ばす
			continue;
		}

	#pragma region POPコマンド
		//POPコマンド
		if (word.find("POP") == 0){
			//x座標
			getline(line_stream, word, ',');
			float x = ( float ) std::atof(word.c_str());

			//y座標
			getline(line_stream, word, ',');
			float y = ( float ) std::atof(word.c_str());

			//z座標
			getline(line_stream, word, ',');
			float z = ( float ) std::atof(word.c_str());

			enemies_.push_back(std::make_unique<Enemy>());
			enemies_.back()->SetPlayer(player_.get());
			enemies_.back()->Init({x,y,z}, model_, this);
		}
	#pragma endregion

	#pragma region WAITコマンド
		else if (word.find("WAIT") == 0){
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機時間
			isWaiting = true;
			waitTimer = waitTime;

			//コマンドループを抜ける

			break;
		}
	#pragma endregion

	}
}