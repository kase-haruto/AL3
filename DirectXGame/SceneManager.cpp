#include "SceneManager.h"
#include"TitleScene.h"
#include"GameScene.h"

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
}

void SceneManager::Update(){
	if (currentScene_){
		currentScene_->Update();
	}
}

void SceneManager::Draw(){
	if (currentScene_){
		currentScene_->Draw();
	}
}