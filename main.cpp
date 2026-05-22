# include <Siv3D.hpp> // Siv3D v0.6.16
#include"Select.h"
#include"Application.h"
#include "SceneMgr.h"
#include "FPS.h"
#include "FontMgr.h"

void Main()
{
	App& app = App::GetInstance();
	FontMgr& fontMgr = FontMgr::GetInstance();
	Select select;
	app.AppInit();
	select.Initialize();
	while (System::Update()) {
		select.Update();
		select.Draw();
#if _DEBUG
		fontMgr.GetFont()(Profiler::FPS()).drawAt(Vec2{ 1800 * SCALE,1100 * SCALE }, ColorF{ 0.2 });
#endif
	}
}
