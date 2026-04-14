#pragma once
class Base {
public:
	virtual ~Base() {}
	virtual void Initialize() {}
	virtual void Finalize() {}
	virtual void Update() = 0;
	virtual void Draw() = 0;
};
