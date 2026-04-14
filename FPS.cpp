#include "stdafx.h"
#include "FPS.h"

void FPS::StartFPS() {
	timer.Start();
}
void FPS::NowFPS() {
	pMicroSec = timer.GetPassTime();

	if (pMicroSec <= 0) return;

	frameCount++;
	totalMicroSec += pMicroSec;

	//FPS更新.
	fps = frameCount * 1000000.0f / (float)totalMicroSec;
	frameCount = 0;
	totalMicroSec = 0;
}
bool FPS::LockFPS() {
	NowFPS();
	if (pMicroSec < 1000000 / limitFPS) {
		return false;
	}
	else {
		StartFPS();
		return true;
	}
}
void FPS::DrawFPS() {
	font.GetFont()(fps).drawAt(Vec2{500,500},ColorF{0.2});
}
