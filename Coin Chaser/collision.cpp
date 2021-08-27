#include <stdio.h>
#include <stdlib.h>

#include "header.h"
#include "player.h"
#include "chunkMap.h"

//this function recieves the position of two items
//the positions of these items are used to determine whether they are colliding
//true is returned if they are colliding and false is returned if they are not
bool isCollision(entityPos item1, entityPos item2) {

    //the positions of the two items are compared
    if (item1.rightX < item2.leftX || item1.leftX > item2.rightX || item1.btmY < item2.topY || item1.topY > item2.btmY) {
        return false;
    } else {
        return true;
    }
}

//this function recieves the position of two items
//the positions of these items are used to check which direction the second item is compared to the first
//1 is returned for left, 2 is returned for right, 3 is returned for up, and 4 is returned for down
int direction(entityPos item1, entityPos item2) {

    //the displacement between the items is calculated
    int dx = item2.center[0] - item1.center[0];
    int dy = item2.center[1] - item1.center[1];

    //a direction is returned based on the displacement between the items
    if (abs(dx) > abs(dy)) {
        if (dx < 0) {
            return 1;   //left
        } else {
            return 2;   //right
        }
    } else {
        if (dy < 0) {
            return 3;   //up
        } else {
            return 4;   //down
        }
    }
}

//this funtion recives the chunk render radius, an entity's position, the chunkmap, and the chunk coordinates of the entity
//obstacle collision checks whether items are colliding, then calls the snap funtion to move the entity if necessary
//the updated entity position is returned
entityPos obstacleCollision(int r, entityPos entity, ChunkMap &chunkMap, int chunkCoordsX, int chunkCoordsY) {

    //for loops use the position of every obstacle in the entity's radius to determine whether the entity is colliding with an obstacle
    for (int i = chunkCoordsX - r + 1; i < chunkCoordsX + r ; i++) {
        for (int j = chunkCoordsY - r + 1; j < chunkCoordsY + r; j++) {
            for (int k = 0; k < chunkMap.fetchOrGen(i, j)->obstacle1Vector.size(); k++) {

                //obstacle collision boxes are calculated using the obstacles position and size
                entityPos obstacle1Box;
                obstacle1Box.leftX = chunkMap.fetch(i, j)->obstacle1Vector[k].pos[0] + 50 + chunkMap.fetch(i, j)->obstacle1Vector[k].displacement[0];
                obstacle1Box.topY = chunkMap.fetch(i, j)->obstacle1Vector[k].pos[1] + 50 + chunkMap.fetch(i, j)->obstacle1Vector[k].displacement[1];
                obstacle1Box.rightX = obstacle1Box.leftX + chunkMap.fetch(i, j)->obstacle1Vector[k].boxSize[0];
                obstacle1Box.btmY = obstacle1Box.topY + chunkMap.fetch(i, j)->obstacle1Vector[k].boxSize[1];

                //the snap function is called if the obstacle is colliding with the entity
                if (isCollision(entity, obstacle1Box)) {
                    obstacle1Box.center[0] = (obstacle1Box.leftX + obstacle1Box.rightX) / 2;
                    obstacle1Box.center[1] = (obstacle1Box.topY + obstacle1Box.btmY) / 2;
                    entity = snap(entity, obstacle1Box, direction(entity, obstacle1Box));
                }
            }
            for (int k = 0; k < chunkMap.fetchOrGen(i, j)->obstacle2Vector.size(); k++) {

                //obstacle collision boxes are calculated using the obstacles position and size
                entityPos obstacle2Box;
                obstacle2Box.leftX = chunkMap.fetch(i, j)->obstacle2Vector[k].pos[0] + 50 + chunkMap.fetch(i, j)->obstacle2Vector[k].displacement[0];
                obstacle2Box.topY = chunkMap.fetch(i, j)->obstacle2Vector[k].pos[1] + 50 + chunkMap.fetch(i, j)->obstacle2Vector[k].displacement[1];
                obstacle2Box.rightX = obstacle2Box.leftX + chunkMap.fetch(i, j)->obstacle2Vector[k].boxSize[0];
                obstacle2Box.btmY = obstacle2Box.topY + chunkMap.fetch(i, j)->obstacle2Vector[k].boxSize[1];

                //the snap function is called if the obstacle is colliding with the entity
                if (isCollision(entity, obstacle2Box)) {
                    obstacle2Box.center[0] = (obstacle2Box.leftX + obstacle2Box.rightX) / 2;
                    obstacle2Box.center[1] = (obstacle2Box.topY + obstacle2Box.btmY) / 2;
                    entity = snap(entity, obstacle2Box, direction(entity, obstacle2Box));
                }
            }
        }
    }
    return entity;
}

