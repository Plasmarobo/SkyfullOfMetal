#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include <events/events.h>
#include <list>
#include <map>


class Dispatcher : public IDispatcher {
 public:
  typedef std::list<EventDelegate> EventDelegateList;

  ~Dispatcher() {
  }

  bool AddListener(IEvent::id_t id, EventDelegate proc) override;

  bool RemoveListener(IEvent::id_t id, EventDelegate proc) override;

  void DispatchEvents() override;

  void SendEvent(IEventPtr event) override;

  size_t QueueSize();

 private:
  std::list<std::shared_ptr<IEvent>> _EventQueue;
  std::map<IEvent::id_t, EventDelegateList> _EventListeners;
};

#endif /* EVENT_DISPATCHER_H */
