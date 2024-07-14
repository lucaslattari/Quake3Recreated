#include "EventSystem.h"

KeyEvent::KeyEvent(int key, int scancode, int action, int mods)
    : key(key), scancode(scancode), action(action), mods(mods) {
    type = EventType::KeyPress;
}

MouseMoveEvent::MouseMoveEvent(double xpos, double ypos)
    : xpos(xpos), ypos(ypos) {
    type = EventType::MouseMove;
}

MouseScrollEvent::MouseScrollEvent(double xoffset, double yoffset) // Adicionado
    : xoffset(xoffset), yoffset(yoffset) {
    type = EventType::MouseScroll;
}

void EventSystem::addListener(EventType type, EventListener listener) {
    listeners[type].push_back(listener);
}

void EventSystem::fireEvent(const Event& event) {
    auto& listenersForEvent = listeners[event.type];
    for (const auto& listener : listenersForEvent) {
        listener(event);
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    EventSystem* eventSystem = static_cast<EventSystem*>(glfwGetWindowUserPointer(window));
    KeyEvent keyEvent(key, scancode, action, mods);
    eventSystem->fireEvent(keyEvent);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    EventSystem* eventSystem = static_cast<EventSystem*>(glfwGetWindowUserPointer(window));
    MouseMoveEvent mouseEvent(xpos, ypos);
    eventSystem->fireEvent(mouseEvent);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) { // Adicionado
    EventSystem* eventSystem = static_cast<EventSystem*>(glfwGetWindowUserPointer(window));
    MouseScrollEvent scrollEvent(xoffset, yoffset);
    eventSystem->fireEvent(scrollEvent);
}
