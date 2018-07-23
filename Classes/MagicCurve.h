#pragma once
#include "cocos2d.h"

using namespace cocos2d;

struct MagicConfig
{
	Vec2 endPostion;
	float heightRate;//œ‡∂‘”⁄‘≤
	bool upside;
	float period;
};


class MagicCurve : public ActionInterval
{
public:

	static MagicCurve* create(float t, const MagicConfig& c);

	//
	// Overrides
	//
	virtual MagicCurve* clone() const override;
	virtual MagicCurve* reverse(void) const override;
	virtual void startWithTarget(Node *target) override;
	/**
	* @param time In seconds.
	*/
	virtual void update(float time) override;

public:
	MagicCurve() {}
	virtual ~MagicCurve() {}

	/**
	* initializes the action with a duration and a bezier configuration
	* @param t in seconds
	*/
	bool initWithDuration(float t, const MagicConfig& c);

protected:
	MagicConfig _config;
	Vec2 _startPosition;
	Vec2 _previousPosition;

};