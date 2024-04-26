#include "BaseEnemyState.h"
#include"ConvertString.h"

BaseEnemyState::BaseEnemyState(const std::string& name, Enemy* enemy)
	:name_(name), enemy_(enemy){};

void BaseEnemyState::DebugLog(){
	Log(name_);
}