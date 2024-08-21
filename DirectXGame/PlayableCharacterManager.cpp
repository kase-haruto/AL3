#include "PlayableCharacterManager.h"
#include"Input.h"
#include"CameraManager.h"
#include"FollowCamera.h"

void PlayableCharacterManager::Update(){
	// 'R' キーが押されたときにキャラクターを切り替え
	if (Input::GetInstance()->TriggerKey(DIK_R)){
		// 現在のキャラクターインデックスを切り替え
		int newCharacterIndex = (currentCharacterIndex_ == 0) ? 1 : 0;
		SwitchCharacter(newCharacterIndex);
	}
}

void PlayableCharacterManager::AddCharacters(Player* player){
	characters_.push_back(player);
}

Player* PlayableCharacterManager::GetCurrentCharacter(){
	return characters_[currentCharacterIndex_];
}

void PlayableCharacterManager::SwitchCharacter(int newCharacterIndex){
    if (newCharacterIndex >= 0 && newCharacterIndex < characters_.size()){
        // 現在のキャラクターを非アクティブに設定
        characters_[currentCharacterIndex_]->SetIsActive(false);

        // 新しいキャラクターをアクティブに設定
        currentCharacterIndex_ = newCharacterIndex;
        characters_[currentCharacterIndex_]->SetIsActive(true);

        // CameraManagerを使用してカメラのターゲットを新しいキャラクターに設定
        FollowCamera* followCamera = CameraManager::GetInstance()->GetFollowCamera();
        if (followCamera){
            followCamera->SetTarget(&characters_[currentCharacterIndex_]->GetWorldTransform());
        }
    }
}