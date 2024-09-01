#include "TransitionSelectCube.h"
#include"Input.h"
#include"MyFunc.h"
#ifdef _DEBUG
#include<imgui.h>

#endif // _DEBUG

#include<numbers>

void TransitionSelectCube::Initialize(const Vector3& pos){
    //===================================================
    //　   worldTransformの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_.y = 1.3f;
	worldTransform_.UpdateMatrix();

    //===================================================
    //　   モデルの初期化
	modelCube_.reset(Model::CreateFromOBJ("cube", false));
}

void TransitionSelectCube::Update(){
#ifdef _DEBUG
    ImGui::Begin("cube");
    ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
    ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.01f);
    ImGui::End();
#endif // _DEBUG

    //モードの変更
    TransitionMode();

    // 行列を更新
    worldTransform_.UpdateMatrix();
}

void TransitionSelectCube::Draw(const ViewProjection& viewProjection){
	modelCube_->Draw(worldTransform_,viewProjection);
}


void TransitionSelectCube::TransitionMode(){
    // 回転角度の目標値を設定
    static float targetAngle = worldTransform_.rotation_.y;


    if (Input::GetInstance()->TriggerKey(DIK_RIGHT)){
        targetAngle += rotateAngle_;
    }

    if (Input::GetInstance()->TriggerKey(DIK_LEFT)){
        targetAngle -= rotateAngle_;
    }

    // 現在の回転角度を目標角度に向けて補完する
    worldTransform_.rotation_.y = Lerp(worldTransform_.rotation_.y, targetAngle, 0.1f);

    
}

