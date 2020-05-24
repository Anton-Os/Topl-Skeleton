#ifndef PHYSICS_H

#include <vector>

#include <Eigen/Dense>

#include "Timer.hpp"


class Physics_Move {
public:
    Physics_Move(){}
    //virtual void updatePos();
    Eigen::Vector3f getPos(){ return mPos; }
protected:
    Eigen::Vector3f mPos = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
};

typedef Eigen::Vector3f (*updatePos_abs)(const Eigen::Vector3f& pos);

class Physics_MoveAbs : Physics_Move {
public:
    Physics_MoveAbs(updatePos_abs updateFunc) : Physics_Move() {
        mAbsMove = updateFunc;
    }
	void updatePos(unsigned reps);
private:
    updatePos_abs mAbsMove;
	unsigned mActionRepeats; // Negative moves in other direction
};

typedef Eigen::Vector3f (*updatePos_timed)(const Eigen::Vector3f& pos, const Timer_EventRatio& timeEvent);

class Physics_MoveTimed : Physics_Move {
public:
    Physics_MoveTimed(updatePos_timed updateFunc) : Physics_Move() {
        mTimedMove = updateFunc;
    }
	void updatePos(const Timer_EventRatio& timeEvent);
private:
    updatePos_timed mTimedMove;
	Timer_EventRatio mActionTimes;
};

#define PHYSICS_H
#endif