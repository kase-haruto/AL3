#pragma once
#include"Actor.h"
#include"MyFunc.h"
#include"Model.h"

#include<vector>
#include<optional>

namespace{
	/// <summary>
	/// プレイヤーのパーツ
	/// </summary>
	enum class Parts{
		body,
		head,
		L_arm,
		R_arm,
		weapon,
		partsCount
	};

	enum class Behavior{
		root,//通常
		attack,//攻撃
		dash,//ダッシュ中
		jump,//ジャンプ中
	};

	struct WorkDash{
		//ダッシュ用の媒介変数
		uint32_t dashParameter_ = 0;
	};

	struct WorkAttack{
		uint32_t attackParameter_ = 0;
		int32_t comboIndex = 0;
		int32_t inComboPhase = 0;
		bool comboNext = false;
	};

	struct ConstAttack{
		//振りかぶりの時間
		uint32_t anticipationTime;
		//ための時間
		uint32_t chargeTime;
		//攻撃の振りの時間
		uint32_t swingTime;
		//硬直時間
		uint32_t recoveryTime;
		//振りかぶりの移動の速さ
		float anticipationSpeeed;
		//ための移動の速さ
		float chrgeSpee;
		//攻撃振りの移動速度
		float swingSpeed;
	};
}

class Player :public Actor{



public:
	Player();
	~Player()override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& model)override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection)override;
	/// <summary>
	/// 追尾カメラ
	/// </summary>
	void SetViewProjection(const ViewProjection* viewProjection);

private:
	/// <summary>
	/// 各パーツのtransformの初期化
	/// </summary>
	void PartsTransformInit();
	/// <summary>
	/// 移動
	/// </summary>
	void Move();
	/// <summary>
	/// 向いている方向に進む
	/// </summary>
	void MoveInDirection(float speed);
	/// <summary>
	/// 浮遊行動の初期化
	/// </summary>
	void InitializeFloatingAction();
	/// <summary>
	/// 通常行動の初期化
	/// </summary>
	void RootInitialize();
	/// <summary>
	/// 攻撃処理の初期化
	/// </summary>
	void AttackInitialize();
	/// <summary>
	/// ダッシュ行動の初期化
	/// </summary>
	void BehaviorDashInitialize();
	/// <summary>
	/// ジャンプ行動初期化
	/// </summary>
	void BehaviorJumpInitialize();
	/// <summary>
	/// 浮遊行動の更新
	/// </summary>
	void UpdateFloatingAction();
	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();
	/// <summary>
	/// 攻撃行動更新
	/// </summary>
	void BehaviorAttackUpdate();
	/// <summary>
	/// ダッシュ行動の更新
	/// </summary>
	void BehaviorDashUpdate();
	/// <summary>
	/// ジャンプ行動更新
	/// </summary>
	void BehaviorJumpUpdate();
	/// <summary>
	/// ふるまいの遷移
	/// </summary>
	void TrasitionaBehavior();
	/// <summary>
	/// ふるまいの更新
	/// </summary>
	void BehaviorUpdate();


	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();
private:
	//攻撃コンボ数
	static const int ComboNum = 3;
	//コンボ定数表
	static const std::array<ConstAttack, ComboNum>kConstAttacks_;


	Vector3 velocity_;

	//カメラのビュープロジェクション
	const ViewProjection* viewPorjection_ = nullptr;
	float targetAngle;

	//パーツごとの変数
	std::vector<std::unique_ptr< WorldTransform>> partsTransform_;

	//浮遊ギミックの媒介変数
	int32_t cycle_ = 30;
	float floatingParameter_ = 0.0f;
	//浮遊の振幅
	float floatingAmplitude;

	bool isAttack_ = false;

	//ふるまい
	Behavior behavior_ = Behavior::root;
	//次の振る舞いのリクエスト
	std::optional<Behavior>behaviorRequest_ = std::nullopt;

	//ダッシュ用変数
	WorkDash workDash_;
	WorkAttack workAttack_;
};

