/// <summary>
/// タイムコール
/// </summary>
#pragma once
#include<functional>
#include<initializer_list>
class TimedCall {
public:
	TimedCall(std::function<void()> function, uint32_t time);
	~TimedCall();

	void Update();

	bool IsConpleted() const { return IsConplete_; }

private:
	//コールバック
	std::function<void()> function_;
	//残り時間
	uint32_t time_;
	//完了フラグ
	bool IsConplete_ = false;
};
