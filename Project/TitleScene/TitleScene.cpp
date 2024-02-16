#include "TitleScene.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	//delete sprite_;
}

void TitleScene::Initialize()
{
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	texhandle_ = TextureManager::Load("resources/TitleDemo.png");
	texhandle1_ = TextureManager::Load("resources/TitleDemoStart.png");
	texhandle2_ = TextureManager::Load("resources/black.png");

	sprite_.reset(Sprite::Create({ 640.0f,360.0f }, { 1280.0f,720.0f }, { 0.5f,0.5f }));
	sprite1_.reset(Sprite::Create({ 640.0f,360.0f }, { 1280.0f,720.0f }, { 0.5f,0.5f }));
	sprite2_.reset(Sprite::Create({ 640.0f,360.0f }, { 1280.0f,720.0f }, { 0.5f,0.5f }));

	pos = { 640.0f,360.0f };
	time = 0.0f;
	i = 0.1f;

	color = 0.0f;

	flag = true;
}

void TitleScene::Update() {	
	viewProjection_.UpdateMatrix();

	
	sprite1_->SetPosition({ pos.x,pos.y });
	sprite2_->SetColor({ 1,1,1,color });

	pos.y += i;
	time++;
	if (time > 90) {
		i *= -1;
		time = 0;
	}
	if (input_->PushKey(DIK_SPACE)) {
		flag = false;
	}

	if (flag ==false) {
		color += 0.01f;
	}

	if (color >= 2) {
		sceneNo_ = GAME;
	}
}

void TitleScene::Draw()
{
	sprite_->Draw(viewProjection_, texhandle_);
	sprite1_->Draw(viewProjection_, texhandle1_);
	if (flag == false) {
		sprite2_->Draw(viewProjection_, texhandle2_);
	}
	ImGui::Begin("Title");
	ImGui::Checkbox("flag", &flag);
	ImGui::End();
}