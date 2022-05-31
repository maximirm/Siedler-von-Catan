#pragma once

struct Keyboard {

    enum Key : unsigned char {
        BACKSPACE = 8,
        TAB = 9,
        ESC = 27,
        SPACE = 32,
        SPECIAL = 255
    } key;

    enum Code {
        LEFT,
        RIGHT,
        UP,
        DOWN
    } code;

    bool pressed;

    enum Modifier { SHIFT, CTRL, ALT };

    bool isActive(Modifier modifier);

    int modifiers;
};
