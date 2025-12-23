#pragma once

namespace LAlex {
    struct MenuItem {
        const char* title;
        const MenuItem* (*func)(const MenuItem* current);
        const MenuItem* parent;
        MenuItem** children;
        int children_count;
    };
}
