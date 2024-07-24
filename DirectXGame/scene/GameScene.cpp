#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene(){}

GameScene::~GameScene(){}

void GameScene::Initialize(){

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;

	//viewProjectionの初期化
	viewProjection_.Initialize();

	///=====================================================
	//		天球
	moedlSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	skydome_ = std::make_unique<Skydome>(moedlSkydome_.get());
	skydome_->Initialize();


	///=====================================================
	//		地面
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	ground_ = std::make_unique<Ground>(modelGround_.get());
	ground_->Initialize(skydome_->GetScale());


	///=====================================================
	//		プレイヤー
	playerModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("playerBody", true)));
	playerModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("playerHead", true)));
	playerModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("player_L_arm", true)));
	playerModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("player_R_arm", true)));
	playerModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("hammer", true)));
	player_ = std::make_unique<Player>();
	std::vector<Model*> modelPointers;
	for (const auto& model : playerModels_){
		modelPointers.push_back(model.get());
	}
	// プレイヤークラスを初期化
	player_->Initialize(modelPointers);

	///=====================================================
	//		敵
	enemyModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("enemy", true)));
	enemyModels_.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("enemy_arm", true)));
	for (int i = 0; i < 3; i++){
		auto enemy = std::make_unique<Enemy>();
		std::vector<Model*>enemyModelPtr;
		for (const auto& model : enemyModels_){
			enemyModelPtr.push_back(model.get());
		}
		enemy->Initialize(enemyModelPtr);
		enemies_.push_back(std::move(enemy));
	}
	
	enemies_[0]->SetPos({10.0f,0.0f,20.0f});
	enemies_[1]->SetPos({20.0f,0.0f,20.0f});
	enemies_[2]->SetPos({30.0f,0.0f,20.0f});
	


	///=====================================================
	//		追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransform());
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	///=====================================================
	//		デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>(kWindowWidth, kWindowHeight);

}

void GameScene::Update(){
	//プレイヤーの更新
	player_->Update();
	//敵の更新
	for (const auto& enemy:enemies_){
		enemy->Update();
	}

#ifdef _DEBUG
	// デバッグ用のカメラ
	followCamera_->Update();
	if (isDebugCameraActive_){
		// デバッグ用のカメラ
		debugCamera_->Update();
		//情報の受け渡し
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
	} else{
		//情報の受け渡し
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		//更新と転送
	}
	viewProjection_.TransferMatrix();

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
	//	地面の描画
	//=========================================================
	ground_->Draw(viewProjection_);

	//=========================================================
	//	プレイヤーの描画
	//=========================================================
	player_->Draw(viewProjection_);

	//=========================================================
	//	敵の描画
	//=========================================================
	for (const auto& enemy : enemies_){
		enemy->Draw(viewProjection_);
	}
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
