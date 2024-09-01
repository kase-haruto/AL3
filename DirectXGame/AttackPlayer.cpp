#include "AttackPlayer.h"
#include"TextureManager.h"

void AttackPlayer::Initialize(const std::vector<Model*>& model){
	Player::Initialize(model);
}

void AttackPlayer::InitializeUi(){
    hpSpriteHandle_ = TextureManager::Load("./Resources/hpBar.png");
    hpBar_.reset(Sprite::Create(hpSpriteHandle_, {70.0f,50.0f}, {1, 1, 1, 1}, {0.0f,0.0f}));
    Vector2 size {500.0f,50.0f};
    hpBar_->SetSize(size);

    iconSpriteHandle_ = TextureManager::Load("./Resources/playerAttackIcon.png");
    icon_.reset(Sprite::Create(iconSpriteHandle_, {10,50}));
    Vector2 textureSize {50,50};
    icon_->SetSize(textureSize);
}

void AttackPlayer::Update(){
	Player::Update();
}

void AttackPlayer::Draw(const ViewProjection& viewProjection){
	Player::Draw(viewProjection);
}

void AttackPlayer::OnCollision(Collider* other){
	Player::OnCollision(other);
}
