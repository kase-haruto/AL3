#include "PlayableCharacterManager.h"
#include "Input.h"
#include "CameraManager.h"
#include "FollowCamera.h"

void PlayableCharacterManager::Update(){
    XINPUT_STATE padState = {};
    XInputGetState(0, &padState);  // padStateを正しく取得

    // 現在のキャラクターが生きているかチェック
    if (!characters_[currentCharacterIndex_]->GetIsAlive()){
        // 生きているキャラクターを探して切り替え
        for (int i = 0; i < characters_.size(); ++i){
            if (characters_[i]->GetIsAlive()){
                SwitchCharacter(i);
                break;
            }
        }
        // 両方とも死んでいる場合、何もしない
        return;
    }

    // 'B' が押されたときにキャラクターを切り替え
    if (padState.Gamepad.wButtons & XINPUT_GAMEPAD_B){
        int newCharacterIndex = (currentCharacterIndex_ == 0) ? 1 : 0;

        // 新しいキャラクターが生きている場合のみ切り替え
        if (characters_[newCharacterIndex]->GetIsAlive()){
            SwitchCharacter(newCharacterIndex);
        }
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

int PlayableCharacterManager::GetCurrentCharacterIndex(){
    return currentCharacterIndex_;
}
