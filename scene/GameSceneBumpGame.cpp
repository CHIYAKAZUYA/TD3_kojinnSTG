#include "GameSceneBumpGame.h"

#include "TextureManager.h"
#include <cassert>
#include <time.h>

using namespace DirectX;

GameSceneBumpGame::GameSceneBumpGame() {}

GameSceneBumpGame::~GameSceneBumpGame()
{
	delete spriteBG_;    // BG
	delete modelPlayer_; // プレイヤー
}


void GameSceneBumpGame::Initialize()
{
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

	// プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	srand((unsigned int)time(NULL));
}


void GameSceneBumpGame::Update()
{
	GameSceneBumpGame::GamePlayUpdate(); // ゲームプレイ更新
}

void GameSceneBumpGame::GamePlayUpdate() {
	PlayerUpdate(); // プレイヤー更新
	collision();
}

void GameSceneBumpGame::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	//switch (sceneMode_) {
	//case 0:
	//	GameSceneBumpGame::GamePlayDraw2DBack(); // ゲームプレイ2D背景表示
	//	break;

	//case 2:
	//	GameSceneBumpGame::GamePlayDraw2DBack(); // ゲームプレイ2D背景表示
	//	break;
	//}

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	GameSceneBumpGame::GamePlayDraw3D();
	//switch (sceneMode_) {
	//case 0:
	//	GameSceneBumpGame::GamePlayDraw3D(); // ゲームプレイ3D表示
	//	break;
	//case 2:
	//	GameSceneBumpGame::GamePlayDraw3D(); // ゲームプレイ3D表示
	//	break;
	//}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);
	//switch (sceneMode_) {
	//case 0:
	//	GameSceneBumpGame::GamePlayDraw2DNear(); // ゲームプレイ2D近景表示
	//	break;
	//case 1:
	//	GameSceneBumpGame::TitleDraw2DNear(); // タイトル2D表示
	//	break;
	//case 2:
	//	GameSceneBumpGame::GameOverDraw2DNear(); // ゲームオーバー2D表示
	//	GameSceneBumpGame::GamePlayDraw2DNear(); // ゲームプレイ2D近景表示
	//	break;
	//}

	//// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameSceneBumpGame::GamePlayDraw3D()
{
	// プレイヤー
	modelPlayer_->Draw(
		worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
}


//------------------
// プレイヤー
//------------------

// プレイヤー更新
void GameSceneBumpGame::PlayerUpdate() {
	// 移動
	worldTransformPlayer_.rotation_.y += 0.1f;

	// 右へ移動
	if (input_->PushKey(DIK_RIGHT)) {
		
	}

	// 左に移動
	if (input_->PushKey(DIK_LEFT)) {
		
	}

	// 行列更新
	worldTransformPlayer_.UpdateMatrix();
}

//------------------
// 衝突判定
//------------------

void GameSceneBumpGame::collision()
{

}
