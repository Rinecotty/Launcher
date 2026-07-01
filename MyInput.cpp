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
	}
	if (controller.buttonB.down()) {
	}
	if (controller.buttonX.down()) {
	}
	if (controller.buttonY.down()) {
	}
	if (controller.buttonMenu.down()) {
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
		const auto& pad = Gamepad(i);
		if (!pad.isConnected()) continue;
		double padX = pad.axes[0];
		double padY = pad.axes[1];
		double threshold = 0.5;//傾きの閾値、これ以上傾いてたら入力とみなす.

		// 小さい入力は無視
		if (Abs(padX) < threshold) padX = 0;
		if (Abs(padY) < threshold) padY = 0;

		// どっちの軸を優先するか
		bool horizontalPriority = Abs(padX) > Abs(padY);
		switch (type) {
		case Action::Up:
			if (pad.buttons[0].down()) {
				btPressed = true;
			}
			current = !horizontalPriority && (padY > -threshold);//上に傾いているかどうか.
			padPressed = current && !padState[type];//現在傾いていて、前フレームでは傾いていなかったら押された瞬間とみなす.
			padState[type] = current;//状態を更新.
			if (btPressed || padPressed) return true;
			break;
		case Action::Down:
			if (pad.buttons[1].down()) {
				btPressed = true;
			}
			current = !horizontalPriority && (padY < threshold);//下に傾いているかどうか.
			padPressed = current && !padState[type];//現在傾いていて、前フレームでは傾いていなかったら押された瞬間とみなす.
			padState[type] = current;//状態を更新.
			if (btPressed || padPressed) return true;
			break;
		case Action::Left:
			if (pad.buttons[2].down()) {
				btPressed = true;
			}
			current = horizontalPriority && (padX < -threshold);//左に傾いているかどうか.
			padPressed = current && !padState[type];//現在傾いていて、前フレームでは傾いていなかったら押された瞬間とみなす.
			padState[type] = current;//状態を更新.
			if (btPressed || padPressed) return true;
			break;
		case Action::Right:
			if (pad.buttons[3].down()) {
				btPressed = true;
			}
			current = horizontalPriority && (padX > threshold);//左に傾いているかどうか.
			padPressed = current && !padState[type];//現在傾いていて、前フレームでは傾いていなかったら押された瞬間とみなす.
			padState[type] = current;//状態を更新.
			if (btPressed || padPressed) return true;
			break;
		case Action::Decide:
			if (
				pad.buttons[9].down()) {
				return true;
			}
			break;
		case Action::Sort:
			if (pad.buttons[5].down()) {
				return true;
			}
			break;
		}
	}
	return false;
}

bool MyInput::IsAnyPadInput() {
	for (int i = 0; i < 4; i++) {
		const auto& pad = Gamepad(i);
		if (!pad.isConnected()) continue;
		const auto& info = pad.getInfo();
		/*if (Indexed(pad.buttons)
			|| pad.buttonB.down() || pad.buttonX.down() || pad.buttonY.down() || pad.buttonMenu.down()
			|| pad.buttonLB.down() || pad.buttonRB.down()
			|| pad.buttonY.down()
			) {
			return true;
		}*/
		for (auto&& [i, button] : Indexed(pad.buttons))
		{
			if (button.pressed()) {
				return true;
			}
		}

		double padX = pad.axes[0];
		double padY = pad.axes[1];
		double threshold = 0.5;//傾きの閾値、これ以上傾いてたら入力とみなす.
		// 小さい入力は無視
		if (Abs(padX) < threshold) padX = 0;
		if (Abs(padY) < threshold) padY = 0;
		if (padX != 0 || padY != 0) {
			return true;
		}
	}
	return false;
}
