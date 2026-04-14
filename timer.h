#pragma once
#pragma once
class MyTimer {
private:
	uint64 startTime = 0;//タイマー開始時間.
	uint64 passTime = 0;//経過時間.
public:
	MyTimer()
		: startTime(0)
		, passTime(0)
	{

	}
	~MyTimer() {}
	void Init();//初期化.
	void Start();//タイマー開始.
	double GetProgressRate(double goal);//引数の数字を目標とした経過率を取得0.0～1.0.
	uint64 GetStartTime() { return startTime; }//開始時間を取得.
	uint64 GetPassTime();//経過時間を取得.
};
