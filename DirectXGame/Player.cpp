#include "Player.h"
#include <algorithm>
#include"MyFunc.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include"CollisionManager.h"
#include"WinApp.h"
#include"ViewProjection.h"

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

Player::~Player(){ delete sprite2DReticle_; }

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

	Move();
	Shoot();
	Rotate();
	worldTransform_.UpdateMatrix();

	ReticleUpdate(viewProjection);


	// 弾の更新
	for (auto& bullet : bullets_){
		bullet->Update();
	}

	// 生存フラグがfalseの弾を削除
	bullets_.remove_if([] (const std::unique_ptr<PlayerBullet>& bullet){
		if (bullet->GetIsDead()){
			//衝突判定リストから削除
			CollisionManager::GetInstance()->RemoveCollider(bullet.get());
			return true;
		}
		return false;
					   });


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
	ImGui::End();
#endif // _DEBUG

	Matrix4x4 m = viewProjection.matView;

	///================================================================
	///	3dレティクル
	///=================================================================

	//レールカメラを考慮しないプレイヤーの座標
	Matrix4x4 playerMatWorld = Matrix4x4::MakeAffineMatrix(worldTransform_.scale_,
														   worldTransform_.rotation_,
														   worldTransform_.translation_);
	// 自機からレティクルへの距離
	float kDistancePlayerTo3DReticle = 50.0f;
	// 自機からレティクルへのオフセット(z+向き)
	Vector3 offset = {0, 0, 1.0f};
	offset = Matrix4x4::Transform(offset, playerMatWorld);

	// ベクトルの長さを整える
	offset = Normalize(offset) * kDistancePlayerTo3DReticle;

	auto GetPos = [] (const Matrix4x4& matrix) -> Vector3{
		return Vector3 {matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]};
	};

	Vector3 playerWorldPos = GetPos(playerMatWorld);

	wTransform3DReticle_.translation_ = playerWorldPos + offset + railScrollVal_;
	wTransform3DReticle_.UpdateMatrix();


	///================================================================
	///	2dレティクル
	///=================================================================

	//3dレティクルのワールド座標から2dレティクルのスクリーン座標を計算
	Vector3 reticlePos = GetPos(wTransform3DReticle_.matWorld_);
	//ビューポート処理
	Matrix4x4 matViewPort = Matrix4x4::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Matrix4x4::Multiply(matViewPort, Matrix4x4::Multiply(viewProjection.matView, viewProjection.matProjection));
	//ワールドからスクリーン座標に変換
	reticlePos = Matrix4x4::Transform(reticlePos, matViewProjectionViewport);
	sprite2DReticle_->SetPosition(Vector2(
								  reticlePos.x + 1280 * 0.5f,
								  reticlePos.y + 720 * 0.5f));
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
		// 自キャラの座標をコピー
		Vector3 pos = worldTransform_.translation_;

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 BulletVel = {0, 0, kBulletSpeed};

		// 速度ベクトルを自キャラの向きに合わせて回転
		BulletVel = TransformNormal(BulletVel, this->worldTransform_.matWorld_);

		// 弾を生成してユニークポインタにラップ
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, GetWorldPosition(), BulletVel);

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

void Player::OnCollision(){

}

void Player::SetParent(const WorldTransform* parent){
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}