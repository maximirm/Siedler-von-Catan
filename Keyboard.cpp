#include "GLIncludes.hpp"
#include "Keyboard.hpp"
#include "Input.hpp"
#include "Context.hpp"

bool Keyboard::isActive(Keyboard::Modifier modifier) {

    switch (modifier) {
    case ALT:
        return modifiers & GLUT_ACTIVE_ALT;
        break;
    case CTRL:
        return modifiers & GLUT_ACTIVE_CTRL;
        break;
    case SHIFT:
        return modifiers & GLUT_ACTIVE_SHIFT;
        break;
    default:
        break;
    }
    return false;
}

