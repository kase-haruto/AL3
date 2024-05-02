#include "TimedCall.h"

TimedCall::TimedCall(std::function<void()> func, uint32_t time)
	:func_(func), time_(time),isFinished_(false){

}

void TimedCall::Update(){
	if (isFinished_){
		return;
	}
	time_--;
	if (time_ <= 0){
		isFinished_ = true;
		func_();
	}
}