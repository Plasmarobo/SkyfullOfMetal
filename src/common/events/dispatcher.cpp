
#include <events/dispatcher.h>

bool Dispatcher::AddListener(IEvent::id_t id, EventDelegate proc) {
  auto i = _EventListeners.find(id);

  if (i == _EventListeners.end()) {
    _EventListeners[id] = EventDelegateList();
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

bool Dispatcher::RemoveListener(IEvent::id_t id, EventDelegate proc) {
  auto j = _EventListeners.find(id);
  if (j == _EventListeners.end()) return false;

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

void Dispatcher::DispatchEvents() {
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

void Dispatcher::SendEvent(IEventPtr event) {
  _EventQueue.push_back(event);
}

size_t Dispatcher::QueueSize() {
  return _EventQueue.size();
}
