#pragma once

#include <functional>
#include <map>
#include <vector>
#include <GLFW/glfw3.h>

enum class EventType {
    KeyPress,
    MouseMove,
    MouseScroll, // Adicionado
};

class Event {
public:
    EventType type;
};

class KeyEvent : public Event {
public:
    int key, scancode, action, mods;
    KeyEvent(int key, int scancode, int action, int mods);
};

class MouseMoveEvent : public Event {
public:
    double xpos, ypos;
    MouseMoveEvent(double xpos, double ypos);
};

class MouseScrollEvent : public Event {
public:
    double xoffset, yoffset;
    MouseScrollEvent(double xoffset, double yoffset);
};

using EventListener = std::function<void(const Event&)>;

class EventSystem {
private:
    std::map<EventType, std::vector<EventListener>> listeners;

public:
    void addListener(EventType type, EventListener listener);
    void fireEvent(const Event& event);
};

// Declaração dos callbacks
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
