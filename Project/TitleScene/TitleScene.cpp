#include "TitleScene.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	//delete sprite_;
}

void TitleScene::Initialize()
{
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	Sprite::StaticInitialize();

	texhandle_ = TextureManager::Load("resources/TitleDemo.png");
	texhandle1_ = TextureManager::Load("resources/TitleDemoStart.png");
	texhandle2_ = TextureManager::Load("resources/black.png");

	sprite_.reset(Sprite::Create(texhandle_));
	sprite1_.reset(Sprite::Create(texhandle1_));
	sprite2_.reset(Sprite::Create(texhandle2_));


	pos = { 100.0f,36.0f };
	time = 0.0f;
	i = 0.1f;

	color = 0.0f;

	flag = true;
}

void TitleScene::Update() {	
	viewProjection_.UpdateMatrix();
	Sprite::StaticUpdate();
	
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
	sprite_->Draw();
	sprite1_->Draw();
	if (flag == false) {
		sprite2_->Draw();
	}
	ImGui::Begin("Title");
	ImGui::Checkbox("flag", &flag);
	if (ImGui::TreeNode("transform")) {
		ImGui::SliderAngle("posX", &pos.x);
		ImGui::SliderAngle("posY", &pos.y);	
		ImGui::TreePop();
	}
	ImGui::End();
}