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
	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();

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
	player_->Initialize(modelPlayer_, viewProjection_.get());

	///=====================================================
	//		デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>(kWindowWidth, kWindowHeight);

}

void GameScene::Update() {

#ifdef _DEBUG
	// デバッグ用のカメラ
	debugCamera_->Update();

	if (isDebugCameraActive_){
		viewProjection_->matView = debugCamera_->GetViewProjection().matView;
		viewProjection_->matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_->TransferMatrix();
	} else{
		viewProjection_->UpdateMatrix();
	}

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
	skydome_->Draw(viewProjection_.get());

	//=========================================================
	//	地面の描画
	//=========================================================
	ground_->Draw(viewProjection_.get());

	//=========================================================
	//	プレイヤーの描画
	//=========================================================
	player_->Draw();

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
