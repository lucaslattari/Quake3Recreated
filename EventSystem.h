#pragma once

#include <functional> // Includes std::function, allowing to store callable objects
#include <map>
#include <vector>

// Enum for different types of events that you may want to support.
enum class EventType {
    KeyPress,
    MouseMove,
    // ... other event types
};

// Class that represents a specific event.
class Event {
public:
    EventType type;
    // ... other specific properties of the event (e.g., key pressed, mouse position, etc.)
};

// Function type for event listeners.
using EventListener = std::function<void(const Event&)>; // Using std::function to store a callable object

// Class to manage events.
class EventSystem {
private:
    std::map<EventType, std::vector<EventListener>> listeners; // Mapping event types to their listeners

public:
    // Add a listener for a specific event type.
    void addListener(EventType type, EventListener listener) {
        listeners[type].push_back(listener);
    }

    // todo: Remove a listener for a specific event type.

    // Trigger an event, notifying all registered listeners for that event type.
    void fireEvent(const Event& event) {
        auto& listenersForEvent = listeners[event.type];
        for (const auto& listener : listenersForEvent) {
            listener(event);
        }
    }
};
