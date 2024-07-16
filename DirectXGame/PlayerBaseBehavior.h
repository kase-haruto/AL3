#pragma once

class Player;

class PlayerBaseBehavior {
public:

	virtual ~PlayerBaseBehavior() = default;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	
protected:
	// Playerクラスのポインタ
	Player* player_;
};

