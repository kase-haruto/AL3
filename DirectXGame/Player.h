#pragma once
#include "Actor.h"
#include"Input.h"
#include"PlayerBullet.h"
#include<list>
#include<memory>
#include"Sprite.h"
#include<vector>
#include <unordered_map>
class Player : 
	public Actor {

	struct Target{
		Vector3 pos;
		Sprite* marker;
		bool isLockOn = false;
		bool isAlive = true;
	};

private:
	Vector3 velocity_;
	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	Input* input_ = nullptr;
	
	//2dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
	//3Dレティクル用ワールドトランスフォーム
	WorldTransform wTransform3DReticle_;
	Vector3 railScrollVal_;
	uint32_t textureReticle_ = TextureManager::Load("./Resources/reticle.png");

	//targetの座標
	std::vector<Vector3>targetPos_;
	Vector3 lockOnTargetPos_;
	bool isLockOn = false;
	std::vector<Target> target_;
	
private://メンバ関数
	/// <summary>
	/// playerの移動処理を行います
	/// </summary>
	void Move();
	/// <summary>
	/// playerの回転を行います
	/// </summary>
	void Rotate();
	/// <summary>
	/// 弾を撃つ関数
	/// </summary>
	void Shoot ();
	/// <summary>
	/// マルチロックオン時の発射処理
	/// </summary>
	void MultiLockOnShoot();
	/// <summary>
	/// シングルロックオン時の発射処理
	/// </summary>
	void SingleLockOnShoot();
	/// <summary>
	/// ロックオン
	/// </summary>
	/// <returns></returns>
	void SingleLockOn(const ViewProjection& viewProjection);
	/// <summary>
	/// マルチロックオン
	/// </summary>
	/// <param name="viewProjection"></param>
	void MultiLockOn(const ViewProjection& viewProjection);


public://メンバ関数

	Player();
	~Player ()override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(Model* model,Vector3 pos);
	/// <summary>
	/// 更新
	/// </summary>
	void Update(const ViewProjection& viewProjection);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewprojection) override;
	/// <summary>
	/// ui描画
	/// </summary>
	void DrawUi();
	/// <summary>
	/// レティクルの更新
	/// </summary>
	void ReticleUpdate(const ViewProjection& viewProjection);
	/// <summary>
	/// 衝突したの処理
	/// </summary>
	void OnCollision()override;

	Vector3 GetVelocity() const { return velocity_; }
	void SetVelocity(const Vector3 velocity) { velocity_ = velocity; }
	Vector3 GetWorldPosition()const override;

	void setScrollVal(const Vector3 scrollVal){ railScrollVal_ = scrollVal; }

	/// <summary>
	/// 弾のリストを取得
	/// </summary>
	/// <returns></returns>
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets()const{ return bullets_; }
	/// <summary>
	/// targetの座標を取得
	/// </summary>
	/// <param name="targetPos"></param>
	void SetTargetPos(const std::vector<Vector3>& targetPos);
	void SetTargetIsAlive(const std::vector<bool>& isAlive);
	void SetParent(const WorldTransform* parent);
};
