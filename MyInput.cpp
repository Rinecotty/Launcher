#include "stdafx.h"
#include "MyInput.h"
MyInput::MyInput() {
	InputInit();
}
void MyInput::InputInit() {
	keyMap[Action::Up] = { KeyUp, KeyW };
	keyMap[Action::Down] = { KeyDown, KeyS };
	keyMap[Action::Left] = { KeyLeft, KeyA };
	keyMap[Action::Right] = { KeyRight, KeyD };
	keyMap[Action::Decide] = { KeyZ, KeyEnter };
	keyMap[Action::Sort] = { KeyX, KeySpace };

	padMap[Action::Up] = { XInput(0).buttonUp };
	padMap[Action::Down] = { XInput(0).buttonDown };
	padMap[Action::Left] = { XInput(0).buttonLeft };
	padMap[Action::Right] = { XInput(0).buttonRight };
}
void MyInput::Update() {
	auto controller = XInput(0);
	if (controller.buttonA.down()) {
		Print << U"Button A is pressed.";
	}
	if (controller.buttonB.down()) {
		Print << U"Button B is pressed.";
	}
	if (controller.buttonX.down()) {
		Print << U"Button X is pressed.";
	}
	if (controller.buttonY.down()) {
		Print << U"Button Y is pressed.";
	}
	if (controller.buttonMenu.down()) {
		Print << U"Button Menu is pressed.";
	}
}

bool MyInput::IsPressed(Action type) {

	//キーが押された瞬間かどうか.
	for (const auto& key : keyMap[type]) {
		if (key.down()) {
			return true;
		}
	}
	bool current = false;//パッドが現在傾いているかどうか.
	bool padPressed = false;//パッドが傾いた瞬間かどうか.
	bool btPressed = false;//パッドのボタンが押された瞬間かどうか.
	//ゲームパッドの入力を確認、(XInputの仕様で最大４台).
	for (int i = 0; i < 4; i++) {
		const auto& pad = XInput(i);
		if (!pad.isConnected()) continue;
		double padX = pad.leftThumbX;
		double padY = pad.leftThumbY;
		double threshold = 0.5;//傾きの閾値、これ以上傾いてたら入力とみなす.

		// 小さい入力は無視
		if (Abs(padX) < threshold) padX = 0;
		if (Abs(padY) < threshold) padY = 0;

		// どっちの軸を優先するか
		bool horizontalPriority = Abs(padX) > Abs(padY);
		switch (type) {
		case Action::Up:
			if (pad.buttonUp.down()) {
				btPressed = true;
			}
			current = !horizontalPriority && (padY > threshold);//上に傾いているかどうか.
			padPressed = current && !padState[type];//現在傾いていて、前フレームでは傾いていなかったら押された瞬間とみなす.
			padState[type] = current;//状態を更新.
			if (btPressed || padPressed) return true;
			break;
		case Action::Down:
			if (pad.buttonDown.down()) {
				btPressed = true;
			}
			current = !horizontalPriority && (padY < -threshold);//下に傾いているかどうか.
			padPressed = current && !padState[type];//現在傾いていて、前フレームでは傾いていなかったら押された瞬間とみなす.
			padState[type] = current;//状態を更新.
			if (btPressed || padPressed) return true;
			break;
		case Action::Left:
			if (pad.buttonLeft.down()) {
				btPressed = true;
			}
			current = horizontalPriority && (padX < -threshold);//左に傾いているかどうか.
			padPressed = current && !padState[type];//現在傾いていて、前フレームでは傾いていなかったら押された瞬間とみなす.
			padState[type] = current;//状態を更新.
			if (btPressed || padPressed) return true;
			break;
		case Action::Right:
			if (pad.buttonRight.down()) {
				btPressed = true;
			}
			current = horizontalPriority && (padX > threshold);//左に傾いているかどうか.
			padPressed = current && !padState[type];//現在傾いていて、前フレームでは傾いていなかったら押された瞬間とみなす.
			padState[type] = current;//状態を更新.
			if (btPressed || padPressed) return true;
			break;
		case Action::Decide:
			if (pad.buttonA.down()) {
				return true;
			}
			break;
		case Action::Sort:
			if (pad.buttonMenu.down()) {
				return true;
			}
			break;
		}
	}
	return false;
}
