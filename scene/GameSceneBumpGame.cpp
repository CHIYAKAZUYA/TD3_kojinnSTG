#include "GameSceneBumpGame.h"

#include "TextureManager.h"
#include <cassert>
#include <time.h>

using namespace DirectX;

GameSceneBumpGame::GameSceneBumpGame() {}

GameSceneBumpGame::~GameSceneBumpGame()
{
	delete spriteBG_;    // BG
	delete modelPlayer_; // �v���C���[
}


void GameSceneBumpGame::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// BG(2D�X�v���C�g)
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

	// �r���[�v���W�F�N�V�����̏�����
	viewProjection_.eye = {0, 1, -6};
	viewProjection_.target = {0, 1, 0};
	viewProjection_.Initialize();

	// �v���C���[
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	srand((unsigned int)time(NULL));
}


void GameSceneBumpGame::Update()
{
	GameSceneBumpGame::GamePlayUpdate(); // �Q�[���v���C�X�V
}

void GameSceneBumpGame::GamePlayUpdate() {
	PlayerUpdate(); // �v���C���[�X�V
	collision();
}

void GameSceneBumpGame::Draw() {

	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(commandList);
	//switch (sceneMode_) {
	//case 0:
	//	GameSceneBumpGame::GamePlayDraw2DBack(); // �Q�[���v���C2D�w�i�\��
	//	break;

	//case 2:
	//	GameSceneBumpGame::GamePlayDraw2DBack(); // �Q�[���v���C2D�w�i�\��
	//	break;
	//}

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�W�F�N�g�`��O����
	Model::PreDraw(commandList);
	GameSceneBumpGame::GamePlayDraw3D();
	//switch (sceneMode_) {
	//case 0:
	//	GameSceneBumpGame::GamePlayDraw3D(); // �Q�[���v���C3D�\��
	//	break;
	//case 2:
	//	GameSceneBumpGame::GamePlayDraw3D(); // �Q�[���v���C3D�\��
	//	break;
	//}

	// 3D�I�u�W�F�N�g�`��㏈��
	Model::PostDraw();
#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(commandList);
	//switch (sceneMode_) {
	//case 0:
	//	GameSceneBumpGame::GamePlayDraw2DNear(); // �Q�[���v���C2D�ߌi�\��
	//	break;
	//case 1:
	//	GameSceneBumpGame::TitleDraw2DNear(); // �^�C�g��2D�\��
	//	break;
	//case 2:
	//	GameSceneBumpGame::GameOverDraw2DNear(); // �Q�[���I�[�o�[2D�\��
	//	GameSceneBumpGame::GamePlayDraw2DNear(); // �Q�[���v���C2D�ߌi�\��
	//	break;
	//}

	//// �f�o�b�O�e�L�X�g�̕`��
	debugText_->DrawAll(commandList);
	//
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

#pragma endregion
}

void GameSceneBumpGame::GamePlayDraw3D()
{
	// �v���C���[
	modelPlayer_->Draw(
		worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
}


//------------------
// �v���C���[
//------------------

// �v���C���[�X�V
void GameSceneBumpGame::PlayerUpdate() {
	// �ړ�
	worldTransformPlayer_.rotation_.y += 0.1f;

	// �E�ֈړ�
	if (input_->PushKey(DIK_RIGHT)) {
		
	}

	// ���Ɉړ�
	if (input_->PushKey(DIK_LEFT)) {
		
	}

	// �s��X�V
	worldTransformPlayer_.UpdateMatrix();
}

//------------------
// �Փ˔���
//------------------

void GameSceneBumpGame::collision()
{

}
