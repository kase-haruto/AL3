#pragma once
#include"Actor.h"

class Player:public Actor {
private:

public:
	Player();
	~Player()override;

	void Initialize(Model* model,ViewProjection* viewProhection);
	void Update();
	void Draw()override;
};

