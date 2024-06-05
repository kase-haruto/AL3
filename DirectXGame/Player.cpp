#include "Player.h"
#include <algorithm>
#include"MyFunc.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include"CollisionManager.h"
#include"WinApp.h"
#include"ViewProjection.h"
#include<cmath>
#include"MyMath.h"

Player::Player(){
	//衝突属性
	collisionAttribute_ = 0b1;
	//衝突マスク(相手)
	collisionMask_ = ~collisionAttribute_;
	SetCollisionAttribute(collisionAttribute_);//敵陣営
	SetCollisionMask(collisionMask_);//敵陣営以外

	//コライダーリストに追加
	CollisionManager::GetInstance()->SetCollider(this);
}

Player::~Player(){
	delete sprite2DReticle_;
	for (auto& bullet : bullets_){
		bullet.reset();
	}
}

void Player::Init(Model* model, Vector3 pos){
	assert(model);
	model_ = model;
	textuerHandle_ = TextureManager::Load("./Resources/uvChecker.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	velocity_ = {0.4f, 0.4f, 0.4f};
	radius_ = 1.0f;
	input_ = Input::GetInstance();
	wTransform3DReticle_.Initialize();

	//レティクル用テクスチャの取得
	uint32_t textureReticle = TextureManager::Load("./Resources/reticle.png");
	//スプライト生成
	Vector4 spriteColor = Vector4 {1,1,1,1};
	sprite2DReticle_ = Sprite::Create(textureReticle,
									  Vector2(wTransform3DReticle_.translation_.x,
									  wTransform3DReticle_.translation_.y),
									  spriteColor,
									  {0.5f,0.5f});
	sprite2DReticle_->SetSize({64.0f,64.0f});
}

void Player::Update(const ViewProjection& viewProjection){
	///==============================
	//古い球の削除
	bullets_.remove_if([] (const std::unique_ptr<PlayerBullet>& bullet){
		if (bullet->GetIsDead()){
			//衝突判定リストから削除
			CollisionManager::GetInstance()->RemoveCollider(bullet.get());
			return true;
		}
		return false;
					   });

	//===============================

	Move();
	Shoot();
	Rotate();
	worldTransform_.UpdateMatrix();

	ReticleUpdate(viewProjection);

	// 弾の更新
	for (auto& bullet : bullets_){
		bullet->Update();
	}

#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.01f);
	ImGui::End();
#endif // _DEBUG
}

void Player::ReticleUpdate(const ViewProjection& viewProjection){

#ifdef _DEBUG
	ImGui::Begin("reticle");
	ImGui::DragFloat3("translate", &wTransform3DReticle_.translation_.x, 0.01f);
	ImGui::Text("2DreticlePos:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("isLockOn:(%d)", isLockOn);
	ImGui::End();
#endif // _DEBUG


	//===============================
	// マウス座標を取得
	POINT mousePoint;
	GetCursorPos(&mousePoint);

	// クライアント座標に変換
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePoint);
	Vector2 mousePos = {static_cast< float >(mousePoint.x), static_cast< float >(mousePoint.y)};
	sprite2DReticle_->SetPosition(mousePos);
	//===============================

	//================================
	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matViewport = Matrix4x4::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matVPV = Matrix4x4::Multiply(Matrix4x4::Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	// 合成行列の逆行列
	Matrix4x4 matInverseVPV = Matrix4x4::Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3(mousePos.x, mousePos.y, 0);
	Vector3 posFar = Vector3(mousePos.x, mousePos.y, 1);

	// スクリーン座標系からワールド座標系
	posNear = Matrix4x4::Transform(posNear, matInverseVPV);
	posFar = Matrix4x4::Transform(posFar, matInverseVPV);
	//==================================

	//====================================
	// 3Dレティクルの座標計算
	// マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Normalize(mouseDirection);

	// カメラから参照オブジェクトの距離
	const float kDistanceTestObject = 70.0f;
	wTransform3DReticle_.translation_ = posNear + (mouseDirection * kDistanceTestObject);
	wTransform3DReticle_.UpdateMatrix();
	//====================================

	LockOn(viewProjection);
}

void Player::LockOn(const ViewProjection& viewProjection){
	// レティクルから敵までの距離を計算し、レティクルの位置を更新
	Vector2 reticlePos = sprite2DReticle_->GetPosition();
	float reticleSize = 32.0f;
	bool foundTarget = false;
	for (const auto& targetPos : targetPos_){
		Vector3 scTargetPos = WorldToScreen(targetPos, viewProjection);

		float dx = scTargetPos.x - reticlePos.x;
		float dy = scTargetPos.y - reticlePos.y;
		float dist = sqrtf(dx * dx + dy * dy);

		if (dist <= reticleSize){
			sprite2DReticle_->SetPosition({scTargetPos.x, scTargetPos.y});
			lockOnTargetPos_ = targetPos;
			foundTarget = true;
			break;
		}
	}
	isLockOn = foundTarget;
}

void Player::Draw(ViewProjection& viewprojection){
	Actor::Draw(viewprojection);
	for (auto& bullet : bullets_){
		bullet->Draw(viewprojection);
	}
	model_->Draw(wTransform3DReticle_, viewprojection);
}

void Player::DrawUi(){
	sprite2DReticle_->Draw();
}

void Player::Shoot(){
	if (input_->TriggerKey(DIK_SPACE)){
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 BulletVel = {0, 0, kBulletSpeed};

		auto GetWpos = [] (const Matrix4x4& matrix) ->Vector3{
			return Vector3 {matrix.m[3][0],matrix.m[3][1],matrix.m[3][2]};
		};

		// 弾の速度を設定
		Vector3 targetPos = isLockOn ? lockOnTargetPos_ : GetWpos(wTransform3DReticle_.matWorld_);

		BulletVel = targetPos - this->GetWorldPosition();
		BulletVel = Normalize(BulletVel) * kBulletSpeed;

		// 弾を生成してユニークポインタにラップ
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, this->GetWorldPosition(), BulletVel);

		// 弾を登録
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Move(){
	Vector3 move = {0, 0, 0};

	if (input_->PushKey(DIK_A)){
		move.x -= velocity_.x;
	} else if (input_->PushKey(DIK_D)){
		move.x += velocity_.x;
	}

	if (input_->PushKey(DIK_S)){
		move.y -= velocity_.y;
	} else if (input_->PushKey(DIK_W)){
		move.y += velocity_.y;
	}

	worldTransform_.translation_ += move;

	//移動限界座標
	const float kMoveLimitX = 17.0f;
	const float kMoveLimitY = 10.0f;

	worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

}

void Player::Rotate(){
	//回転の速さ
	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_RIGHT)){
		worldTransform_.rotation_.y += kRotSpeed;
	} else if (input_->PushKey(DIK_LEFT)){
		worldTransform_.rotation_.y -= kRotSpeed;
	}
}

Vector3 Player::GetWorldPosition()const{
	Vector3 wPos;
	//ワールド行列の平行移動成分を取得
	wPos.x = worldTransform_.matWorld_.m[3][0];
	wPos.y = worldTransform_.matWorld_.m[3][1];
	wPos.z = worldTransform_.matWorld_.m[3][2];
	return wPos;
}

void Player::OnCollision(){}

void Player::SetParent(const WorldTransform* parent){
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

void Player::SetTargetPos(const std::vector<Vector3>& targetPos){
	targetPos_ = targetPos;
}