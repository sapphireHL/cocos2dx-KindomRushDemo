#pragma once
#include "Monster.h"
#include <fstream>
#include <string>
#include <sstream>

enum eventType
{
	eventAddMonster,
	eventWaveAlert
};

struct event {
	int time;
	eventType type;
	MonsterType monsterType;
	int path, subpath;
	int waitTime;
	event() : time(0), type(eventWaveAlert), monsterType(MonsterType::gnoll_reaver), path(-1), subpath(-1), waitTime(0){};
};

class TimeEvent {
public:
	TimeEvent() :curTime(0) {};
	void readEvents(std::string filename);
	struct event getCurrentEvent();
	void goToNextTime();
	bool iaAllEventsOver();
	int getAlertWaitTime();

private:
	int curTime;
	std::vector<struct event> events;
};