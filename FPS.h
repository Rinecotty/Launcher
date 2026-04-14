#pragma once
#pragma once
#include "timer.h"
#include "FontMgr.h"
class FPS {
private:
	MyTimer timer{};
	FontMgr& font = FontMgr::GetInstance();
	float fps;//今のFPS.
	int frameCount;//フレーム数カウント.
	long long pMicroSec;//経過時間.
	long long totalMicroSec;//合計経過時間.
	int limitFPS;//FPS制限値.
public:
	FPS(int targetfps)
		: fps(0)
		, limitFPS(targetfps)
		, pMicroSec(0)
		, frameCount(0)
		, totalMicroSec(0)
	{
	}
	~FPS() {}
	void StartFPS();//FPS制限のスタート.
	void NowFPS();
	void DrawFPS();
	bool LockFPS();
};
