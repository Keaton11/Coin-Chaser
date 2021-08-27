#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

#include "header.h"
#include "chunkMap.h"

//function recieves the chunkmap, and three integers which correspond to the coordinates and vector number of a given enemy
//the enemy's position box is determined using their size and position
//the position of the enemy's position box is returned
entityPos updateEnemyBox(ChunkMap &chunkMap, int x, int y, int vectorNum) {

    //enemy's position box is created using enemy's size and coordinates
    entityPos enemyBox;
    enemyBox.leftX = chunkMap.fetch(x, y)->enemyVector[vectorNum].pos[0] + 50;
    enemyBox.topY = chunkMap.fetch(x, y)->enemyVector[vectorNum].pos[1] + 50;
    enemyBox.rightX = enemyBox.leftX + chunkMap.fetch(x, y)->enemyVector[vectorNum].boxSize[0];
    enemyBox.btmY = enemyBox.topY + chunkMap.fetch(x, y)->enemyVector[vectorNum].boxSize[1];

    //the center of the enemy is calculated
    enemyBox.center[0] = (enemyBox.leftX + enemyBox.rightX) / 2;
    enemyBox.center[1] = (enemyBox.topY + enemyBox.btmY) / 2;

    //the chunk the enemy is in is determined
    enemyBox.chunkCoords[0] = std::floor(float(chunkMap.fetch(x, y)->enemyVector[vectorNum].pos[0]) / (Chunk::chunkSize * Chunk::tileSize));
    enemyBox.chunkCoords[1] = std::floor(float(chunkMap.fetch(x, y)->enemyVector[vectorNum].pos[1]) / (Chunk::chunkSize * Chunk::tileSize));
    return enemyBox;
}

//function recieves the chunk render radius, the player's position, the chunkmap, the chunk coordinates of the player, and the player's health
//function checks whether the player has been hit by an enemy and updates the players position
//the player's new position is returned as a struct
entityPos enemyCollision(int r, entityPos playerPos, ChunkMap &chunkMap, int chunkCoordsX, int chunkCoordsY, int &health) {

    //for loops determines the position box of every enemy within the chunk render radius
    for (int i = chunkCoordsX - r + 1; i < chunkCoordsX + r ; i++) {
        for (int j = chunkCoordsY - r + 1; j < chunkCoordsY + r; j++) {
            for (int k = 0; k < chunkMap.fetchOrGen(i, j)->enemyVector.size(); k++) {

                entityPos enemyBox;
                enemyBox = updateEnemyBox(chunkMap, i, j, k);

                //if statement determines whether the enemy's collision box collides with the player
                if (isCollision(playerPos, enemyBox)) {

                    //if they have collided, the player's health decreases by one and a sound effect is played
                    health--;
                    al_play_sample_instance(chunkMap.fetchOrGen(i, j)->enemyVector[k].enemy_sound_instance);

                    //the player's health does not decrease if it is already at zero
                    if (health < 0) {
                        health = 0;
                    }

                    //the players position is updated based on the movement of the enemy
                    playerPos = snap(playerPos, enemyBox, direction(playerPos, enemyBox));
                }
            }
        }
    }
    return playerPos;
}

//function recieves the chunk render radius, the position of the player, the chunkmap and the chunk coordinates of the player
//function moves all enemies within a certain radius towards the player
//no values are returned
void moveEnemies(int r, entityPos playerPos, ChunkMap &chunkMap, int chunkCoordsX, int chunkCoordsY) {

    //for loops determines the position box of every enemy within the chunk render radius
    for (int i = chunkCoordsX - r + 1; i < chunkCoordsX + r ; i++) {
        for (int j = chunkCoordsY - r + 1; j < chunkCoordsY + r; j++) {
            for (int k = 0; k < chunkMap.fetchOrGen(i, j)->enemyVector.size(); k++) {

                //collision box is created for the enemy
                entityPos enemyBox;
                enemyBox = updateEnemyBox(chunkMap, i, j, k);

                //if statement determines whether the displacement from the player to an enemy is less than 700 in any direction
                if (abs(playerPos.leftX - enemyBox.leftX) < 700 || abs(playerPos.topY - enemyBox.topY) < 700) {

                    //switch statement changes the position of enemies within this radius so that they move towards the player
                    switch(direction(enemyBox, playerPos)) {
                        case 1: //left
                            chunkMap.fetch(i, j)->enemyVector[k].pos[0] -= chunkMap.fetch(i, j)->enemyVector[k].speed;
                            break;
                        case 2: //right
                            chunkMap.fetch(i, j)->enemyVector[k].pos[0] += chunkMap.fetch(i, j)->enemyVector[k].speed;
                            break;
                        case 3: //up
                            chunkMap.fetch(i, j)->enemyVector[k].pos[1] -= chunkMap.fetch(i, j)->enemyVector[k].speed;
                            break;
                        case 4: //down
                            chunkMap.fetch(i, j)->enemyVector[k].pos[1] += chunkMap.fetch(i, j)->enemyVector[k].speed;
                            break;
                    }

                    //a new collision box is created for the enemy, and the obstacleCollision function is called
                    entityPos enemyBox;
                    enemyBox = updateEnemyBox(chunkMap, i, j, k);
                    enemyBox = obstacleCollision(r, enemyBox, chunkMap, chunkCoordsX, chunkCoordsY);

                    //the enemy's new position is restored in the enemy vector
                    chunkMap.fetch(i, j)->enemyVector[k].pos[0] = enemyBox.leftX - 50;
                    chunkMap.fetch(i, j)->enemyVector[k].pos[1] = enemyBox.topY - 50;
                    enemyBox = updateEnemyBox(chunkMap, i, j, k);

                    //if the enemy has moved into a new chunk, it is removed from its chunk vector and moved to a new one
                    if (i != enemyBox.chunkCoords[0] || j != enemyBox.chunkCoords[1]) {
                        chunkMap.fetch(enemyBox.chunkCoords[0], enemyBox.chunkCoords[1])->enemyVector.push_back(Enemy(chunkMap.fetch(i, j)->enemyVector[k].pos[0], chunkMap.fetch(i, j)->enemyVector[k].pos[1], chunkMap.fetch(i, j)->enemyVector[k].tileType));
                        chunkMap.fetch(i, j)->enemyVector.erase(chunkMap.fetch(i, j)->enemyVector.begin() + k);
                    }
                }
            }
        }
    }
}
