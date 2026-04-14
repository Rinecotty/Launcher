#include "stdafx.h"
#include "SceneMgr.h"
#include"Select.h"

SceneMgr::SceneMgr() :
	mNextScene(Scene_None) //次のシーン管理変数
{
	mScene = (BaseScene*) new Select(this);
}

//初期化
void SceneMgr::Initialize() {
	mScene->Initialize();
}

//終了処理
void SceneMgr::Finalize() {
	mScene->Finalize();
}

//更新
void SceneMgr::Update() {
	if (mNextScene != Scene_None) {    //次のシーンがセットされていたら
		mScene->Finalize();//現在のシーンの終了処理を実行
		delete mScene;
		switch (mNextScene) {       //シーンによって処理を分岐
		case Scene_Select:        //次の画面がメニューなら
			mScene = (BaseScene*) new Select(this);   //メニュー画面のインスタンスを生成する
			break;//以下略
		
		}
		mNextScene = Scene_None;    //次のシーン情報をクリア
		mScene->Initialize();    //シーンを初期化
	}

	mScene->Update(); //シーンの更新
}

//描画
void SceneMgr::Draw() {
	mScene->Draw(); //シーンの描画
}

// 引数 nextScene にシーンを変更する
void SceneMgr::ChangeScene(SceneList NextScene) {
	mNextScene = NextScene;    //次のシーンをセットする
}

SceneMgr& SceneMgr::GetInstance() {
	static SceneMgr instance;
	return instance;
}
