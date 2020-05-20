#include "Timer.hpp"

#include <ratio>

unsigned Timer_Event::isTrigger(double milSecsPass) { // Sees if over milliseconds some event triggered
    mEventCount += mTimeBuff / mElapseTime; // This gives us how many times event has gone off overall
    
    mTimeBuff += milSecsPass; // Update the time buffer
    if(mTimeBuff < mElapseTime) return 0; // Nothing triggered, not enough time passed

    unsigned triggerCount = 0;
    while(mTimeBuff >= mElapseTime){
        mTimeBuff -= mElapseTime;
        triggerCount++;
    }
    
    return triggerCount; // Number of times 
}

using namespace std::chrono;

void Timer_Interval::reset(){
    mRelTimeSpan = milliseconds(0);
    mAbsTimeSpan = milliseconds(0);
    mStartSec = steady_clock::now();
}

double Timer_Interval::getRelMillsecs(){
    updateTimer();
    return mRelTimeSpan.count();
}

double Timer_Interval::getAbsMillsecs(){
    updateTimer();
    return mAbsTimeSpan.count();
}

void Timer_Interval::updateTimer(){
    mEndSec = steady_clock::now(); // Gets current time
    mRelTimeSpan = duration_cast<duration<double>>(mEndSec - mStartSec);
    mAbsTimeSpan += mRelTimeSpan;
    mStartSec = mEndSec;
}