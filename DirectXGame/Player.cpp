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
	wTransform3DReticle_.translation_ = this->worldTransform_.translation_;
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
	ImGui::End();
#endif // _DEBUG


	// スプライトの現在の座標を取得
	Vector2 spritePosition = sprite2DReticle_->GetPosition();
	XINPUT_STATE joyState;

	// ジョイスティック状態の取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)){
		// ジョイスティックの入力を正規化
		float joyX = static_cast< float >(joyState.Gamepad.sThumbRX) / SHRT_MAX;
		float joyY = static_cast< float >(joyState.Gamepad.sThumbRY) / SHRT_MAX;

		// デッドゾーンの設定
		const float deadZone = 0.2f;

		// ジョイスティックのデッドゾーンを考慮
		if (fabs(joyX) > deadZone){
			spritePosition.x += joyX * 5.0f;
		}
		if (fabs(joyY) > deadZone){
			spritePosition.y -= joyY * 5.0f;
		}
	}

	//===============================
	// マウス座標を取得
	POINT mousePoint;
	GetCursorPos(&mousePoint);

	// クライアント座標に変換
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePoint);
	Vector2 mousePos = {static_cast< float >(mousePoint.x), static_cast< float >(mousePoint.y)};
	sprite2DReticle_->SetPosition(mousePos);
	//================================

	sprite2DReticle_->SetPosition(isPad_ ? spritePosition : mousePos);

	//================================
	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matViewport = Matrix4x4::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matVPV = Matrix4x4::Multiply(Matrix4x4::Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	// 合成行列の逆行列
	Matrix4x4 matInverseVPV = Matrix4x4::Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3(spritePosition.x, spritePosition.y, 0);
	Vector3 posFar = Vector3(spritePosition.x, spritePosition.y, 1);

	// スクリーン座標系からワールド座標系
	posNear = Matrix4x4::Transform(posNear, matInverseVPV);
	posFar = Matrix4x4::Transform(posFar, matInverseVPV);
	//==================================

	//====================================
	// 3Dレティクルの座標計算
	// レティクルレイの方向
	Vector3 reticleDirection = posFar - posNear;
	reticleDirection = Normalize(reticleDirection);

	// カメラから参照オブジェクトの距離
	const float kDistanceTestObject = 70.0f;
	wTransform3DReticle_.translation_ = posNear + (reticleDirection * kDistanceTestObject);
	wTransform3DReticle_.UpdateMatrix();
	//====================================


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

	XINPUT_STATE joyState;

	if (!Input::GetInstance()->GetJoystickState(0, joyState)){
		isPad_ = false;
	}

	if (input_->TriggerKey(DIK_SPACE) ||
		joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER){
		// 自キャラの座標をコピー
		Vector3 pos = worldTransform_.translation_;

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 BulletVel = {0, 0, kBulletSpeed};

		auto GetWpos = [] (const Matrix4x4& matrix) ->Vector3{
			return Vector3 {matrix.m[3][0],matrix.m[3][1],matrix.m[3][2]};
		};

		// 自機から照準オブジェクトのベクトル
		BulletVel = GetWpos(wTransform3DReticle_.matWorld_) - GetWorldPosition();
		BulletVel = Normalize(BulletVel) * kBulletSpeed;

		// 弾を生成してユニークポインタにラップ
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, GetWorldPosition(), BulletVel);

		// 弾を登録
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Move(){
	const float kCharacterSpeed = 1.0f;
	Vector3 move = {0, 0, 0};

	//ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	///==================================
	///	ゲームパッドの移動処理
	if (Input::GetInstance()->GetJoystickState(0, joyState)){
		move.x += ( float ) joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += ( float ) joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}
	///==================================


	///==================================
	///	キーボードの移動処理
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
	///==================================


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