#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, Model* model2){
	//NULLポインタチェック
	assert(model);

	//引数のデータをメンバ変数に代入
	model_ = model;
	model2_ = model2;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.scale_ = { 3,3,3 };
	worldTransform_.translation_ = { 0,-18,0 };

	//ワールド座標変換の初期化
	worldTransform_.Initialize();
}

void Player::Update(){
	//ファイル名を指定してテクスチャを読み込む
	if (playerColor == 0) {
		textureHandle_ = TextureManager::Load("black.png");
	}
	else if (playerColor == 1) {
		textureHandle_ = TextureManager::Load("white.png");
	}

	//デスフラグが立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
	});

	//移動限界座標
	const float kMoveLimitX = 85.0f;
	const float kMoveLimitY = 40.0f;
	const float kMoveLimitZ = 70.0f;


	//移動関数
	Move();
	//回転関数
	Rotate();

	//弾を出す
	Attack();

	if (input_->PushKey(DIK_R)) {
		worldTransform_.rotation_.z += 0.5f;
	}

	if (input_->TriggerKey(DIK_C)) {
		isChange = true;

		if (playerColor == 0) {
			playerColor = 1;
		}
		else if (playerColor == 1) {
			playerColor = 0;
		}
	}

	if (isChange == true) {

		if (state == 1) {
			worldTransform_.rotation_.z -= 0.75f;
			worldTransform_.translation_.x += dashSpeed;
			dashSpeed-=0.5f;
		}
		if (state == 2) {
			worldTransform_.rotation_.z += 0.75f;
			worldTransform_.translation_.x -= dashSpeed;
			dashSpeed-=0.5f;
		}
		
		
		if (worldTransform_.rotation_.z >= 6.5|| worldTransform_.rotation_.z <= -6.5) {
			isChange = false;
			worldTransform_.rotation_.z = 0;
			dashSpeed = 4.0f;
		}
	}

	//弾の更新処理
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//移動制限
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	worldTransform_.translation_.z = max(worldTransform_.translation_.z, -kMoveLimitZ);
	worldTransform_.translation_.z = min(worldTransform_.translation_.z, +kMoveLimitZ);

	//行列変換
	MatrixConvert();

	//行列を送信
	worldTransform_.TransferMatrix();

	//デバッグ
	/*debugText_->SetPos(20, 20);
	debugText_->Printf(
		"PlayerPos %f,%f,%f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);*/
	/*debugText_->SetPos(20, 100);
	debugText_->Printf("%f",worldTransform_.rotation_.z);

	debugText_->SetPos(20, 120);
	debugText_->Printf("%d", state);

	debugText_->SetPos(20, 60);
	debugText_->Printf("UP/DOWN/RIGHT/LEFT Key PlayerMove");

	debugText_->SetPos(20, 80);
	debugText_->Printf("SPACE Key PlayerBullet");*/
}

//描画関数
void Player::Draw(ViewProjection &viewProjection){
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	model2_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_){
		bullet->Draw(viewProjection);
	}
}

//行列変換関数
void Player::MatrixConvert(){
	Matrix4 matScale;
	Matrix4 matRotX, matRotY, matRotZ;
	Matrix4 matRot;
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	Matrix4 matTrans2 = MathUtility::Matrix4Identity();
	matScale.ScaleSet(worldTransform_.scale_);
	matRotZ.RotZSet(worldTransform_.rotation_.z);
	matRotX.RotXSet(worldTransform_.rotation_.x);
	matRotY.RotYSet(worldTransform_.rotation_.y);
	matTrans.TransSet(worldTransform_.translation_);
	matRot = matRotZ * matRotX * matRotY;
	//合成
	worldTransform_.matWorld_ = matScale * matRot * matTrans;
}

//移動関数
void Player::Move(){
	//キャラクターの移動ベクトル
	move = { 0,0,0 };

	if (input_->PushKey(DIK_RIGHT)) {
		move += { 0.5f,0,0 };
		if (isChange == false) {
			state = 1;
		}
	}
	if (input_->PushKey(DIK_LEFT)) {
		move += { -0.5f,0,0 };
		if (isChange == false) {
			state = 2;
		}
	}
	if (input_->PushKey(DIK_UP)) {
		move += { 0,0,0.5f };
	}
	if (input_->PushKey(DIK_DOWN)) {
		move += { 0,0,-0.5f };
	}
	if (input_->PushKey(DIK_Z)) {
		move += { 0, 0.5f, 0};
	}
	if (input_->PushKey(DIK_X)) {
		move += { 0, -0.5f, 0};
	}

	worldTransform_.translation_ += move;
}

//回転関数
void Player::Rotate(){
	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y -= 0.05f;
	}
	else if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y += 0.05f;
	}
}

//弾の攻撃
void Player::Attack()
{
	coolTime--;

	if (input_->PushKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity.multiplyMat4(worldTransform_.matWorld_);

		if (coolTime < 0) {
			//弾の生成と初期化
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(model2_, worldTransform_.translation_, velocity, playerColor);
			std::unique_ptr<PlayerBullet> newBullet2 = std::make_unique<PlayerBullet>();
			newBullet2->Initialize2(model2_, worldTransform_.translation_, velocity, playerColor);
			//弾を登録する
			bullets_.push_back(std::move(newBullet));
			bullets_.push_back(std::move(newBullet2));

			coolTime = 7;
		}
	}
}

//ワールド座標を取得
Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Player::OnCollision()
{
}
