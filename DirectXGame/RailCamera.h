#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include<memory>
/// <summary>
/// レールカメラ
/// </summary>
class RailCamera{
private:
	WorldTransform wTransform_;
	ViewProjection viewProjection_;

public:
	RailCamera();
	~RailCamera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 wPos);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection(){ return viewProjection_; }
	/// <summary>
	/// ワールド行列から取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition(){
		Vector3 wPos;
		wPos = wTransform_.translation_;
		return wPos;
	}

	const WorldTransform &GetWorldTransform()const { return wTransform_; }

	const ViewProjection& GetViewProjection()const{ return viewProjection_; }
};

