//
// Created by Utilka on 10/13/2022.
//

#include <iostream>
#include <cmath>
#include <string>
#include "GameObject.h"
#include "Framework.h"

#define INIT_BALL_SPEED 1


namespace MyGame {
    GameObject::GameObject() = default;

    void GameObject::changeSprite(Sprite *newSprite) {
        sprite = newSprite;
        setSpriteSize(sprite, s_x, s_y);
    }

    void GameObject::changeLocation(double location_x, double location_y) {
        l_x = location_x;
        l_y = location_y;
    }

    void GameObject::changeSize(int size_x, int size_y) {
        s_x = size_x;
        s_y = size_y;
        setSpriteSize(sprite, s_x, s_y);
    }

    void GameObject::Draw() {
        drawSprite(sprite, l_x, l_y);
    }

    Platform::Platform() {
        l_x = 0;
        l_y = 0;
        s_x = 0;
        s_y = 0;
    }

    Platform::Platform(const char *spritePath, int playingWidth, int playingHeight) {

        sprite = createSprite(spritePath);
        s_x = playingWidth / 8;
        s_y = s_x * 128 / 485;
        l_x = playingWidth / 2 - s_x / 2;
        l_y = playingHeight - playingHeight / 20 - s_y;
        setSpriteSize(sprite, s_x, s_y);
    }

    Ball::Ball() = default;

    Ball::Ball(const char *spritePath, int playingWidth,
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

// launches a ball into given direction with predetermined speed
    void Ball::Launch(int target_x, int target_y) {
        speed_x = double(target_x - l_x - s_x / 2) / 100;
        speed_y = double(target_y - l_y - s_y / 2) / 100;
//    double relative_target_x = double(double(target_x) - l_x - double(s_x) / 2);
//    double relative_target_y = double(double(target_y) - l_y - double(s_y) / 2);
//
//    printf("relative_target_x: %f | relative_target_y: %f\n",relative_target_x,relative_target_y);
//
//    const double predetVectorLength = std::pow(INIT_BALL_SPEED,2);
//    double targetVectorLength = std::pow(relative_target_x,2) + std::pow(relative_target_y,2);
//    double scaleDifference = std::sqrt(predetVectorLength/targetVectorLength);
//    printf("predetVectorLength: %f | targetVectorLength: %f | scaleDifference: %f\n",predetVectorLength,targetVectorLength,scaleDifference );
//
//    speed_x = relative_target_x*scaleDifference;
//    speed_y = relative_target_y*scaleDifference;
//    printf("speed_x: %f | speed_y: %f | vector length: %f\n\n",speed_x,speed_y,std::sqrt(std::pow(speed_x,2) + std::pow(speed_y,2)));

    }

    void
    Ball::checkAllCollisions(Platform *player, Block *blockList, int blockListSize) {
        int bottomSide = 600 - this->s_y;
        int rightSide = 800 - this->s_x;

        //TODO change to branchless?
        if ((this->l_x <= 0) || (this->l_x >= rightSide)) {
            std::cout << "amongus" << std::endl;
            this->speed_x = -speed_x;
        }
        //TODO change to branchless?
        if ((this->l_y <= 0) || (this->l_y >= bottomSide)) {
            std::cout << "amongus" << std::endl;
            this->speed_y = -speed_y;
        }

        if (this->checkCollision(player)) {
            std::cout << "amongus" << std::endl;
            collisionSide side = this->getCollisionSide(player);
            std::cout << side << std::endl;
            std::cout << (side & Ball::collisionSide::vertical) << std::endl;
            std::cout << (side & Ball::collisionSide::horizontal) << std::endl;
            if ((side & Ball::collisionSide::vertical)) { this->speed_x = -speed_x; } //TODO change to branchless?
            if ((side & Ball::collisionSide::horizontal)) { this->speed_y = -speed_y; }
        }

        for (int i = 0; i < blockListSize; i++) {
            //TODO change to branchless? and add better dead block collision removal
            if (this->checkCollision(&blockList[i])&& (blockList[i].hitPoints>0)) {
                std::cout << "amongus" << std::endl;
                collisionSide side = this->getCollisionSide(player);
                std::cout << side << std::endl;
                std::cout << (side & Ball::collisionSide::vertical) << std::endl;
                std::cout << (side & Ball::collisionSide::horizontal) << std::endl;
                if ((side & Ball::collisionSide::vertical)) { this->speed_x = -speed_x; } //TODO change to branchless?
                if ((side & Ball::collisionSide::horizontal)) { this->speed_y = -speed_y; }

                blockList[i].damage();


            }
        }

    }

    void Ball::checkAllCollisions(Platform *player) {

    }

// to check for collision we will use oneline branchless logic statement
// which is best explained in two pieces:
// true if one rectangle is on left or right side of other
//    ((l_x > (targetGO->l_x + targetGO->s_x)) || (targetGO->l_x > (l_x + s_x)))
// true if one rectangle is above ir below other
//    (((l_y + s_y) < targetGO->l_y) || ((targetGO->l_y + targetGO->s_y) < l_y))
// OR them together and take NOT from it to return true when rectangles collide

    bool Ball::checkCollision(GameObject *targetGO) {
        return !(((l_x > (targetGO->l_x + targetGO->s_x)) || (targetGO->l_x > (l_x + s_x))) ||
                 (((l_y + s_y) < targetGO->l_y) || ((targetGO->l_y + targetGO->s_y) < l_y)));
    }

// to get direction from which collision occurred its easiest to test both collision conditions
// on state of the ball that was in on tick before the collision and see which condition is no not satisfied
    Ball::collisionSide Ball::getCollisionSide(GameObject *targetGO) {
        double prevL_x = l_x - speed_x;
        double prevL_y = l_y - speed_y;

        bool verticalCollision = ((prevL_x > (targetGO->l_x + targetGO->s_x)) || (targetGO->l_x > (prevL_x + s_x)));
        bool horizontalCollision = (((prevL_y + s_y) < targetGO->l_y) || ((targetGO->l_y + targetGO->s_y) < prevL_y));

        return collisionSide(verticalCollision + horizontalCollision * 2);
    }

    Block::Block() = default;

    Block::Block(int spriteSelector, int initHitPoints, double location_x, double location_y, int width, int height) {

//        char* s1 [128];
//        char* s2 [128];
//        std::sprintf(reinterpret_cast<char *>(s1), "./data/%d-Breakout-Tiles.png", spriteSelector);
//        std::sprintf(reinterpret_cast<char *>(s2), "./data/%d-Breakout-Tiles.png", spriteSelector + 1);
        undamagedSprite = createSprite("./data/15-Breakout-Tiles.png");
        damagedSprite = createSprite("./data/16-Breakout-Tiles.png");
        sprite = undamagedSprite;

        hitPoints = initHitPoints;

        s_x = width;
        s_y = height;
        l_x = location_x;
        l_y = location_y;
        setSpriteSize(undamagedSprite, s_x, s_y);
        setSpriteSize(damagedSprite, s_x, s_y);
    }

    void Block::damage(int value) {
        hitPoints -= value;
        sprite = damagedSprite;
    }

    void Block::Draw() {
        if (hitPoints > 0) {
            drawSprite(sprite, l_x, l_y);
        }
    }


}