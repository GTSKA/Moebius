#pragma once
#ifndef _MOEBIUS_FRAMEMOVE_CONTROL_H_
#define	_MOEBIUS_FRAMEMOVE_CONTROL_H_

class IFrameMoveCtrl
{
public:
	virtual void OnFrameMove(float elpsedTime, float absTime) = 0;
};

#endif
