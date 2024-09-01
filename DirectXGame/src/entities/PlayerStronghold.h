#pragma once
#include "BaseStronghold.h"

#include"Player.h"
class PlayerStronghold :
    public BaseStronghold{
public:
	/// <summary>
	/// コンストラクタ
	/// デストラクタ
	/// </summary>
	PlayerStronghold() = default;
	~PlayerStronghold()override = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model)override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection)override;

	void SetProtectPlayer(Player* player){ protectPlayer_ = player; }

	Player* GetPlayerPtr()const{ return protectPlayer_; }
private:
	Player* protectPlayer_ = nullptr;
};



