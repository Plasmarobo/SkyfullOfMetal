#include <catch.hpp>
#include <common/events/events.h>
#include <common/events/dispatcher.h>

#include <common/events/listener.h>

class MoveEvent {
 public:
  DECLARE_EVENT(EVENT_MOVE);
};

class StopEvent {
 public:
  DECLARE_EVENT(EVENT_STOP);
};

class ProduceEvent {
 public:
  DECLARE_EVENT(EVENT_PRODUCE);
};

class TestListener : public Listener {
 public:
  explicit TestListener(std::weak_ptr<IDispatcher> dispatch) :
    Listener(dispatch) {
    _moving = false;
    _stopped = false;
    _producing = false;
  }
  bool _moving;
  bool _stopped;
  bool _producing;
};

SCENARIO("Dispatching Events", "[Dispatcher]") {
  GIVEN("A dispatcher with no listeners") {
    Dispatcher dispatch();
    WHEN("A MOVE event is sent") {
      MoveEvent mv;
      dispatch.SendEvent(&mv);
      THEN("The event is added to queue") {
        REQUIRE(dispatch.QueueSize() == 1);
      }
      AND_WHEN("Events are dispatched") {
        dispatch.DispatchEvents();
        THEN("The event is removed from queue") {
          REQUIRE(dispatch.QueueSize() == 0);
        }
      }
    }
  }
}

SCENARIO("Listening for Events", "[Listener]") {
  Dispatcher dispatch;
  TestListener mslistener(&dispatch);
  mslistener.OnEvent<MoveEvent>([&](std::shared_ptr<MoveEvent> data){
    mslistener._moving = true;
  });
  mslistener.OnEvent<StopEvent>([&](std::shared_ptr<StopEvent> data){
    mslistener._stopped = true;
  });

  TestListener slistener(&dispatch);
  slistener.OnEvent<StopEvent>([&](std::shared_ptr<StopEvent> data){
    slistener._stopped = true;
  });

  TestListener plistener(&dispatch);
  plistener.OnEvent<ProduceEvent>([&](std::shared_ptr<ProduceEvent> data){
    plistener._producing = true;
  });

  GIVEN("A dispatcher with listeners") {
    WHEN("A MOVE event is sent") {
      MoveEvent mv;
      dispatch.SendEvent(&mv);
      dispatch.DispatchEvents();
      THEN("MOVE listeners should respond") {
        REQUIRE(mslistener._moving == true);
        REQUIRE(slistener._moving == false);
        REQUIRE(plistener._moving == false);
      }
    }

    WHEN("A STOP event is sent") {
      StopEvent s;
      dispatch.SendEvent(&sv);
      dispatch.DispatchEvents();
      THEN("STOP listeners should respond") {
        REQUIRE(mslistener._stopped == true);
        REQUIRE(slistener._stopped == true);
        REQUIRE(plistener._stopped == false);
      }
    }

    WHEN("A PRODUCE event is sent") {
      ProduceEvent p;
      dispatch.SendEvent(&p);
      dispatch.DispatchEvents();
      THEN("PRODUCE listeners should respond") {
        REQUIRE(mslistener._producing == false);
        REQUIRE(slistener._producing == false);
        REQUIRE(plistener._producing == true);
      }
    }

    WHEN("Multiple events arrive") {
      MoveEvent mv;
      StopEvent sv;
      ProduceEvent pv;
      dispatch.SendEvent(&mv);
      dispatch.SendEvent(&sv);
      dispatch.SendEvent(&pv);
      mslistener._moving = false;
      mslistener._stopped = false;
      mslistener._producing = false;
      slistener._moving = false;
      slistener._stopped = false;
      slistener._producing = false;
      plistener._moving = false;
      plistener._stopped = false;
      plistener._producing = false;
      dispatch.DispatchEvents();
      THEN("Listeners respond to all listened events") {
        REQUIRE(mslistener._moving == true);
        REQUIRE(mslistener._stopped == true);
        REQUIRE(mslistener._producing == false);
        REQUIRE(slistener._moving == false);
        REQUIRE(slistener._stopped == true);
        REQUIRE(slistener._producing == false);
        REQUIRE(plistener._moving == false);
        REQUIRE(plistener._stopped == false);
        REQUIRE(plistener._producing == true);
      }
    }

    WHEN("A Listener is removed") {
      slistener.RemoveHandler(StopEvent::ID());
      AND_WHEN("A Stop event is sent") {
        StopEvent ev;
        slistener._stopped = false;
        mslistener._stopped = false;
        dispatch.SendEvent(&sv);
        dispatch.DispatchEvents();
        THEN("The event does not reach the removed listener") {
          REQUIRE(slistener._stopped == false);
          REQUIRE(mslistener._stopped == true);
        }
      }
    }

  // TODO: Test race conditions for listener/dispatcher deletion
  }
}
