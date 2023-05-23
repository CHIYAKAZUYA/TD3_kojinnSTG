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
	delete spriteTitle_;//タイトル
	delete spriteEnter_;//エンター
	delete spriteGameOver_;//ゲームオーバー
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// BG(2Dスプライト)
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

	// ビュープロジェクションの初期化
	viewProjection_.eye = {0, 1, -6};
	viewProjection_.target = {0, 1, 0};
	viewProjection_.Initialize();

	// ステージ
	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {4.5f, 1, 40};
	worldTransformStage_.Initialize();

	// プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	// ビーム
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	for (int i = 0; i < 10; i++) 
	{
		worldTransformBeam_[i].scale_ = {0.3f, 0.3f, 0.3f};
		worldTransformBeam_[i].Initialize();
	}

	// 敵
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	for (int i = 0; i < 10; i++)
	{
		worldTransformEnemy_[i].scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformEnemy_[i].Initialize();
	}

	//タイトル
	textureHandleTitle_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});

	//エンター
	textureHandleEnter_ = TextureManager::Load("enter.png");
	spriteEnter_ = Sprite::Create(textureHandleEnter_, {390, 420});

	//ゲームオーバー
	textureHandleGameOver_ = TextureManager::Load("gameover.png");
	spriteGameOver_ = Sprite::Create(textureHandleGameOver_, {0, 100});

	//サウンドデータの読み込み
	soundDataHandleTitleBGM_ = audio_->LoadWave("Audio/Ring05.wav");
	soundDataHandleGamePlayBGM_ = audio_->LoadWave("Audio/Ring08.wav");
	soundDataHandleGameOverBGM_ = audio_->LoadWave("Audio/Ring09.wav");
	soundDataHandleEnemyHitSE_ = audio_->LoadWave("Audio/chord.wav");
	soundDataHandlePlayerHitSE_ = audio_->LoadWave("Audio/tada.wav");

	//タイトルBGMを再生
	voiceHandleBGM_ = audio_->PlayWave(soundDataHandleTitleBGM_, true);

	srand((unsigned int)time(NULL));
}

void GameScene::Update() 
{
	GameTimer_++;
	//各シーンの更新処理を呼び出す
	switch (sceneMode_) 
	{
	case 0:
		GameScene::GamePlayUpdate();//ゲームプレイ更新
		break;
	case 1:
		GameScene::TitleUpdate(); // タイトル更新
		break;
	case 2:
		GameScene::GameOverUpdate(); // ゲームオーバー更新
		break;
	}
}

