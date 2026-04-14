#pragma once
#include "SceneChanger.h"
#include "BaseScene.h"
class SceneMgr : public SceneChanger, Base {

private:
	BaseScene* mScene;    //シーン管理変数
	SceneList mNextScene;    //次のシーン管理変数
	SceneMgr();
	~SceneMgr() {}

public:
	void Initialize() override;//初期化
	void Finalize() override;//終了処理
	void Update() override;//更新
	void Draw() override;//描画

	// 引数 nextScene にシーンを変更する
	void ChangeScene(SceneList NextScene) override;
	static SceneMgr& GetInstance();
};
