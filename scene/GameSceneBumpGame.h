#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

class GameSceneBumpGame 
{
  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameSceneBumpGame();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameSceneBumpGame();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();


	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void GamePlayUpdate(); // ゲームプレイ更新

	void Update();

	void PlayerUpdate(); // プレイヤー更新


	void collision(); // 衝突判定

	/// <summary>
	/// 描画
	/// </summary>
	void GamePlayDraw3D();     // ゲームプレイ3D表示
	void GamePlayDraw2DBack(); // ゲームプレイ背景2D表示
	void GamePlayDraw2DNear(); // ゲームプレイ近景2D表示

	void Draw();

	void TitleDraw2DNear(); // タイトル近景2D

	void GameOverDraw2DNear(); // ゲームオーバー近景2D

	int sceneMode_ = 0; // シーンモード(0:ゲームプレイ　1:タイトル　2:ゲームオーバー)


	private://メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	

	// BG(スプライト)
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;

	// ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	// プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;
};

