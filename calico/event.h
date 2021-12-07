#pragma once

#include <SDL2/SDL.h>
#include <entt/signal/sigh.hpp>
#include <type_traits>
#include <vector>

namespace calico {

struct calico_event {
  bool is_handled;
};

struct global_window_event : calico_event {
  SDL_Event sdl_event;
};

/**
 * @brief Basic dispatcher implementation.
 *
 * A dispatcher can be used either to trigger an immediate event or to enqueue
 * events to be published all together once per tick.<br/>
 * Listeners are provided in the form of member functions. For each event of
 * type `Event`, listeners are such that they can be invoked with an argument of
 * type `Event &`, no matter what the return type is.
 *
 * The dispatcher creates instances of the `sigh` class internally. Refer to the
 * documentation of the latter for more details.
 */
class dispatcher {
  struct basic_pool {
    virtual ~basic_pool() = default;
    virtual void publish() = 0;
    virtual void disconnect(void *) = 0;
    virtual void clear() = 0;
  };

  template <typename Event> struct pool_handler final : basic_pool {
    static_assert(std::is_same_v<Event, std::decay_t<Event>>,
                  "Invalid event type");
    static_assert(std::is_base_of_v<calico_event, Event>,
                  "Event must be derived from calico_event");

    using signal_type = entt::sigh<void(Event &)>;
    using sink_type = typename signal_type::sink_type;

    void publish() override;
    void disconnect(void *instance) override;
    void clear() override;
    sink_type bucket();

    template <typename... Args> void trigger(Args &&...args);
    template <typename... Args> void enqueue(Args &&...args);

  private:
    signal_type signal_{};
    std::vector<Event> events_;
  };

  template <typename Event> [[nodiscard]] pool_handler<Event> &assure();

public:
  /*! @brief Default constructor. */
  dispatcher() = default;

  /*! @brief Default move constructor. */
  dispatcher(dispatcher &&) = default;

  /*! @brief Default move assignment operator. @return This dispatcher. */
  dispatcher &operator=(dispatcher &&) = default;

  /**
   * @brief Returns a sink object for the given event.
   *
   * A sink is an opaque object used to connect listeners to events.
   *
   * The function type for a listener is _compatible_ with:
   * @code{.cpp}
   * void(Event &);
   * @endcode
   *
   * The order of invocation of the listeners isn't guaranteed.
   *
   * @sa sink
   *
   * @tparam Event Type of event of which to get the sink.
   * @return A temporary sink object.
   */
  template <typename Event> auto sink();

  /**
   * @brief Triggers an immediate event of the given type.
   *
   * All the listeners registered for the given type are immediately notified.
   * The event is discarded after the execution.
   *
   * @tparam Event Type of event to trigger.
   * @tparam Args Types of arguments to use to construct the event.
   * @param args Arguments to use to construct the event.
   */
  template <typename Event, typename... Args> void trigger(Args &&...args);

  /**
   * @brief Triggers an immediate event of the given type.
   *
   * All the listeners registered for the given type are immediately notified.
   * The event is discarded after the execution.
   *
   * @tparam Event Type of event to trigger.
   * @param event An instance of the given type of event.
   */
  template <typename Event> void trigger(Event &&event);

  /**
   * @brief Enqueue an event of the given type.
   *
   * An event of the given type is queued. No listener is invoked. Use the
   * `update` member function to notify listeners when ready.
   *
   * @tparam Event Type of event to enqueue.
   * @tparam Args Types of arguments to use to construct the event.
   * @param args Arguments to use to construct the event.
   */
  template <typename Event, typename... Args> void enqueue(Args &&...args);

  /**
   * @brief Enqueue an event of the given type.
   *
   * An event of the given type is queued. No listener is invoked. Use the
   * `update` member function to notify listeners when ready.
   *
   * @tparam Event Type of event to enqueue.
   * @param event An instance of the given type of event.
   */
  template <typename Event> void enqueue(Event &&event);

  /**
   * @brief Utility function to disconnect everything related to a given value
   * or instance from a dispatcher.
   * @tparam Type Type of class or type of payload.
   * @param value_or_instance A valid object that fits the purpose.
   */
  template <typename Type> void disconnect(Type &value_or_instance);

  /**
   * @brief Utility function to disconnect everything related to a given value
   * or instance from a dispatcher.
   * @tparam Type Type of class or type of payload.
   * @param value_or_instance A valid object that fits the purpose.
   */
  template <typename Type> void disconnect(Type *value_or_instance);

  /**
   * @brief Discards all the events queued so far.
   *
   * If no types are provided, the dispatcher will clear all the existing
   * pools.
   *
   * @tparam Event Type of events to discard.
   */
  template <typename... Event> void clear();

  /**
   * @brief Delivers all the pending events of the given type.
   *
   * This method is blocking and it doesn't return until all the events are
   * delivered to the registered listeners. It's responsibility of the users
   * to reduce at a minimum the time spent in the bodies of the listeners.
   *
   * @tparam Event Type of events to send.
   */
  template <typename Event> void update();

  /**
   * @brief Delivers all the pending events.
   *
   * This method is blocking and it doesn't return until all the events are
   * delivered to the registered listeners. It's responsibility of the users
   * to reduce at a minimum the time spent in the bodies of the listeners.
   */
  void update();

private:
  std::vector<std::unique_ptr<basic_pool>> pools_;
};

} // namespace calico
