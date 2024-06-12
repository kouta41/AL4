#include"TimedCall.h"

TimedCall::TimedCall(std::function<void()> function, uint32_t time) {
	time_ = time;
	function_ = function;
}

TimedCall::~TimedCall() {}

void TimedCall::Update() {
	if (IsConplete_ == true) {
		return;
	}
	if (time_-- <= 0) {
		IsConplete_ = true;
		function_();
	}
}