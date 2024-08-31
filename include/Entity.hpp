#ifndef Entity_HPP_
#define Entity_HPP_

#include <raylib.h>

enum class EntityType
{
    EMPTY,
    FISH,
    SHARK,
};

struct Entity
{
    Vector2 position;
    EntityType type;
    int timer;
};

#endif