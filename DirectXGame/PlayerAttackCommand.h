#pragma once
#include"Player.h"

class PlayerAttackCommand{
	public:
		virtual ~PlayerAttackCommand() = default;
		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize(Player* player) = 0;
		/// <summary>
		/// コマンドの実行
		/// </summary>
		/// <param name="player"></param>
		virtual void Execute(Player* player) = 0;

		bool GetIsFinished(){ return isFinished_; }

protected:
	Player* player_ = nullptr;
	bool isFinished_ = false;

	float movingStep_ = 0.21f;
};

