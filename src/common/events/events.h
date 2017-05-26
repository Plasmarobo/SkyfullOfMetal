#ifndef EVENTS_COMMON_H
#define EVENTS_COMMON_H

class IEvent {
 public:
  typedef size_t id_t;
  virtual id_t GetID() = 0;
};

typedef std::shared_ptr<IEvent> EventPtr;
typedef std::function<void(IEvent&)> EventDelegate;

class IDispatcher {
 public:
  virtual bool AddListener(IEvent::id_t id, EventDelegate *proc);
  virtual bool RemoveListener(IEvent::id_t id, EventDelegate *proc);
  virtual void DispatchEvents() = 0;
  virtual void SendEvent(EventPtr event) = 0;
};

#define DECLARE_EVENT(type) \
  static IEvent::id_t ID() { \
    return reinterpret_cast<IEvent::id_t>(&ID); \
  } \
  IEvent::id_t GetID() override { \
    return ID(); \
  }\

#endif /* EVENTS_COMMON_H */