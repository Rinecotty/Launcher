#pragma once
#include "timer.h"
class MyInput
{
private:
	int inputCnt = 0;//前フレームとの比較用.
	MyTimer inputTimer;//入力の間隔を空けるためのタイマー.
	MyInput();
	~MyInput(){}
	HashTable<Action, Array<Input>> keyMap;
	HashTable<Action, Array<Input>> padMap;
	HashTable<Action, bool> padState;//ゲームパッドが傾いた瞬間かどうか前フレームとの比較用.

public:
	void InputInit();
	static MyInput& GetInstance() {
		static MyInput instance;
		return instance;
	}
	bool IsPressed(Action type);//Actionの中からどのキーが押された瞬間かを返す関数.
	void Update();
};

