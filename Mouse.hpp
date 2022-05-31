#pragma once

struct Mouse {
    enum Button { LEFT, RIGHT };
    Button button;
    glm::ivec2 position;
    glm::ivec2 movement;
    bool pressed;
};
