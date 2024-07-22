#pragma once
#include"PlayerAttackCommand.h"

class PlayerWeaponSwingDown :
	public PlayerAttackCommand{
public:
	~PlayerWeaponSwingDown(){}
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
	float shakeUpAngle_ = -3.1f;
	Vector3 weaponInitAngle_ = {0.0f,0.0f,0.0f};
};

