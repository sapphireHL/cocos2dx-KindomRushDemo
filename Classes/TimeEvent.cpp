#include "TimeEvent.h"

void TimeEvent::readEvents(std::string filename)
{
	events.reserve(100);
	std::string  s;
	std::ifstream config(filename);
	while (std::getline(config, s)) {
		struct event e;
		std::stringstream  ss(s);
		getline(ss, s, ',');
		e.time = std::stoi(s);
		getline(ss, s, ',');
		e.type = eventType(std::stoi(s));
		getline(ss, s, ',');
		e.monsterType = MonsterType(std::stoi(s));
		getline(ss, s, ',');
		e.path = std::stoi(s);
		getline(ss, s, ',');
		e.subpath = std::stoi(s);
		getline(ss, s, ',');
		e.waitTime = std::stoi(s);
		events.push_back(e);
	}
}

struct event TimeEvent::getCurrentEvent() {
	return events[curTime];
}

void TimeEvent::goToNextTime()
{
	if (curTime  < (int)events.size() - 1) {
		curTime++;
	}
}

bool TimeEvent::iaAllEventsOver()
{
	return curTime == events.size() - 1;
}

int TimeEvent::getAlertWaitTime()
{
	return events[curTime].waitTime;
}
