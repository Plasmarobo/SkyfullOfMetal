// Copyright 2017 Austen Higgins-Cassidy (plasmarobo@gmail.com)
#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include <utility>
#include <vector>
#include <events/events.h>

class Listener {
 public:
  template<class T>
  bool OnEvent(std::function<void(std::shared_ptr<T>)> proc) {
    return OnEvent(T::ID(), [&, proc](IEventPtr data){
      auto ev = std::dynamic_pointer_cast<T>(data);
      if (ev) proc(ev);
    });
  }

 protected:
  typedef std::pair<IEvent::id_t, EventDelegate> _EventHandler;
  explicit Listener(std::weak_ptr<IDispatcher> dispatch):
                    _EventDispatcher(dispatch) {
  }

  virtual ~Listener() {
    if (_EventDispatcher.expired()) return;
    auto em = _EventDispatcher.lock();
    for (auto i : _Subscriptions) {
      em->RemoveListener(i.first, i.second);
    }
  }

  bool OnEvent(IEvent::id_t id, EventDelegate proc) {
    if (_EventDispatcher.expired()) return false;
    auto em = _EventDispatcher.lock();
    if (em->AddListener(id, proc)) {
      _Subscriptions.push_back(_EventHandler(id, proc));
    }
  }

 private:
  std::weak_ptr<IDispatcher> _EventDispatcher;
  std::vector<_EventHandler> _Subscriptions;
};

#endif // EVENT_LISTENER_H