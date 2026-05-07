#pragma once
#define SCALE (1)//後でここに実行時の画面サイズ/ 開発環境の画面サイズしたものを入れて比率調整をする.
enum Direction {
	up,
	down,
	left,
	right,
	none
};

//操作の定義.
enum Action {
	Up,
	Down,
	Left,
	Right,
	Decide,
	Sort,
};
