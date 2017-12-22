#include "Manager.h"

static EventManager sEvtMgr;
static HistoryManager sHistoryMgr;

EventManager::EventManager() {
	memset(mListeners, 0, sizeof(mListeners));
}
void EventManager::addEventListener(EventType et, EventListener *listener) {
	if (listener == NULL) return;
	if (sEvtMgr.mListeners[et] == NULL) {
		sEvtMgr.mListeners[et] = new std::vector<EventListener*>();
	}
	sEvtMgr.mListeners[et]->push_back(listener);
}
void EventManager::removeEventListener(EventType et, EventListener *listener) {
	if (listener == NULL) return;
	std::vector<EventListener*> *items = sEvtMgr.mListeners[et];
	if (items == NULL) return;
	for (int i = 0; i < items->size(); ++i) {
		if (items->at(i) == listener) {
			items->erase(items->begin() + i);
			break;
		}
	}
}
void EventManager::sendEvent(EventType et, void *evtSrc, void *newVal, void *oldVal) {
	std::vector<EventListener*> *items = sEvtMgr.mListeners[et];
	if (items == NULL) return;
	for (int i = 0; i < items->size(); ++i) {
		EventListener *listener = items->at(i);
		listener->onEvent(et, evtSrc, newVal, oldVal);
	}
}

void HistoryManager::execDo(XAction *action) {
	if (action == NULL) return;
	sHistoryMgr.mActions.push_back(action);
	action->onDo();
}
void HistoryManager::execUndo(XAction *action) {
	if (sHistoryMgr.mActions.size() == 0) return;
	XAction *ac = sHistoryMgr.mActions.at(sHistoryMgr.mActions.size() - 1);
	sHistoryMgr.mActions.erase(sHistoryMgr.mActions.begin() + sHistoryMgr.mActions.size() - 1);
	ac->onUndo();
}
