#include "stdafx.h"
#include "Application.h"
App::App()
	: isFullScreen(true)
{ }
App& App::GetInstance() {
	static App instance;
	return instance;
}
void App::LoadAppInfo() {
	INI appini{ U"config.ini" };
	if (!appini) {
		//ファイルがなければデフォルトで作成.
		INI tmp;
		tmp.write(U"Window", U"width", 1920);
		tmp.write(U"Window", U"height", 1200);
		tmp.write(U"Window", U"Fullscreen", 1);//0がfalseで1がtrue.
		tmp.write(U"Window", U"Fra1eless", 0);
		tmp.write(U"Config", U"AppLeaveTime",30);
		tmp.write(U"Config", U"AudioLeaveTime",30);
		tmp.save(U"config.ini");
		System::MessageBoxOK(FileSystem::CurrentDirectory() + U"\n内にアプリケーション設定ファイル`config.ini`が見つかりませんでした。\nデフォルト値で作成しました");
		LoadAppInfo();//もう一度読み込む.
	}
	else {
		//window設定.
		UI::WindowSize = { Parse<int32>(appini[U"Window.width"]) , Parse<int32>(appini[U"Window.height"]) };
		//フルスクリーン表示するかどうか.
		UI::FullScreen = appini.get<int32>(U"Window.Fullscreen") != 0;//0以外ならtrue.
		//フレームを表示するかどうか.
		UI::Frameless = appini.get<int32>(U"Window.Frameless") != 0;
		UI::AppLeaveTime = Parse<int32>(appini[U"Config.AppLeaveTime"]);
		UI::AudioLeaveTime = Parse<int32>(appini[U"Config.AudioLeaveTime"]);
		SCALE = Min(UI::WindowSize.x / (double)1920  , UI::WindowSize.y/ (double)1200);//SCALEをウィンドウサイズに合わせて調整.	
	}
}
void App::AppInit() {
	LoadAppInfo();//情報読み込み.
	Window::SetTitle(U"Game Launcher");//タイトル設定.
	Window::Resize(UI::WindowSize);//ウィンドウサイズ設定.
	Window::SetStyle(UI::Frameless ? WindowStyle::Frameless : WindowStyle::Fixed);//ウィンドウフレーム表示するかどうか.
	Window::SetFullscreen(UI::FullScreen);//フルスクリーンにするかどうか.
	Scene::SetBackground(back);
	
	UI::WindowSize = Scene::Size();
}
