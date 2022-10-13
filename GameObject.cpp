//
// Created by Utilka on 10/13/2022.
//

#include <iostream>
#include "GameObject.h"
#include "Framework.h"


MyGame::GameObject::GameObject() {
    l_x = 0;
    l_y = 0;
    s_x = 0;
    s_y = 0;
}


MyGame::GameObject::GameObject(double location_x, double location_y, int size_x, int size_y) {
    l_x = location_x;
    l_y = location_y;
    s_x = size_x;
    s_y = size_y;
}

MyGame::GameObject::GameObject(const char *spritePath) {
    sprite = createSprite(spritePath);
    l_x = 0;
    l_y = 0;
    getSpriteSize(sprite, s_x, s_y);
}

MyGame::GameObject::GameObject(const char *spritePath, double location_x, double location_y, int size_x, int size_y) {
    sprite = createSprite(spritePath);
    l_x = location_x;
    l_y = location_y;
    s_x = size_x;
    s_y = size_y;
    setSpriteSize(sprite, s_x, s_y);
}

MyGame::GameObject::GameObject(Sprite *newSprite) {
    sprite = newSprite;
    l_x = 0;
    l_y = 0;
    getSpriteSize(sprite, s_x, s_y);
}

MyGame::GameObject::GameObject(Sprite *newSprite, double location_x, double location_y, int size_x, int size_y) {
    sprite = newSprite;
    l_x = location_x;
    l_y = location_y;
    s_x = size_x;
    s_y = size_y;
    setSpriteSize(sprite, s_x, s_y);
}

void MyGame::GameObject::changeSprite(Sprite *newSprite) {
    sprite = newSprite;
    setSpriteSize(sprite, s_x, s_y);
}

void MyGame::GameObject::changeLocation(double location_x, double location_y) {
    l_x = location_x;
    l_y = location_y;
}

void MyGame::GameObject::changeSize(int size_x, int size_y) {
    s_x = size_x;
    s_y = size_y;
    setSpriteSize(sprite, s_x, s_y);
}

void MyGame::GameObject::Draw() {
    drawSprite(sprite, l_x, l_y);
}

MyGame::Platform::Platform() {
    l_x = 0;
    l_y = 0;
    s_x = 0;
    s_y = 0;
}

MyGame::Platform::Platform(const char *spritePath, int playingWidth, int playingHeight) {

    sprite = createSprite(spritePath);
    s_x = playingWidth / 8;
    s_y = s_x * 128 / 485;
    l_x = playingWidth / 2 - s_x / 2;
    l_y = playingHeight - playingHeight / 20 - s_y;
    setSpriteSize(sprite, s_x, s_y);
}

MyGame::Ball::Ball() {
    l_x = 0;
    l_y = 0;
    s_x = 0;
    s_y = 0;
}

MyGame::Ball::Ball(const char *spritePath, int playingWidth,
                   int playingHeight) {
    sprite = createSprite(spritePath);
    s_x = playingWidth / 30;
    s_y = playingWidth / 30;
    l_x = playingWidth / 2 - s_x / 2;
    l_y = playingHeight / 2 - s_y / 2;
    setSpriteSize(sprite, s_x, s_y);

    speed_x = 0;
    speed_y = 0;
}

void MyGame::Ball::Start(int target_x, int target_y) {
    speed_x = double(target_x - l_x - s_x / 2) / 100;
    speed_y = double(target_y - l_y - s_y / 2) / 100;

}

void MyGame::Ball::Draw() {
    drawSprite(sprite, l_x, l_y);
}

void MyGame::Ball::checkAllCollisions(MyGame::Platform *player, MyGame::GameObject *objectList, int objectListSize) {

}

void MyGame::Ball::checkAllCollisions(MyGame::Platform *player) {

    int bottomSide = 600-this->s_y;
    int rightSide = 800-this->s_x;

    if (this->checkCollision(player)){
        std::cout << "amongus" << std::endl;
        this->speed_y=-speed_y;
    }
    if ((this->l_x<=0)||(this->l_x>=rightSide)){
        std::cout << "amongus" << std::endl;
        this->speed_x=-speed_x;
    }
    if ((this->l_y<=0)||(this->l_y>=bottomSide)){
        std::cout << "amongus" << std::endl;
        this->speed_y=-speed_y;
    }
}

bool MyGame::Ball::checkCollision(MyGame::GameObject *targetGO) {
    // true if one rectangle is on left or right side of other
//    if ((l_x > (targetGO->l_x + targetGO->s_x)) || (targetGO->l_x > (l_x + s_x)))

    // true if one rectangle is above ir below other
//    if (((l_y + s_y) < targetGO->l_y) || ((targetGO->l_y + targetGO->s_y) < l_y))

    // OR them together and take NOT from it to return true when they collide
    return !(((l_x > (targetGO->l_x + targetGO->s_x)) || (targetGO->l_x > (l_x + s_x)))||
        (((l_y + s_y) < targetGO->l_y) || ((targetGO->l_y + targetGO->s_y) < l_y)));
}
