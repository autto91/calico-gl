#include "event.h"
#include <utility>

template <typename Event>
void calico::dispatcher::pool_handler<Event>::publish() {
  const auto len = events_.size();

  for (std::size_t pos{}; pos < len; pos++) {
    signal_.publish(events_[pos]);
    if (events_[pos].is_handled) {
      events_.erase(events_.cbegin(), events_.cbegin() + 1);
    }
  }
}

template <typename Event>
void calico::dispatcher::pool_handler<Event>::disconnect(void *instance) {
  bucket().disconnect(instance);
}

template <typename Event>
void calico::dispatcher::pool_handler<Event>::clear() {
  events_.clear();
}

template <typename Event>
typename calico::dispatcher::pool_handler<Event>::sink_type
calico::dispatcher::pool_handler<Event>::bucket() {
  return signal_type{signal_};
}

template <typename Event>
template <typename... Args>
void calico::dispatcher::pool_handler<Event>::trigger(Args &&...args) {
  Event instance{std::forward<Args>(args)...};

  signal_.publish(instance);
}

template <typename Event>
template <typename... Args>
void calico::dispatcher::pool_handler<Event>::enqueue(Args &&...args) {
  if constexpr (std::is_aggregate_v<Event>) {
    events_.push_back(Event{std::forward<Args>(args)...});
  } else {
    events_.push_back(std::forward<Args>(args)...);
  }
}

template <typename Event>
calico::dispatcher::pool_handler<Event> &calico::dispatcher::assure() {
  const auto index = std::type_index<Event>::value();
}
