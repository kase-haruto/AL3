#pragma once
#include<string>

//前方宣言
class Enemy;

class BaseEnemyState{
protected:
	//状態名
	std::string name_;
	//対象の敵
	Enemy* enemy_ = nullptr;

public:
	BaseEnemyState(const std::string& name, Enemy* enemy);
	virtual ~BaseEnemyState(){};

	//更新処理
	virtual void Update() = 0;

	//デバッグ出力
	virtual void DebugLog();
};