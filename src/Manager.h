#pragma once
#include <vector>

enum EventType {
	EVENT_TYPE_OPT_TOOL_FONT_NAME_CHANGE,
	EVENT_TYPE_OPT_TOOL_FONT_SIZE_CHANGE,

	EVENT_TYPE_CELL_SELECT_RANGE_CHANGE,

	EVENT_TYPE_NUM
};

class EventListener {
public:
	virtual void onEvent(EventType et, void *evtSrc, void *newVal, void *oldVal) = 0;
	virtual ~EventListener() {}
};

class EventManager {
public:
	EventManager();
	static void addEventListener(EventType et, EventListener *listener);
	static void removeEventListener(EventType et, EventListener *listener);
	static void sendEvent(EventType et, void *evtSrc, void *newVal, void *oldVal);
protected:
	std::vector<EventListener*> *mListeners[EVENT_TYPE_NUM];
};

class Action {
public:
	virtual void onDo() = 0;
	virtual void onUndo() = 0;
	virtual ~Action() {};
};

class HistoryManager {
public:
	static void execDo(Action *action);
	static void execUndo(Action *action);
protected:
	std::vector<Action*> mActions;
};

