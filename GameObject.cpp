//
// Created by Utilka on 10/13/2022.
//

#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include "GameObject.h"
#include "Framework.h"

#define INIT_BALL_SPEED 1


namespace MyGame {
    GameObject::GameObject() = default;

    MyGame::GameObject::GameObject(double location_x, double location_y, int size_x, int size_y) {
        l_x = location_x;
        l_y = location_y;
        s_x = size_x;
        s_y = size_y;
    }

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

    Platform::Platform(double location_x, double location_y, int size_x, int size_y) :
            GameObject(location_x, location_y, size_x, size_y) {

    }

    void Platform::Draw() {
        drawSprite(sprite, l_x, l_y);
    }


    Ball::Ball() = default;
    Ball::Ball(double location_x, double location_y, int size_x, int size_y) :
            GameObject(location_x, location_y, size_x, size_y) {

        baseSpeed_x = 0;
        baseSpeed_y = 0;
        modifier = 1;
    }

// launches a ball into given direction with predetermined speed
    void Ball::launch(int target_x, int target_y) {
        baseSpeed_x = double(target_x - l_x - s_x / 2) / 100;
        baseSpeed_y = double(target_y - l_y - s_y / 2) / 100;
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
//    baseSpeed_x = relative_target_x*scaleDifference;
//    baseSpeed_y = relative_target_y*scaleDifference;
//    printf("baseSpeed_x: %f | baseSpeed_y: %f | vector length: %f\n\n",baseSpeed_x,baseSpeed_y,std::sqrt(std::pow(baseSpeed_x,2) + std::pow(baseSpeed_y,2)));
    }

    void
    Ball::checkAllCollisions(Platform *player, Block *blockList, int blockListSize) {
        int bottomSide = 600 - this->s_y;
        int rightSide = 800 - this->s_x;
        //TODO read actual window size

        //TODO change to branchless?
        if ((this->l_x <= 0) || (this->l_x >= rightSide)) {
//            std::cout << "amongus" << std::endl;
            baseSpeed_x = -baseSpeed_x;
            setModifier(modifier+0.1);


        }
        //TODO change to branchless?
        if ((this->l_y <= 0) || (this->l_y >= bottomSide)) {
//            std::cout << "amongus" << std::endl;
            baseSpeed_y = -baseSpeed_y;
            setModifier(modifier+0.1);

        }

        if (this->checkCollision(player)) {
//            std::cout << "amongus" << std::endl;
            collisionSide side = this->getCollisionSide(player);
            if ((side & Ball::collisionSide::vertical)) { this->baseSpeed_x = -baseSpeed_x; } //TODO change to branchless?
            if ((side & Ball::collisionSide::horizontal)) { this->baseSpeed_y = -baseSpeed_y; }

        }

        for (int i = 0; i < blockListSize; i++) {
            //TODO change to branchless? and add better dead block collision removal
            if (this->checkCollision(&blockList[i]) && (blockList[i].hitPoints > 0)) {
//                std::cout << "amongus" << std::endl;
                collisionSide side = this->getCollisionSide(&blockList[i]);
//                std::cout << side << std::endl;
//                std::cout << (side & Ball::collisionSide::vertical) << std::endl;
//                std::cout << (side & Ball::collisionSide::horizontal) << std::endl;
                if ((side & Ball::collisionSide::vertical)) { this->baseSpeed_x = -baseSpeed_x; } //TODO change to branchless?
                if ((side & Ball::collisionSide::horizontal)) { this->baseSpeed_y = -baseSpeed_y; }

                blockList[i].damage();

                setModifier(modifier-0.1);

            }
        }

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
        double prevL_x = l_x - baseSpeed_x;
        double prevL_y = l_y - baseSpeed_y;

//        printf("curr l_x %f|l_y %f|ver %d|hor %d\n",
//               l_x,l_y,targetGO->l_x,,
//               ((l_x > (targetGO->l_x + targetGO->s_x)) || (targetGO->l_x > (l_x + s_x))),(((l_y + s_y) < targetGO->l_y) || ((targetGO->l_y + targetGO->s_y) < l_y)));
        bool verticalCollision = ((prevL_x > (targetGO->l_x + targetGO->s_x)) || (targetGO->l_x > (prevL_x + s_x)));
        bool horizontalCollision = (((prevL_y + s_y) < targetGO->l_y) || ((targetGO->l_y + targetGO->s_y) < prevL_y));
//        printf("prev l_x %f|l_y %f|ver %d|hor %d\n",prevL_x,prevL_y,verticalCollision,horizontalCollision);

        return collisionSide(verticalCollision + horizontalCollision * 2);
    }

    void Ball::Draw() {
        drawSprite(sprite, l_x, l_y);
    }

    double Ball::getSpeed_x() const {
        return baseSpeed_x * modifier;
    }

    double Ball::getSpeed_y() const {
        return baseSpeed_y * modifier;
    }

    void Ball::setModifier(double newValue) {
        modifier = std::max(std::min(newValue,2.0),0.4);
    }


    Block::Block() = default;

    Block::Block(int spriteSelector, int initHitPoints, double location_x, double location_y, int width, int height) {
        hitPoints = initHitPoints;
        s_x = width;
        s_y = height;
        l_x = location_x;
        l_y = location_y;
    }

    void Block::damage(int value) {
        hitPoints -= value;
    }

    void Block::Draw() {

            drawSprite(blockSprite[std::min(hitPoints,3)], l_x, l_y);
        }

    Ability::Ability() = default;

    Ability::Ability(int ef, double location_x, double location_y, int size_x, int size_y) {
        effect = ef;
        resetPos_x=int(location_x);
        resetPos_y=int(location_y);
        l_x=location_x;
        l_y=location_y;
        s_x=size_x;
        s_y=size_y;
        baseSpeed_x = 0;
        baseSpeed_y = 0;
    }

    void Ability::spawn(int target_x, int target_y) {
        active = true;
        launch(target_x,target_y);
    }

    void Ability::Draw() {
        drawSprite(abilitySprite[active*(effect+1)], l_x, l_y);
    }

    void Ability::reset() {
        baseSpeed_x = 0;
        baseSpeed_y = 0;
        l_x=resetPos_x;
        l_y=resetPos_y;
    }

    void Ability::checkAllCollisions(Platform *player) {
        int bottomSide = 600 - this->s_y;
        int rightSide = 800 - this->s_x;
        //TODO read actual window size

        //TODO change to branchless?
        if ((this->l_x <= 0) || (this->l_x >= rightSide)) {
//            std::cout << "amongus" << std::endl;
            baseSpeed_x = -baseSpeed_x;
            setModifier(modifier+0.1);
     }
        //TODO change to branchless?
        if ((this->l_y <= 0) || (this->l_y >= bottomSide)) {
//            std::cout << "amongus" << std::endl;
            baseSpeed_y = -baseSpeed_y;
        }
        if (this->checkCollision(player)) {
//            std::cout << "amongus" << std::endl;

            reset();
        }
    }

}