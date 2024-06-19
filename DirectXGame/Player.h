#pragma once
#include"Actor.h"
#include"MyFunc.h"

class Player:public Actor {

public:
	Player();
	~Player()override;

	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection& viewProjection)override;

	void SetViewProjection(const ViewProjection* viewProjection);

private:
	void Move();

private:
	//カメラのビュープロジェクション
	const ViewProjection* viewPorjection_ = nullptr;
	float targetAngle;
};

