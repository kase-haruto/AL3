#include "RailCamera.h"
#include<imgui.h>
#include"MyMath.h"
#include"PrimitiveDrawer.h"

RailCamera::RailCamera(){}

RailCamera::~RailCamera(){}

void RailCamera::Initialize(Vector3 wPos){
	wTransform_.translation_ = wPos;
	wTransform_.rotation_ = {0.0f,0.0f,0.0f};
	wTransform_.scale_ = {1.0f,1.0f,1.0f};

	viewProjection_.Initialize();
	wTransform_.UpdateMatrix(false);

	ctrlPoints_ = {
		{0,0,0},
		{10,10,0},
		{10,15,0},
		{20,15,0},
		{20,0,0},
		{30,0,0},
	};
}

void RailCamera::Update(){
	ImGui::Begin("camera");
	ImGui::DragFloat3("translation", &wTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation", &wTransform_.rotation_.x, 0.01f);
	ImGui::End();

	/*wTransform_.translation_.z += 0.1f;
	scrollVal_.z += 0.1f;*/
	wTransform_.UpdateMatrix(false);
	viewProjection_.matView = Matrix4x4::Inverse(wTransform_.matWorld_);
}

void RailCamera::DrawLine(ViewProjection& viewProjection){
	std::vector<Vector3> pointsDrawing;
	const size_t segmentCount = 100;

	// Catmull-Romスプラインのポイントを計算
	for (size_t i = 0; i < segmentCount+1; i++){
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