#include "RailCamera.h"
#include<imgui.h>
RailCamera::RailCamera(){}

RailCamera::~RailCamera(){}

void RailCamera::Initialize(Vector3 wPos){
	wTransform_.translation_ = wPos;
	wTransform_.rotation_ = {0.0f,0.0f,0.0f};
	wTransform_.scale_ = {1.0f,1.0f,1.0f};

	viewProjection_.Initialize();
}

void RailCamera::Update(){
	ImGui::Begin("camera");
	ImGui::DragFloat3("translation", &wTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation", &wTransform_.rotation_.x, 0.01f);
	ImGui::End();

	wTransform_.translation_.z += -0.001f;
	wTransform_.UpdateMatrix(false);
	viewProjection_.matView = Matrix4x4::Inverse(wTransform_.matWorld_);
}