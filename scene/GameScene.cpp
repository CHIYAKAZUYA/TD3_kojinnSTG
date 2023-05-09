#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include<time.h>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete spriteBG_;   // BG
	delete modelStage_; //ステージ
	delete modelPlayer_;//プレイヤー
	delete modelBeam_;//ビーム
	delete modelEnemy_;//敵
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// BG(2Dスプライト)
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

	//ビュープロジェクションの初期化
	viewProjection_.eye = {0, 1, -6};
	viewProjection_.target = {0, 1, 0};
	viewProjection_.Initialize();

	//ステージ
	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {4.5f, 1, 40};
	worldTransformStage_.Initialize();

	//プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	//ビーム
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	worldTransformBeam_.scale_ = {0.3f, 0.3f, 0.3f};
	worldTransformBeam_.Initialize();

	//敵
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy_.Initialize();
	EnemyFlag = 0;

	srand((unsigned int)time(NULL));
}

void GameScene::Update() { 
	
}

void GameScene::GamePlayUpdate() {
	PlayerUpdate(); // プレイヤー更新
	BeamUpdate();   // ビーム更新
	EnemyUpdate();  // 敵更新
	collision();    // 衝突判定更新
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	//背景
	spriteBG_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GamePlayDraw3D(){
	// ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

	// プレイヤー
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);

	// ビーム
	if (BeamFlag == 1) {
		modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);
	}

	// 敵
	if (EnemyFlag == 1) {
		modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
	}
}

void GameScene::GamePlayDraw2DBack() {
	// 背景
	spriteBG_->Draw();
}

void GameScene::GamePlayDraw2DNear() {
	// ゲームスコア
	char str[100];
	sprintf_s(str, "SCORE %d", GameScore_);
	debugText_->Print(str, 200, 10, 2);

	// プレイヤーライフ
	char str2[100];
	sprintf_s(str2, "LIFE %d", playerLife_);
	debugText_->Print(str2, 800, 10, 2);
}

//------------------
//プレイヤー
//------------------

//プレイヤー更新
void GameScene::PlayerUpdate() {
	//移動

	//右へ移動
	if (input_->PushKey(DIK_RIGHT)) 
	{
		if (worldTransformPlayer_.translation_.x <= 4)
		{
			worldTransformPlayer_.translation_.x += 0.1f;
		}
	}

	//左に移動
	if (input_->PushKey(DIK_LEFT)) 
	{
		if (worldTransformPlayer_.translation_.x >= -4) 
		{
			worldTransformPlayer_.translation_.x -= 0.1f;
		}
	}

	//行列更新
	worldTransformPlayer_.UpdateMatrix();
}

//------------------
//ビーム
//------------------

//ビーム更新
void GameScene::BeamUpdate() 
{
	//ビーム発生
	BeamBron();
	//移動
	BeamMove();

	//行列更新
	worldTransformBeam_.UpdateMatrix();
}

//ビーム発生
void GameScene::BeamBron()
{
	if (input_->PushKey(DIK_SPACE)) 
	{
		if (BeamFlag == 0) 
		{
			worldTransformBeam_.translation_.x =
				worldTransformPlayer_.translation_.x;
			worldTransformBeam_.translation_.z =
				worldTransformPlayer_.translation_.z;
			BeamFlag = 1;
		}
	}
}

//ビーム移動
void GameScene::BeamMove() 
{
	if (BeamFlag == 1) 
	{
		worldTransformBeam_.translation_.z += 1;
	}

	if (worldTransformBeam_.translation_.z > 40)
	{
		BeamFlag = 0;

	}

	//回転
	worldTransformBeam_.rotation_.x += 0.1f;
}


//------------------
// 敵
//------------------

void GameScene::EnemyUpdate() 
{ 
	//敵発生
	EnemyBron(); 

	//敵移動
	EnemyMove();

	// 行列更新
	worldTransformEnemy_.UpdateMatrix();
}

//敵発生
void GameScene::EnemyBron()
{
	if (EnemyFlag == 0)
	{
		int x = rand() % 80;
		float x2 = (float)x / 10 - 4;
		worldTransformEnemy_.translation_.x = x2;
		worldTransformEnemy_.translation_.z = 40;
		EnemyFlag = 1; 
	}

	if (EnemyFlag == 1)
	{
		if (worldTransformEnemy_.translation_.z < -5)
		{
			EnemyFlag = 0;
		}
	}
}

//敵移動
void GameScene::EnemyMove() 
{ 
	if (EnemyFlag == 1)
	{
		worldTransformEnemy_.translation_.z -= 0.5f;
	}

	// 回転
	worldTransformEnemy_.rotation_.x += 0.1f;
}

//------------------
// 衝突判定
//------------------

void GameScene::collision() 
{ 
	//プレイヤーと敵間の衝突判定
	collisionPlayerEnemy();

	//ビームと敵間の衝突判定
	colisionBeamEnemy();
}

//プレイヤーと敵間の衝突判定
void GameScene::collisionPlayerEnemy() 
{
	//敵が存在するなら
	if (EnemyFlag == 1) 
	{
		//差を求める
		float dx = abs(worldTransformPlayer_.translation_.x
			- worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformPlayer_.translation_.z
			- worldTransformEnemy_.translation_.z);

		//衝突したら
		if (dx < 1 && dz < 1)
		{
			playerLife_ -= 1;
			//存在しない
			EnemyFlag = 0;
		}
	}
}

//ビームと敵間の衝突判定
void GameScene::colisionBeamEnemy()
{
	// 敵が存在するなら
	if (EnemyFlag == 1)
	{
		// 差を求める
		float dx = abs(worldTransformBeam_.translation_.x
			- worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformBeam_.translation_.z
			- worldTransformEnemy_.translation_.z);

		// 衝突したら
		if (dx < 1 && dz < 1)
		{
			GameScore_ += 150;
			// 存在しない
			EnemyFlag = 0;
			BeamFlag = 0;
		}
	}
}