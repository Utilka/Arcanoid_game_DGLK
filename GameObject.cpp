//
// Created by Utilka on 10/13/2022.
//

#include "GameObject.h"
#include "Framework.h"


MyGame::GameObject::GameObject() {
    l_x=0;
    l_y=0;
    s_x=0;
    s_y=0;
    }


MyGame::GameObject::GameObject(int location_x, int location_y, int size_x, int size_y) {
    l_x=location_x;
    l_y=location_y;
    s_x=size_x;
    s_y=size_y;
}

MyGame::GameObject::GameObject(const char *spritePath) {
    sprite =createSprite(spritePath);
    l_x=0;
    l_y=0;
    getSpriteSize(sprite,s_x,s_y);
}

MyGame::GameObject::GameObject(const char *spritePath, int location_x, int location_y, int size_x, int size_y) {
    sprite =createSprite(spritePath);
    l_x=location_x;
    l_y=location_y;
    s_x=size_x;
    s_y=size_y;
    setSpriteSize(sprite, s_x, s_y);
}

MyGame::GameObject::GameObject(Sprite *newSprite) {
    sprite = newSprite;
    l_x=0;
    l_y=0;
    getSpriteSize(sprite,s_x,s_y);
}

MyGame::GameObject::GameObject(Sprite *newSprite, int location_x, int location_y,int size_x,int size_y) {
    sprite = newSprite;
    l_x=location_x;
    l_y=location_y;
    s_x=size_x;
    s_y=size_y;
    setSpriteSize(sprite, s_x, s_y);
}

void MyGame::GameObject::changeSprite(Sprite *newSprite) {
    sprite = newSprite;
    setSpriteSize(sprite, s_x, s_y);
}

void MyGame::GameObject::changeLocation(int location_x, int location_y) {
    l_x=location_x;
    l_y=location_y;
}

void MyGame::GameObject::changeSize(int size_x, int size_y) {
    s_x=size_x;
    s_y=size_y;
    setSpriteSize(sprite, s_x, s_y);
}

void MyGame::GameObject::Draw() {
    drawSprite(sprite,l_x,l_y);
}

MyGame::Platform::Platform() {
    l_x=0;
    l_y=0;
    s_x=0;
    s_y=0;
}

MyGame::Platform::Platform(const char *spritePath, int playingWidth, int playingHeight) {

    sprite =createSprite(spritePath);
    s_x= playingWidth / 8;
    s_y=s_x*128/485;
    l_x= playingWidth / 2 - s_x / 2;
    l_y= playingHeight - playingHeight / 20 - s_y;
    setSpriteSize(sprite, s_x, s_y);
}

MyGame::Ball::Ball() {
    l_x=0;
    l_y=0;
    s_x=0;
    s_y=0;
}

MyGame::Ball::Ball(const char *spritePath, int playingWidth,
                   int playingHeight){
    sprite =createSprite(spritePath);
    s_x= playingWidth / 30;
    s_y= playingWidth / 30;
    l_x= playingWidth / 2 - s_x / 2;
    l_y= playingHeight / 2 - s_y / 2;
    setSpriteSize(sprite, s_x, s_y);

    speed_x=0;
    speed_y=0;
}

void MyGame::Ball::Start(int target_x, int target_y) {
    speed_x = double(target_x-l_x-s_x/2)/100;
    speed_y = double(target_y-l_y-s_y/2)/100;

}

void MyGame::Ball::Draw() {
    drawSprite(sprite,l_x,l_y);
}

void MyGame::Ball::ballLogic(MyGame::Platform *player, MyGame::GameObject *objectList, int objectListSize) {

}

#define a1x l_x
#define a1y l_y
#define a2x (l_x+s_x)
#define a2y l_y
#define a3x l_x
#define a3y (l_y+s_y)
#define a4x (l_x+s_x)
#define a4y (l_y+s_y)
#define b1x otherGameObject->l_x
#define b1y otherGameObject->l_y
#define b2x (otherGameObject->l_x+otherGameObject->s_x)
#define b2y otherGameObject->l_y
#define b3x otherGameObject->l_x
#define b3y (otherGameObject->l_y+otherGameObject->s_y)
#define b4x (otherGameObject->l_x+otherGameObject->s_x)
#define b4y (otherGameObject->l_x+otherGameObject->s_y)

bool MyGame::Ball::checkCollision(MyGame::GameObject *otherGameObject) {
    bool result;
    if (otherGameObject)

    return false;
}

bool MyGame::Ball::changeSpeed(double newSpeed_x, double newSpeed_y) {
    return false;
}
