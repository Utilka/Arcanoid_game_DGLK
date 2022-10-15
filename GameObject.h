//
// Created by Utilka on 10/13/2022.
//
#include <Framework.h>

#ifndef ARCANOID_CMAKE_GAMEOBJECT_H
#define ARCANOID_CMAKE_GAMEOBJECT_H

namespace MyGame {
    class GameObject {
    public:

        static Sprite *sprite;
        double l_x{}, l_y{};
        int s_x{}, s_y{};

        GameObject();

        GameObject(double location_x, double location_y, int size_x, int size_y);

        void changeSprite(Sprite *newSprite);

        void changeLocation(double location_x, double location_y);

        void changeSize(int size_x, int size_y);

        virtual void Draw();
    };

    class Platform : public GameObject {
    public:
        static Sprite *sprite;
        Platform();

        Platform(double location_x, double location_y, int size_x, int size_y);
        void Draw() override;

        void ApplyAbility();

    };

    class Block : public GameObject {
    public:

        static Sprite *sprite;
        static Sprite *blockSprite[4];

        int maxPoints{1};
        int hitPoints{1};

        Block();

        // spriteSelector expects index from filename of undamaged block
        Block(int spriteSelector, int initHitPoints, double location_x, double location_y, int width, int height);

        void damage(int value = 1);

        void Draw() override;
    };

    class Ball : public GameObject {
    public:
        static Sprite *sprite;
        double baseSpeed_x{}, baseSpeed_y{},modifier{1};

        Ball();

        Ball(double location_x, double location_y, int size_x, int size_y);

        void checkAllCollisions(Platform *player, Block *blockList, int blockListSize);

        bool checkCollision(GameObject *targetGO);

        // here none means that target was colliding with source object (ball) on previous tick
        // vertical - object hit left or right side, horizontal - top or bottom, both - edge
        enum collisionSide {none, vertical, horizontal, both};
        collisionSide getCollisionSide(GameObject *targetGO);

        void launch(int target_x, int target_y);

        double getSpeed_x() const;
        double getSpeed_y() const;
        // modifier value is limited between 0.4 and 3
        void setModifier(double newValue);

        void Draw() override;
    };

    class Ability : public Ball {
    public:

        static Sprite *abilitySprite[3];
        bool active{0};
        int effect{0};
        int resetPos_x{0}, resetPos_y{0};

        Ability();
        Ability(int ef, double location_x, double location_y,int size_x, int size_y);

        void Draw() override;
        void spawn(int target_x, int target_y);

        void reset();

        void checkAllCollisions(Platform *player);


    };

}


#endif //ARCANOID_CMAKE_GAMEOBJECT_H
