#pragma once
typedef enum {
	Scene_Select,

	Scene_None
}SceneList;

//シーンを変更するインターフェイスクラス.
class SceneChanger {
public:
	virtual ~SceneChanger() = 0;
	virtual void ChangeScene(SceneList NextScene) = 0;
};
