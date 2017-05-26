#include <events/dispatcher.h>

bool AddListener(IEvent::id_t id, EventDelegate *proc) {
  auto i = _EventListeners.find(id);
  if (i == _EventListeners.end()) {
    _EventListeners[id] = std::list<EventDelegate>();
  }
  auto &list = _EventListeners[id];
  for (auto i = list.begin(); i != list.end(); ++i) {
    EventDelegate &func = *i;
    if (func.target<EventDelegate>() == proc.target<EventDelegate>()) {
      return false;
    }
  }
  list.push_back(proc);
}

bool RemoveListener(IEvent::id_t id, EventDelegate *proc) {
  auto j = _EventListeners.find(id);
  if (j == _EventListeners.end()) {
    return false;
  }
  auto &list = _EventListeners[id];
  for (auto i = list.begin(); i != list.end(); ++i) {
    EventDelegate &func = *i;
    if (func.target<EventDelegate>() == proc.target<EventDelegate>()) {
      list.erase(i);
      return true;
    }
  }
  return false;
}

void DispatchEvents() {
  size_t count = _EventQueue.size();
  for (auto it = _EventQueue.begin(); it != _EventQueue.end(); ++it) {
    if (!count) break;
    auto &i = *it;
    auto liseners = _EventListeners.find(i->GetID());
    if (liseners != _EventListeners.end()) {
      for (auto l : liseners->second) {
        l(i);
      }
    }
    it = _EventQueue.erase(it);
    count--;
  }
}

void SendEvent(EventPtr event) {
  _EventQueue.push_back(event);
}
