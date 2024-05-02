#pragma once
#include<functional>

/// <summary>
/// 次元発動
/// </summary>
class TimedCall{
public:
	//コンストラクタ
	TimedCall(std::function<void()> func, uint32_t time);
	
	//更新
	void Update();
	//完了ならtureを返す
	bool IsFinished(){ return isFinished_; }

private:
	//コールバック
	std::function<void()> func_;
	uint32_t time_;
	bool isFinished_ = false;
};

