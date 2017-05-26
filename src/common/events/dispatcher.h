#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include <list>
#include <map>
#include <events/events.h>

class Dispatcher : public IDispatcher {
 public:
  typedef std::list<EventDelegate*> EventDelegateList;

  ~Dispatcher() {
  }

  bool AddListener(IEvent::id_t id, EventDelegate *proc) override;

  bool RemoveListener(IEvent::id_t id, EventDelegate *proc) override;

  void DispatchEvents() override;

  void SendEvent(EventPtr event) override;

 private:
  std::list<std::shared_ptr<IEvent>> _EventQueue;
  std::map<IEvent::id_t, EventDelegateList> _EventListeners;
};

#endif /* EVENT_DISPATCHER_H */