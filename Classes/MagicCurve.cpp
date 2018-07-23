#include "MagicCurve.h"

static inline Vec2 pointAt(float t, const MagicConfig & c) {
	Vec2 centerPosition = c.endPostion / 2;
	float radius = centerPosition.distance(Vec2(0, 0));
	float angle = centerPosition.getAngle();
	float x, y;
	if (c.upside) {
		x = -radius * cos(3.1415926 * t * 2 + angle) + centerPosition.x;
		y = -radius * sin(3.1415926 * t * 2 + angle) + centerPosition.y;
	}
	else {
		x = -radius * cos(3.1415926 * (c.period - t) * 2 + angle) + centerPosition.x;
		y = -radius * sin(3.1415926 * (c.period - t) * 2 + angle) + centerPosition.y;
	}
	return Vec2(x, y);
}

MagicCurve * MagicCurve::create(float t, const MagicConfig & c)
{
	MagicCurve *curve = new  MagicCurve();
	if (curve && curve->initWithDuration(t, c))
	{
		curve->autorelease();
		return curve;
	}

	delete curve;
	curve = nullptr;
	return nullptr;
}

MagicCurve * MagicCurve::clone() const
{
	return MagicCurve::create(_duration, _config);
}

MagicCurve * MagicCurve::reverse(void) const
{
	MagicConfig r;

	r.endPostion = _startPosition;
	r.heightRate = _config.heightRate;

	MagicCurve *action = MagicCurve::create(_duration, r);
	return action;
}

void MagicCurve::startWithTarget(Node * target)
{
	ActionInterval::startWithTarget(target);
	_previousPosition = _startPosition = target->getPosition();
}

void MagicCurve::update(float time)
{
	if (_target) {
		Vec2 currentPos = _target->getPosition();
		Vec2 diff = currentPos - _previousPosition;
		_startPosition = _startPosition + diff;

		Vec2 newPos = _startPosition + pointAt(time / 2, _config);
		_target->setPosition(newPos);

		_previousPosition = newPos;
	}
	
	//_target->setPosition(_startPosition + pointAt(time / 2, _config));
}

bool MagicCurve::initWithDuration(float t, const MagicConfig & c)
{
	if (ActionInterval::initWithDuration(t))
	{
		_config = c;
		return true;
	}

	return false;
}
