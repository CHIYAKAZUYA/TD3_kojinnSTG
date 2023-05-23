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
	void BeamMove();//ビーム移動
	void BeamBron();//ビーム発生

	void EnemyUpdate();//敵更新
	void EnemyMove();//敵移動
	void EnemyBorn();//敵発生
	void EnemyJamp();//敵ジャンプ

	void collision();//衝突判定
	void collisionPlayerEnemy();//プレイヤーと敵間の衝突判定
	void colisionBeamEnemy();//ビームと敵間の衝突判定

	void TitleUpdate();//タイトル更新

	void GameOverUpdate();//ゲームオーバー更新

	void GamePlayStart();//ゲーム開始時初期化関数

	void stageUpdate();//ステージ更新

	int GameScore_ = 0;//ゲームスコア
	int playerLife_ = 3;//プレイヤーライフ
	int GameTimer_ = 0;// タイマー変数

	int sceneMode_ = 1;//シーンモード(0:ゲームプレイ　1:タイトル　2:ゲームオーバー)

	/// <summary>
	/// 描画
	/// </summary>
	void GamePlayDraw3D();//ゲームプレイ3D表示
	void GamePlayDraw2DBack();//ゲームプレイ背景2D表示
	void GamePlayDraw2DNear();//ゲームプレイ近景2D表示
	
	void Draw();

	void TitleDraw2DNear(); // タイトル近景2D

	void GameOverDraw2DNear();//ゲームオーバー近景2D

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
	WorldTransform worldTransformStage_[20];
	
	//プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	//ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_[10];
	int BeamFlag[10] = {}; // ビーム存在フラグ
	int beamTimer = 0;//ビーム発生タイマー

	//敵
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_[10];
	int EnemyFlag[10] = {}; // 敵存在フラグ
	float enemySpeed[10] = {};
	float enemyJampSpeed[10] = {};

	//タイトル
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;

	//エンター表示
	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;

	//ゲームオーバー
	uint32_t textureHandleGameOver_ = 0;
	Sprite* spriteGameOver_ = nullptr;

	//サウンド
	uint32_t soundDataHandleTitleBGM_ = 0;//タイトルBGM
	uint32_t soundDataHandleGamePlayBGM_ = 0;//ゲームプレイBGM
	uint32_t soundDataHandleGameOverBGM_ = 0;//ゲームオーバーBGM
	uint32_t soundDataHandleEnemyHitSE_ = 0;//敵ヒットSE
	uint32_t soundDataHandlePlayerHitSE_ = 0;//プレイヤーヒットSE
	uint32_t voiceHandleBGM_ = 0;//音声再生ハンドル
};
