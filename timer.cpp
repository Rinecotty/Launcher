#include "stdafx.h"
#include "timer.h"

void MyTimer::Init() {
	startTime = 0;
}
void MyTimer::Reset() {
	startTime = Time::GetMicrosec();
}
void MyTimer::Start() {
	startTime = Time::GetMicrosec();
}

uint64 MyTimer::GetPassTime() {
	passTime =  Time::GetMicrosec() - startTime;
	return passTime;
}
double MyTimer::GetProgressRate(double goal) {
	double rate;
	double pass = GetPassTime();
	rate = pass / goal;
	return rate;
}
