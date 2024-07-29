#include "LockOn.h"
#include "TextureManager.h"
#include "GlobalVariables.h"
#include"WinApp.h"
#include "MyFunc.h"

#include <cmath>


LockOn::LockOn() : lockOnMark_(nullptr, [] (Sprite* p){ delete p; }){
    const char* groupName = "LockOn";
    GlobalVariables* globalVariables = GlobalVariables::GetInstance();
    globalVariables->CreateGroup(groupName);

    globalVariables->AddItem(groupName, "minDistance", minDistance_);
    globalVariables->AddItem(groupName, "maxDistance", maxDistanec_);
    globalVariables->AddItem(groupName, "angleRange", angleRange_);
}

void LockOn::Initialize(){
    lockOnMark_ = std::unique_ptr<Sprite, void(*)(Sprite*)>(
        Sprite::Create(TextureManager::Load("reticle.png"), markerPos_, {1.0f, 0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}),
        [] (Sprite* p){ delete p; }
    );
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection){
    prePadState = padState;

    XInputGetState(0, &padState);

    ApplyGlobalVariables();

    // 現在のボタン状態を取得
    WORD currentButtonState = padState.Gamepad.wButtons;
    WORD preButtonState = prePadState.Gamepad.wButtons;

    bool isLeftShoulderPressed = (currentButtonState & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
    bool wasLeftShoulderPressed = (preButtonState & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;

    bool isLeftShoulderJustPressed = isLeftShoulderPressed && !wasLeftShoulderPressed;

    //ロックオン状態なら
    if (target_){
        // ロックオン状態時の処理を追加
        if (isLeftShoulderJustPressed){
            target_ = nullptr;
        }
    } else{
        //ロックオントリガーを押したら
        if (isLeftShoulderJustPressed){
            SelectTarget(enemies, viewProjection);
        }
    }

    if (target_){
        //ロックオンmarkerの座標計算
        Vector3 positionWorld = target_->GetCenter();
       
        //ビューポート行列
        Matrix4x4 matViewport = Matrix4x4::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
        Matrix4x4 matviewProjectionViewport = Matrix4x4::Multiply(Matrix4x4::Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
        //ワールド座標からスクリーン座標
        Vector3 positionScreen = Matrix4x4::Transform(positionWorld,matviewProjectionViewport);
        //vector2に格納
        Vector2 positionScreenV2(positionScreen.x, positionScreen.y);
        //スプライトの座標を設定
        lockOnMark_->SetPosition(positionScreenV2);
    }
}

void LockOn::Draw(){
    //ロックオン時にmarker描画
    if (target_){
        lockOnMark_->Draw();
    }
}

void LockOn::ApplyGlobalVariables(){
    GlobalVariables* globalVariables = GlobalVariables::GetInstance();
    const char* groupName = "LockOn";

    minDistance_ = globalVariables->GetValue<float>(groupName, "minDistance");
    maxDistanec_ = globalVariables->GetValue<float>(groupName, "maxDistance");
    angleRange_ = globalVariables->GetValue<float>(groupName, "angleRange");
}

void LockOn::SelectTarget(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection){
    //目標
    std::list<std::pair<float, const Enemy*>> targets;

    //すべての敵に対して純にロックオン判定
    for (const std::unique_ptr<Enemy>& enemy : enemies){
        //敵のロックオン座標取得
        Vector3 positionWorld = enemy->GetCenter();
        //ワールド座標からビュー座標変換
        Vector3 positionView = Matrix4x4::Transform(positionWorld, viewProjection.matView);

        //距離条件チェック
        if (minDistance_ <= positionView.z && positionView.z <= maxDistanec_){
            //カメラ前方との角度を計算
            float arcTangent = std::atan2(
                std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z
            );

            //角度条件チェック
            if (std::abs(arcTangent) <= angleRange_){
                targets.emplace_back(std::make_pair(positionView.z, enemy.get()));
            }
        }
    }

    //ロックオン対象のリセット
    target_ = nullptr;

    if (!targets.empty()){
        //距離で昇順にソート
        targets.sort([] (const auto& pair1, const auto& pair2){ return pair1.first < pair2.first; });

        //ソートの結果一番近い敵をロックオン対象とする
        target_ = targets.front().second;
    }
}

Vector3 LockOn::GetTargetPosition()const{
    if (target_){
        return target_->GetCenter();
    }
    return Vector3();
}