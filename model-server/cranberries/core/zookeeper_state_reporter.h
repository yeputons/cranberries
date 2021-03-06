#ifndef CRANBERRIES_ZOOKEEPER_STATE_REPORTER_H_
#define CRANBERRIES_ZOOKEEPER_STATE_REPORTER_H_

#include "tensorflow_serving/core/servable_state.h"
#include "tensorflow_serving/util/event_bus.h"
#include "zookeeper_cc/zookeeper_cc.h"

namespace tensorflow {
namespace serving {
namespace cranberries {

// Processes events coming from EventBus<ServableState> about
// current state of servables and saves that information info
// Zookeeper.
//
// TODO(egor.suvorov): make it non-blocking, because EventBus<>'s
// subscriber is assumed to be non-blocking.
//
// TODO(egor.suvorov): make it handle Zookeeper disconnections
// gracefully, right now it assumes connection is always up and
// it's enough to react to changes only.
//
// TODO(egor.suvorov): make it handle ephemeral znodes remaining from crashed
// instance (they are not recreated and removed).
class ZookeeperStateReporter {
 public:
  ZookeeperStateReporter(zookeeper_cc::Zookeeper *zookeeper)
    : zookeeper_(zookeeper) {}
  ~ZookeeperStateReporter() {}

  EventBus<ServableState>::Callback GetEventBusCallback();

 private:
  void ProcessEvent(const EventBus<ServableState>::EventAndTime &ev);

  zookeeper_cc::Zookeeper *zookeeper_;
};

}  // namespace cranberries
}  // namespace serving
}  // namespace tensorflow

#endif  // CRANBERRIES_ZOOKEEPER_STATE_REPORTER_H_
