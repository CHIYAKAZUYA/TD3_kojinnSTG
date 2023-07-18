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
  public: // �����o�֐�
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	GameSceneBumpGame();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameSceneBumpGame();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();


	/// <summary>
	/// ���t���[������
	/// </summary>
	void GamePlayUpdate(); // �Q�[���v���C�X�V

	void Update();

	void PlayerUpdate(); // �v���C���[�X�V


	void collision(); // �Փ˔���

	/// <summary>
	/// �`��
	/// </summary>
	void GamePlayDraw3D();     // �Q�[���v���C3D�\��
	void GamePlayDraw2DBack(); // �Q�[���v���C�w�i2D�\��
	void GamePlayDraw2DNear(); // �Q�[���v���C�ߌi2D�\��

	void Draw();

	void TitleDraw2DNear(); // �^�C�g���ߌi2D

	void GameOverDraw2DNear(); // �Q�[���I�[�o�[�ߌi2D

	int sceneMode_ = 0; // �V�[�����[�h(0:�Q�[���v���C�@1:�^�C�g���@2:�Q�[���I�[�o�[)


	private://�����o�ϐ�
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// �Q�[���V�[���p
	/// </summary>
	

	// BG(�X�v���C�g)
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;

	// �r���[�v���W�F�N�V����(����)
	ViewProjection viewProjection_;

	// �v���C���[
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;
};

