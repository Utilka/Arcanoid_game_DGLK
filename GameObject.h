//
// Created by Utilka on 10/13/2022.
//
#include <Framework.h>

#ifndef ARCANOID_CMAKE_GAMEOBJECT_H
#define ARCANOID_CMAKE_GAMEOBJECT_H

namespace MyGame {
    class GameObject {
    public:
        Sprite *sprite;
        int l_x, l_y;
        int s_x, s_y;

        GameObject();

        GameObject(int location_x, int location_y, int size_x, int size_y);

        explicit GameObject(const char *spritePath);

        GameObject(const char *spritePath, int location_x, int location_y, int size_x, int size_y);

        explicit GameObject(Sprite *newSprite);

        GameObject(Sprite *newSprite, int location_x, int location_y, int size_x, int size_y);

        void changeSprite(Sprite *newSprite);

        void changeLocation(int location_x, int location_y);

        void changeSize(int size_x, int size_y);

        void Draw();
    };

    class Platform : public GameObject {
    public:
        Platform();

        Platform(const char *spritePath, int playingWidth, int playingHeight);

        void ApplyAbility();

    };

    class Block : public GameObject {

    };

    class Wall : public GameObject {

    };

    class Ball : public GameObject {
    public:
        double l_x, l_y;
        double speed_x, speed_y;

        Ball();
        Ball(const char *spritePath, int playingWidth, int playingHeight);

        void ballLogic(Platform* player,GameObject objectList[],int objectListSize);

        bool checkCollision(GameObject *otherGameObject);

        bool changeSpeed(double newSpeed_x,double newSpeed_y);

        void Start(int target_x, int target_y);

        void Draw();
    };

    class Floor : public Wall {

    };

}


#endif //ARCANOID_CMAKE_GAMEOBJECT_H
