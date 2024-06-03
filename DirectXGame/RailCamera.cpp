#include "RailCamera.h"
#include<imgui.h>
#include"MyMath.h"
#include"PrimitiveDrawer.h"
#include"MyFunc.h"
#include<algorithm>
#include<cmath>

RailCamera::RailCamera(){}

RailCamera::~RailCamera(){}

void RailCamera::Initialize(Vector3 wPos){
	wTransform_.translation_ = wPos;
	wTransform_.rotation_ = {0.0f,0.0f,0.0f};
	wTransform_.scale_ = {1.0f,1.0f,1.0f};

	viewProjection_.Initialize();
	wTransform_.UpdateMatrix(false);

	/*ctrlPoints_ = {
		{0,0,0},
		{10,10,0},
		{10,15,0},
		{20,15,0},
		{20,0,0},
		{30,0,0},
	};*/

	ctrlPoints_ = {
		{0,0,0},
		{0,0,10},
		{0,5,30},
		{0,5,60},
		{0,0,80},
		{0,0,100},
	};
}

void RailCamera::Update(){
	ImGui::Begin("camera");
	ImGui::DragFloat3("translation", &wTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation", &wTransform_.rotation_.x, 0.01f);
	ImGui::End();

	// ビュー行列を計算
	viewProjection_.matView = Matrix4x4::Inverse(wTransform_.matWorld_);

	t_ += 0.001f;
	t_ = std::clamp(t_, 0.0f, 1.0f);

	// スプライン上の現在のカメラ位置を計算
	Vector3 eye = CatmullRomPosition(ctrlPoints_, t_);

	float t_2 = t_ + 0.001f;
	t_2 = std::clamp(t_2, 0.0f, 1.0f);

	// 注視点を求める
	Vector3 target = CatmullRomPosition(ctrlPoints_, t_2);

	// targetとeyeの差分ベクトル
	Vector3 forward = target - eye;

	// 方向ベクトルを正規化
	forward = Normalize(forward);

	// トランスフォームの更新
	wTransform_.translation_ = eye;
	float horizontalDistance = sqrtf(forward.x * forward.x + forward.z * forward.z);
	wTransform_.rotation_.x = std::atan2(-forward.y, horizontalDistance);
	wTransform_.rotation_.y = std::atan2(forward.x, forward.z);

	// 行列の更新
	wTransform_.UpdateMatrix(false);
}


void RailCamera::DrawLine(ViewProjection& viewProjection){
	std::vector<Vector3> pointsDrawing;
	const size_t segmentCount = 100;

	// Catmull-Romスプラインのポイントを計算
	for (size_t i = 0; i < segmentCount + 1; i++){
		float t = 1.0f / segmentCount * i;
		Vector3 pos = CatmullRomPosition(ctrlPoints_, t);
		pointsDrawing.push_back(pos);
	}

	// ビュー投影行列の設定
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection);

	// ライン描画
	for (size_t i = 0; i < segmentCount; i++){
		PrimitiveDrawer::GetInstance()->DrawLine3d(pointsDrawing[i], pointsDrawing[i + 1], {1.0f, 1.0f, 1.0f, 1.0f});
	}
}

void RailCamera::Draw(){}