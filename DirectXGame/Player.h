#pragma once
#include"Actor.h"
#include"MyFunc.h"
#include"Model.h"

#include<vector>
#include<optional>

namespace PlayerDetails{
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

}

using PlayerDetails::Behavior;
using PlayerDetails::WorkDash;
using PlayerDetails::Parts;

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
	/// 向いている方向に進む
	/// </summary>
	void MoveInDirection(float speed);
	/// <summary>
	/// 追尾カメラ
	/// </summary>
	void SetViewProjection(const ViewProjection* viewProjection);


	///==========================================================
	///ゲッター
	///==========================================================
	bool GetIsAttack()const;
	std::vector<std::unique_ptr< WorldTransform>> GetPartsTransform(){ return std::move(partsTransform_); }
	Vector3 GetVelocity()const;
	Vector3 GetDirection()const;
	std::optional<Behavior> GetBehaviorRequest()const;

	///==========================================================
	///セッター
	///==========================================================
	void SetIsAttack(const bool isAttack);
	void SetVelocity(const Vector3& vel);
	void SetDirection(const Vector3& dir);
	void SetBehavior(const std::optional<Behavior>& BehaviorRequest);
	//translation
	void SetHeadTranslation(const Vector3& translation);
	void Set_L_ArmTranslation(const Vector3& translation);
	void Set_R_ArmTranslation(const Vector3& translation);
	void SetBodyTranslation(const Vector3& translation);
	//rotation
	void SetHeadRotation(const Vector3& rotation);
	void Set_L_ArmRotation(const Vector3& rotation);
	void Set_R_ArmRotation(const Vector3& rotation);
	void SetBodyRotation(const Vector3& rotation);

#pragma region
	void SetHeadTranslationX(const float translationX);
	void SetHeadTranslationY(const float translationY);
	void SetHeadTranslationZ(const float translationZ);

	void SetBodyTranslationX(const float translationX);
	void SetBodyTranslationY(const float translationY);
	void SetBodyTranslationZ(const float translationZ);

	void Set_L_ArmTranslationX(const float translationX);
	void Set_L_ArmTranslationY(const float translationY);
	void Set_L_ArmTranslationZ(const float translationZ);

	void Set_R_ArmTranslationX(const float translationX);
	void Set_R_ArmTranslationY(const float translationY);
	void Set_R_ArmTranslationZ(const float translationZ);

#pragma endregion パーツtranslation

#pragma region
	void SetHeadRotationX(const float RotationX);
	void SetHeadRotationY(const float RotationY);
	void SetHeadRotationZ(const float RotationZ);

	void SetBodyRotationX(const float RotationX);
	void SetBodyRotationY(const float RotationY);
	void SetBodyRotationZ(const float RotationZ);

	void Set_L_ArmRotationX(const float RotationX);
	void Set_L_ArmRotationY(const float RotationY);
	void Set_L_ArmRotationZ(const float RotationZ);

	void Set_R_ArmRotationX(const float RotationX);
	void Set_R_ArmRotationY(const float RotationY);
	void Set_R_ArmRotationZ(const float RotationZ);
#pragma endregion パーツrotation


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
};

