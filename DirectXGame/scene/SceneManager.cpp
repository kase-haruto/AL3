#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include"ResultScene.h"

bool SceneManager::isClear_ = false;
bool SceneManager::isFailure_ = false;

SceneManager::SceneManager(){
    ChangeScene(std::make_unique<TitleScene>());
}

// シングルトンのインスタンスを取得するメソッド
SceneManager* SceneManager::GetInstance(){
    static SceneManager instance;
    return &instance;
}

void SceneManager::ChangeScene(std::unique_ptr<IScene> newScene){
    if (currentScene_){
        currentScene_->Finalize();
    }
    currentScene_ = std::move(newScene);
    currentScene_->Initialize();
    sceneChanged_ = true;  // シーンが切り替わったことを記録

    return;
}

void SceneManager::Update(){
    if (sceneChanged_){
        sceneChanged_ = false;  // 次のフレームで更新をスキップ
        return;
    }

    if (currentScene_){
        currentScene_->Update();
    }
}

void SceneManager::Draw(){
    if (currentScene_){
        currentScene_->Draw();
    }
}

void SceneManager::SetIsExit(){
    isExitGame_ = true;
}

bool SceneManager::CheckIsExit(){
    return isExitGame_;
}