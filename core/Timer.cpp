#include "Timer.h"

unsigned Topl_Countup::isTrigger(double milSecsPass) { // Sees if over milliseconds some event triggered
    mTimeBuff += milSecsPass;

    if(mTimeBuff < mElapseTime) return 0; // Nothing triggered, not enough time passed

    unsigned triggerCount = 0;
    while(mTimeBuff >= mElapseTime){
        mTimeBuff -= mElapseTime;
        triggerCount++;
    }

    mEventCount += triggerCount; // Stores total number of occurances
    return triggerCount; // Number of times 
}

using namespace std::chrono;

double Topl_Timer::getSecsPassed(){
    mEndSec = high_resolution_clock::now(); // Gets current time
    mSecTimeSpan = duration_cast<duration<double>>(mEndSec - mStartSec); // Computes difference
    mStartSec = mEndSec; // Sets the new start time
    return mSecTimeSpan.count();
}