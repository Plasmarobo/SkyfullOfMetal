#include <catch.hpp>
#include <common/events/events.h>
#include <common/events/dispatcher.h>
#include <common/events/listener.h>

DECLARE_EVENT(EVENT_MOVE);
DECLARE_EVENT(EVENT_STOP);
DECLARE_EVENT(EVENT_PRODUCE);

SCENARIO("Dispatching Events", "[events]") {
  GIVEN("A dispatcher with no listeners") {

  }

  GIVEN("A dispatcher with listeners") {
    WHEN("A MOVE event is sent") {

    }

    WHEN("A STOP event is sent") {

    }

    WHEN("A PRODUCE event is sent") {

    }

    WHEN("A Listener is removed") {
      AND_WHEN("A MOVE event is sent") {

      }
    }
  }
}