void GameScene::GamePlayUpdate() {
	PlayerUpdate(); // プレイヤー更新
	BeamUpdate();   // ビーム更新
	EnemyUpdate();  // 敵更新
	collision();    // 衝突判定更新

	if (playerLife_ <= 0)
	{
		//BGM切り替え
		//BGMを停止
		audio_->StopWave(voiceHandleBGM_);
		//ゲームオーバーBGMを再生
		voiceHandleBGM_ = audio_->PlayWave(soundDataHandleGameOverBGM_, true);
		sceneMode_ = 2;
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	switch (sceneMode_) 
	{
	case 0:
		GameScene::GamePlayDraw2DBack(); // ゲームプレイ2D背景表示
		break;
	
	case 2:
		GameScene::GamePlayDraw2DBack(); // ゲームプレイ2D背景表示
		break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	switch (sceneMode_) 
	{
	case 0:
		GameScene::GamePlayDraw3D(); // ゲームプレイ3D表示
		break;
	case 2:
		GameScene::GamePlayDraw3D(); // ゲームプレイ3D表示
		break;
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);
	switch (sceneMode_) 
	{
	case 0:
		GameScene::GamePlayDraw2DNear(); // ゲームプレイ2D近景表示
		break;
	case 1:
		GameScene::TitleDraw2DNear();//タイトル2D表示
		break;
	case 2:
		GameScene::GameOverDraw2DNear();//ゲームオーバー2D表示
		GameScene::GamePlayDraw2DNear(); // ゲームプレイ2D近景表示
		break;
	}

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
	for (int i = 0; i < 10; i++) 
	{
		if (BeamFlag[i] == 1) {
			modelBeam_->Draw(worldTransformBeam_[i], viewProjection_, textureHandleBeam_);
		}
	}

	// 敵
	for (int i = 0; i < 10; i++)
	{
		if (EnemyFlag[i] == 1) {
			modelEnemy_->Draw(worldTransformEnemy_[i], viewProjection_, textureHandleEnemy_);
		}
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

	for (int i = 0; i < 10; i++) 
	{
		// 行列更新
		worldTransformBeam_[i].UpdateMatrix();
	}
}

//ビーム発生
void GameScene::BeamBron()
{ 
	//ビーム発生タイマーが0ならば
	if (beamTimer == 0) 
	{
		for (int b = 0; b < 10; b++) {
			if (input_->PushKey(DIK_SPACE)) {
				if (BeamFlag[b] == 0) {
					worldTransformBeam_[b].translation_.x =
						worldTransformPlayer_.translation_.x;
					worldTransformBeam_[b].translation_.z =
						worldTransformPlayer_.translation_.z;
					BeamFlag[b] = 1;
					beamTimer = 1;
					break;
				}
			}
		}
	}
	else 
	{
		//ビームタイマーが1以上
		//ビームタイマーが10を超えたら発射可能
		beamTimer++;
		if (beamTimer > 10) 
		{
			beamTimer = 0;
		}
	}
}

//ビーム移動
void GameScene::BeamMove() 
{
	for (int i = 0; i < 10; i++) 
	{
		if (BeamFlag[i] == 1) {
			worldTransformBeam_[i].translation_.z += 1;

			if (worldTransformBeam_[i].translation_.z > 40) {
				BeamFlag[i] = 0;
			}
		}

		// 回転
		worldTransformBeam_[i].rotation_.x += 0.1f;
	}
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
	for (int i = 0; i < 10; i++)
	{
		worldTransformEnemy_[i].UpdateMatrix();
	}
}

//敵発生
void GameScene::EnemyBron()
{
	if (rand() % 10 == 0)
	{
		for (int e = 0; e < 10; e++) {
			if (rand() % 2 == 0) {
				enemySpeed[e] = 0.2f;
			} 
			else 
			{
				enemySpeed[e] = -0.2f;
			}
			
			if (EnemyFlag[e] == 0) {
				int x = rand() % 80;
				float x2 = (float)x / 10 - 4;
				worldTransformEnemy_[e].translation_.x = x2;
				worldTransformEnemy_[e].translation_.z = 40;
				EnemyFlag[e] = 1;
				break;
			}

			else if (EnemyFlag[e] == 1) {
				if (worldTransformEnemy_[e].translation_.z < -5) {
					EnemyFlag[e] = 0;
				}
			}
		}
	}
}

//敵移動
void GameScene::EnemyMove() 
{ 
	for (int i = 0; i < 10; i++) 
	{
		if (EnemyFlag[i] == 1) {
			worldTransformEnemy_[i].translation_.z -= 0.2f;
			worldTransformEnemy_[i].translation_.x += enemySpeed[i];

			if (worldTransformEnemy_[i].translation_.x >= 5)
			{
				enemySpeed[i] = -0.2f;
			} 
			else if (worldTransformEnemy_[i].translation_.x <= -5) 
			{
				enemySpeed[i] = 0.2f;
			}

			// 回転
			worldTransformEnemy_[i].rotation_.x += 0.1f;
		}
	}
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
	for (int e = 0; e < 10; e++) 
	{
		// 敵が存在するなら
		if (EnemyFlag[e] == 1) {
			// 差を求める
			float dx = abs(worldTransformPlayer_.translation_.x
				  - worldTransformEnemy_[e].translation_.x);
			float dz = abs(worldTransformPlayer_.translation_.z
				  - worldTransformEnemy_[e].translation_.z);

			// 衝突したら
			if (dx < 1 && dz < 1) {
				playerLife_ -= 1;

				// 存在しない
				EnemyFlag[e] = 0;

				//プレイヤーヒットSE
				audio_->PlayWave(soundDataHandlePlayerHitSE_);
				break;
			}
		}
	}
}

//ビームと敵間の衝突判定
void GameScene::colisionBeamEnemy() 
{
	for (int e = 0; e < 10; e++) 
	{
		// 敵が存在するなら
		if (EnemyFlag[e] == 1) 
		{
			for (int b = 0; b < 10; b++) 
			{// 差を求める
				float dx = abs(worldTransformBeam_[b].translation_.x
					- worldTransformEnemy_[e].translation_.x);
				float dz = abs(worldTransformBeam_[b].translation_.z
					- worldTransformEnemy_[e].translation_.z);

				// 衝突したら
				if (dx < 1 && dz < 1) {
					GameScore_ += 150;
					// 存在しない
					EnemyFlag[e] = 0;
					BeamFlag[b] = 0;

					//敵ヒットSEを再生
					audio_->PlayWave(soundDataHandleEnemyHitSE_);
				}
				break;
			}
		}
	}
}

//------------------
// タイトル
//------------------
//タイトル更新
void GameScene::TitleUpdate() {
	if (input_->TriggerKey(DIK_RETURN)) 
	{
		GameScene::GamePlayStart();
		sceneMode_ = 0;

		//BGM切り替え
		// BGMを停止
		audio_->StopWave(voiceHandleBGM_);
		// ゲームプレイBGMを再生
		voiceHandleBGM_ = audio_->PlayWave(soundDataHandleGamePlayBGM_, true);
	}
}

//タイトル表示
void GameScene::TitleDraw2DNear() 
{ 
	spriteTitle_->Draw(); 
	if (GameTimer_ % 40 >= 20) 
	{
		//エンター表示
		spriteEnter_->Draw();
	}
}


//------------------
// ゲームオーバー
//------------------
// ゲームオーバー更新
void GameScene::GameOverUpdate() {
	if (input_->PushKey(DIK_RETURN)) {
		//BGM切り替え
		//BGMを停止
		audio_->StopWave(voiceHandleBGM_);
		//タイトルBGMを再生
		audio_->PlayWave(soundDataHandleTitleBGM_, true);
		sceneMode_ -= 1;
	}
}

// ゲームオーバー表示
void GameScene::GameOverDraw2DNear() {
	spriteGameOver_->Draw();
	if (GameTimer_ % 40 >= 20) {
		// エンター表示
		spriteEnter_->Draw();
	}
}

void GameScene::GamePlayStart() { 
	playerLife_ = 3;
	GameScore_ = 0;
	for (int i = 0; i < 10; i++) 
	{
		EnemyFlag[i] = 0;
		BeamFlag[i] = 0;
	}
}