#include "stdafx.h"
#include "Application.h"
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
		tmp.save(U"config.ini");
		System::MessageBoxOK(FileSystem::CurrentDirectory() + U"\n内にアプリケーション設定ファイル`config.ini`が見つかりませんでした。\nデフォルト値で作成しました");
		return;
	}
	else {
		//window設定.
		UI::WindowSize = { Parse<int32>(appini[U"Window.width"]) , Parse<int32>(appini[U"Window.height"]) };
	}
}
void App::AppInit() {
	LoadAppInfo();//情報読み込み.
	Window::SetTitle(U"Game Launcher");//タイトル設定.
	Window::Resize(UI::WindowSize);//ウィンドウサイズ設定.
	Window::SetFullscreen(false);//フルスクリーンにするかどうか.
	Window::SetStyle(UI::Frameless ? WindowStyle::Frameless : WindowStyle::Fixed);//ウィンドウフレーム表示するかどうか.
	Scene::SetBackground(back);
	
	UI::WindowSize = Scene::Size();
}
