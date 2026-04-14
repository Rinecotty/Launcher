#pragma once
#pragma once
#include "Base.h"
#include "SceneChanger.h"
//シーンの基底クラス.
class BaseScene :public Base
{
protected:
	SceneChanger* sceneChanger;
public:
	BaseScene(SceneChanger* changer);
	virtual ~BaseScene() {}
	virtual void Initialize() override {}//初期化処理.
	virtual void Finalize() override;		//終了処理.
	virtual void Update() override {}		//更新処理.
	virtual void Draw() override;			//描画処理.
};
