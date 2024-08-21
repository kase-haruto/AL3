#pragma once
#include"BaseStronghold.h"

class EnemyStronghold:public BaseStronghold{
public:
	/// <summary>
	/// コンストラクタ
	/// デストラクタ
	/// </summary>
	EnemyStronghold() = default;
	~EnemyStronghold()override = default;

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

private:

};

