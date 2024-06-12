#include "Player.h"
#include"TextureManager.h"

#include<cassert>

Player::Player(){}

Player::~Player(){}

void Player::Initialize(Model* model,ViewProjection* viewprojection){
	assert(model);
	assert(viewprojection);
	model_ = model;
	viewProjection_ = viewprojection;
	worldTransform_.Initialize();
	textureHandle_ = TextureManager::Load("uvChecker.png");
}

void Player::Update(){
	worldTransform_.TransferMatrix();
}

void Player::Draw(){
	Actor::Draw();
}