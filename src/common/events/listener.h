// Copyright 2017 Austen Higgins-Cassidy (plasmarobo@gmail.com)
#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include <events/events.h>
#include <utility>
#include <vector>

class Listener {
 public:
  template<class T>
  bool OnEvent(std::function<void(std::shared_ptr<T>)> proc) {
    return OnEvent(T::ID(), [&, proc](IEventPtr data){
      auto ev = std::dynamic_pointer_cast<T>(data);
      if (ev) proc(ev);
    });
  }

  bool RemoveHandler(IEvent::id_t id) {
    if (_EventDispatcher.expired()) return false;
    auto em = _EventDispatcher.lock();
    auto ev = std::find_if(_Subscriptions.begin(),
                           _Subscriptions.end(),
                           [&id](const _EventHandler& obj){
                            return obj.first == id;
                           });
    if (ev == _Subscriptions.end()) return false;
    em->RemoveListener(ev.first, ev.second);
    _Subscriptions.erase(ev);
    return true;
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
    return true;
  }

 private:
  std::weak_ptr<IDispatcher> _EventDispatcher;
  std::vector<_EventHandler> _Subscriptions;
};

#endif // EVENT_LISTENER_H
