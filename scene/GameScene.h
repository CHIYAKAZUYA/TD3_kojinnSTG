﻿#pragma once

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

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void GamePlayUpdate();//ゲームプレイ更新
	
	void Update();

	void PlayerUpdate();//プレイヤー更新

	void BeamUpdate();//ビーム更新
	void BeamMove();  //ビーム移動
	void BeamBron();  //ビーム発生

	void EnemyUpdate();//敵更新
	void EnemyMove();  //敵移動
	void EnemyBron();  //敵発生

	void collision();//衝突判定
	void collisionPlayerEnemy();//プレイヤーと敵間の衝突判定
	void colisionBeamEnemy();//ビームと敵間の衝突判定

	int GameScore_ = 0;//ゲームスコア
	int playerLife_ = 3;//プレイヤーライフ

	/// <summary>
	/// 描画
	/// </summary>
	void GamePlayDraw3D();//ゲームプレイ3D表示
	void GamePlayDraw2DBack();//ゲームプレイ背景2D表示
	void GamePlayDraw2DNear();//ゲームプレイ近景2D表示
	
	void Draw();

  private: // メンバ変数
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

	//ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	//ステージ
	uint32_t textureHandleStage_ = 0;
	Model* modelStage_ = nullptr;
	WorldTransform worldTransformStage_;
	
	//プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	//ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_;
	int BeamFlag = 0;//ビーム存在フラグ

	//敵
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_;
	int EnemyFlag = 1; // 敵存在フラグ
};
