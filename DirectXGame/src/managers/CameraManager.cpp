#include "CameraManager.h"

CameraManager* CameraManager::GetInstance(){
	static CameraManager instance;
	return &instance;
}

FollowCamera* CameraManager::GetFollowCamera()const{
	return followCamera_;
}

void CameraManager::SetActiveCamera(FollowCamera* camera){
	followCamera_ = camera;
}

void CameraManager::Update(){
	if (followCamera_){
		followCamera_->Update();
	}
}