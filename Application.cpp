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
		tmp.write(U"Window", U"Fullscreen", true);
		tmp.write(U"Window", U"Frameless", false);
		tmp.save(U"config.ini");
		System::MessageBoxOK(FileSystem::CurrentDirectory() + U"\n内にアプリケーション設定ファイル`config.ini`が見つかりませんでした。\nデフォルト値で作成しました");
		return;
	}
	else {
		//window設定.
		UI::WindowSize = { Parse<int32>(appini[U"Window.width"]) , Parse<int32>(appini[U"Window.height"]) };
		//フルスクリーン表示するかどうか.
		//UI::FullScreen = { Parse<bool>(appini[U"Window.Fullscreen"]) };
		//フレームを表示するかどうか.
		//UI::Frameless = {Parse<bool>(appini[U"Window.Frameless"])};
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
