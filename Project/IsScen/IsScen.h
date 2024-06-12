#pragma once
#include <memory>

enum SCENE { TITLE, GAME};

class IScene
{
protected:
	// シーンナンバー
	static int sceneNo_;

public:

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual ~IScene();

	int GetSceneNo();

};