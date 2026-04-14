#pragma once
//ゲーム情報入れる構造体.
//struct Game {
//	String title;							//ゲームのタイトル.
//	FilePath path;						//ゲーム実行ファイル.
//	bool isWebApp = false;		//Webブラウザで起動.
//	Texture texture;					//ゲームの画像.
//	String desc;							//ゲームの説明文.
//	String staff;						//ゲームの開発スタッフ.
//	String tools;						//ゲームの開発ツール.
//	bool useMouse = false;		//マウスを使用するか.
//	bool useKeyboard = false;	//キーボードを使用するか.
//	bool useGamepad = false;	//ゲームパッドを使用するか.
//	int32 priority = 0;				//ランチャー表示優先度.
//};

namespace UI {
	inline Size WindowSize{ 1920,1200 };//ウィンドウサイズ.
	constexpr bool Frameless = false;//ウィンドウのフレーム表示するか.
}

//アプリケーション設定用のクラス.
class App {
private:
	ColorF back{ 0.13, 0.2, 0.46 };//背景色.
	App(){}
	~App(){}
public:
	static App& GetInstance();
	void LoadAppInfo();
	void AppInit();//設定ファイルを読み込んでウィンドウサイズ等アプリケーションセットアップ
};
