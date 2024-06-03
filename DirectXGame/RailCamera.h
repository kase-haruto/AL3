#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include<memory>
#include<vector>
/// <summary>
/// レールカメラ
/// </summary>
class RailCamera{
private:
	WorldTransform wTransform_;
	ViewProjection viewProjection_;

	Vector3 scrollVal_;

	std::vector<Vector3>ctrlPoints_;
	

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
	/// 線の描画
	/// </summary>
	void DrawLine(ViewProjection& viewProjection);
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
		//ワールド行列の平行移動成分を取得
		wPos.x = wTransform_.matWorld_.m[3][0];
		wPos.y = wTransform_.matWorld_.m[3][1];
		wPos.z = wTransform_.matWorld_.m[3][2];
		return wPos;
	}

	const WorldTransform &GetWorldTransform()const { return wTransform_; }

	const ViewProjection& GetViewProjection()const{ return viewProjection_; }

	Vector3 GetScrollVal()const{ return scrollVal_; }
};