//this function recieves the position of an entity and a position box as well as the direction of collision
//the function snaps the entity's position to the position box
//the entity's new position is returned
entityPos snap(entityPos playerPos, entityPos Box, int direction) {

    //based on the direction of the collision, the entity's position is altered so it does not pass through the obstacle
    if (direction == 1) {
        playerPos.center[0] += Box.rightX - playerPos.leftX + 1;
        playerPos.rightX += Box.rightX - playerPos.leftX + 1;
        playerPos.leftX = Box.rightX + 1;
    } else if (direction == 2) {
        playerPos.center[0] += Box.leftX - playerPos.rightX - 1;
        playerPos.leftX += Box.leftX - playerPos.rightX - 1;
        playerPos.rightX = Box.leftX - 1;
    } else if (direction == 3) {
        playerPos.center[1] += Box.btmY - playerPos.topY + 1;
        playerPos.btmY += Box.btmY - playerPos.topY + 1;
        playerPos.topY = Box.btmY + 1;
    } else if (direction == 4) {
        playerPos.center[1] += Box.topY - playerPos.btmY - 1;
        playerPos.topY += Box.topY - playerPos.btmY - 1;
        playerPos.btmY = Box.topY - 1;
    }
    return playerPos;
}

//this function recieves the chunk render raduis, the player's position, the type of collectible, the chunkmap, and the chunk coordinates of the player
//the number of times the player has collided with the given collectible type is calculated
//the number of collectibles collected by the player is returned as an integer
int collectibleCollision(int r, int collectibleType, entityPos playerPos, ChunkMap &chunkMap, int chunkCoordsX, int chunkCoordsY) {

    //vector for collectibles collected is declared
    std::vector<std::vector<int>> collected;

    //for loops check if the collectibles of the given type in the chunk render raduis are colliding with the player
    for (int i = chunkCoordsX - r + 1; i < chunkCoordsX + r ; i++) {
        for (int j = chunkCoordsY - r + 1; j < chunkCoordsY + r; j++) {

            if (collectibleType == 1) {
                for (int k = 0; k < chunkMap.fetchOrGen(i, j)->coinVector.size(); k++) {

                    //collision box is created for each coin in the chunk render radius based on their position and size
                    entityPos coinBox;
                    coinBox.leftX = chunkMap.fetch(i, j)->coinVector[k].pos[0] + 50;
                    coinBox.topY = chunkMap.fetch(i, j)->coinVector[k].pos[1] + 50;
                    coinBox.rightX = coinBox.leftX + 35;
                    coinBox.btmY = coinBox.topY + 35;

                    //isCollision function is called to determine whether coins have collided with the player
                    if (isCollision(playerPos, coinBox)) {
                        al_play_sample_instance(chunkMap.fetchOrGen(i, j)->coinVector[k].coin_sound_instance);
                        std::vector<int> bufferCoin = {i, j, k};
                        collected.push_back(bufferCoin);
                    }
                }
            } else if (collectibleType == 2) {
                for (int k = 0; k < chunkMap.fetchOrGen(i, j)->fruitVector.size(); k++) {

                    //collision box is created for each fruit in the chunk render radius based on their position and size
                    entityPos fruitBox;
                    fruitBox.leftX = chunkMap.fetch(i, j)->fruitVector[k].pos[0] + 60;
                    fruitBox.topY = chunkMap.fetch(i, j)->fruitVector[k].pos[1] + 60;
                    fruitBox.rightX = fruitBox.leftX + 30;
                    fruitBox.btmY = fruitBox.topY + 30;

                    //isCollision function is called to determine whether fruits have collided with the player
                    if (isCollision(playerPos, fruitBox)) {
                        std::vector<int> bufferFruit = {i, j, k};
                        collected.push_back(bufferFruit);
                    }
                }
            } else if (collectibleType == 3) {
                for (int k = 0; k < chunkMap.fetchOrGen(i, j)->powerUpVector.size(); k++) {

                    //collision box is created for each power up in the chunk render radius based on their position and size
                    entityPos powerUpBox;
                    powerUpBox.leftX = chunkMap.fetch(i, j)->powerUpVector[k].pos[0] + 50;
                    powerUpBox.topY = chunkMap.fetch(i, j)->powerUpVector[k].pos[1] + 50;
                    powerUpBox.rightX = powerUpBox.leftX + 35;
                    powerUpBox.btmY = powerUpBox.topY + 35;

                    //isCollision function is called to determine whether power ups have collided with the player
                    if (isCollision(playerPos, powerUpBox)) {
                        std::vector<int> bufferPowerUp = {i, j, k};
                        collected.push_back(bufferPowerUp);
                    }
                }
            }
        }
    }
    int total = 0;
    for (int i = 0; i < collected.size(); i++) {

        //the total collisions between the player and the given collectible is calculated
        total++;
        int x = collected[i][0];
        int y = collected[i][1];
        int index = collected[i][2];

        //if the player has collected the collectible, it is erased from its vector
        if (collectibleType == 1) {
            chunkMap.fetch(x, y)->coinVector.erase(chunkMap.fetch(x, y)->coinVector.begin() + index);
        } else if (collectibleType == 2) {
            chunkMap.fetch(x, y)->fruitVector.erase(chunkMap.fetch(x, y)->fruitVector.begin() + index);
        } else if (collectibleType == 3) {
            chunkMap.fetch(x, y)->powerUpVector.erase(chunkMap.fetch(x, y)->powerUpVector.begin() + index);
        }
    }
    return total;
}
