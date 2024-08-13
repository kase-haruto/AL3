#pragma once
#include "PlayerAttackCommand.h"
class PlayerMowDown :
    public PlayerAttackCommand{
public:
	~PlayerMowDown(){}
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player)override;
	/// <summary>
	/// コマンドの実行
	/// </summary>
	/// <param name="player"></param>
	void Execute(Player* player)override;

private:
	float targetArmAngle_ = 1.3f;
	float shakeUpAngle_ = -1.6f;

	Vector3 bodyInitRotation = {0.0f,-1.2f,0.0f};
	Vector3 bodyTargetRotation = {0.0f,1.2f,0.0f};

	Vector3 weaponInitRotation_ = {0.5f,0.25f,1.6f};
	Vector3 weaponInitTranslation_ = {0.0f,1.6f,0.0f};

	Vector3 weaponTargetAngle_ = {2.7f,0.25f,1.6f};
};

