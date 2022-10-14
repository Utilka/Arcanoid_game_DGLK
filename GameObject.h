//
// Created by Utilka on 10/13/2022.
//
#include <Framework.h>

#ifndef ARCANOID_CMAKE_GAMEOBJECT_H
#define ARCANOID_CMAKE_GAMEOBJECT_H

namespace MyGame {
    class GameObject {
    public:
        Sprite *sprite{};
        double l_x{}, l_y{};
        int s_x{}, s_y{};

        GameObject();

        void changeSprite(Sprite *newSprite);

        void changeLocation(double location_x, double location_y);

        void changeSize(int size_x, int size_y);

        virtual void Draw();
    };

    class Platform : public GameObject {
    public:
        Platform();

        Platform(const char *spritePath, int playingWidth, int playingHeight);

        void ApplyAbility();

    };

    class Block : public GameObject {
    public:

        Sprite *undamagedSprite{};
        Sprite *damagedSprite{};

        int maxPoints{1};
        int hitPoints{1};

        Block();

        // spriteSelector expects index from filename of undamaged block
        Block(int spriteSelector, int initHitPoints, double location_x, double location_y, int width, int height);

        void damage(int value = 1);

        void Draw();
    };

    class Ball : public GameObject {
    public:
        double speed_x{}, speed_y{};

        Ball();

        Ball(const char *spritePath, int playingWidth, int playingHeight);

        void checkAllCollisions(Platform *player);

        void checkAllCollisions(Platform *player, Block *blockList, int blockListSize);

        bool checkCollision(GameObject *targetGO);

        // here none means that target was colliding with source object (ball) on previous tick
        // vertical - object hit left or right side, horizontal - top or bottom, both - edge
        enum collisionSide {none, vertical, horizontal, both};
        collisionSide getCollisionSide(GameObject *targetGO);

        void Launch(int target_x, int target_y);

    };

    class Floor {

    };

}


#endif //ARCANOID_CMAKE_GAMEOBJECT_H
