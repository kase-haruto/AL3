#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene(){ delete moedlSkydome_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;

	//viewProjectionの初期化
	viewProjection_.Initialize();

	///=====================================================
	//		天球
	moedlSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = std::make_unique<Skydome>(moedlSkydome_);
	skydome_->Initialize();

	///=====================================================
	//		地面
	modelGround_ = Model::CreateFromOBJ("ground", true);
	ground_ = std::make_unique<Ground>(modelGround_);
	ground_->Initialize(skydome_->GetScale());

	
	///=====================================================
	//		プレイヤー
	modelPlayer_ = Model::CreateFromOBJ("player", false);
	player_ = std::make_unique<Player>();
	player_->Initialize(modelPlayer_);

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

void GameScene::Update() {
	//プレイヤーの更新
	player_->Update();


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

void GameScene::Draw() {

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
