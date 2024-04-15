#include "GameScene.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"


/*==================================================================================================
        グリッド線を引く関数
===================================================================================================*/

/// <summary>
/// グリッドを描画します
/// </summary>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="startPos"></param>
/// <param name="endPos"></param>
void DrawGridLine( const Vector3& startPos, const Vector3& endPos) {
	// グリッドの描画
	PrimitiveDrawer::GetInstance()->DrawLine3d(startPos, endPos, {1.0f, 0.0f, 0.0f, 1.0f});
}

/// <summary>
/// 縦線を描画します
/// </summary>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="gridHalfWidth"></param>
/// <param name="subdivision"></param>
void DrawVerticalGridLines(float gridHalfWidth, uint32_t subdivision) {
	const float kGridEvery = (gridHalfWidth * 2.0f) / float(subdivision);

	for (uint32_t xIndex = 0; xIndex <= subdivision; ++xIndex) {
		// グリッドの幅を均等に分割した位置を計算
		float xPos = -gridHalfWidth + xIndex * kGridEvery;

		// 始点と終点のワールド座標を設定
		Vector3 worldStartPos(xPos, 0.0f, gridHalfWidth);
		Vector3 worldEndPos(xPos, 0.0f, -gridHalfWidth);

		// ラインを描画
		DrawGridLine(worldStartPos, worldEndPos);
	}
}

/// <summary>
/// 横線を描画します
/// </summary>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="gridHalfWidth"></param>
/// <param name="subdivision"></param>
void DrawHorizontalGridLines(float gridHalfWidth, uint32_t subdivision) {
	const float kGridEvery = (gridHalfWidth * 2.0f) / float(subdivision);

	for (uint32_t zIndex = 0; zIndex <= subdivision; ++zIndex) {
		// グリッドの幅を均等に分割した位置を計算
		float zPos = -gridHalfWidth + zIndex * kGridEvery;

		// 始点と終点のワールド座標を設定
		Vector3 worldStartPos(-gridHalfWidth, 0.0f, zPos);
		Vector3 worldEndPos(gridHalfWidth, 0.0f, zPos);

		// ラインを描画
		DrawGridLine(worldStartPos, worldEndPos);
	}
}

/// <summary>
/// 縦横の線を描画します
/// </summary>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
void DrawGrid() {
	const float kGridHalfWidth = 20.0f;
	const uint32_t kSubdivision = 10;

	DrawVerticalGridLines(kGridHalfWidth, kSubdivision);
	DrawHorizontalGridLines(kGridHalfWidth, kSubdivision);
}


GameScene::GameScene() {}

GameScene::~GameScene() { delete sprite_,	model_,	debugCamera_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("./Resources/uvChecker.png");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	// 3dモデルの生成
	model_ = Model::Create();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// サウンドdataの読み込み
	soundDataHandle_ = audio_->LoadWave("fanfare.wav");
	// 再生
	audio_->PlayWave(soundDataHandle_);

	// ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
	
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸法く表示の表示をゆうこうにする
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {
	ImGui::Begin("debug1");
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::Text("kamata tarou%d,%d,%d", 2050, 12, 31);
	 ImGui::ShowDemoWindow();
	ImGui::End();

	//スプライトの座標を取得
	 Vector2 pos = sprite_->GetPosition();
	//座標を{2,1}移動
	 pos.x += 2.0f;
	 pos.y += 1.0f;
	//移動した座標をスプライトに繁栄
	 sprite_->SetPosition(pos);

	// スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->StopWave(voiceHandle_);
	}

	//デバッグカメラの更新
	debugCamera_->Update();
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
	sprite_->Draw();

	
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
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
	// ラインの描画
	DrawGrid();
	
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
