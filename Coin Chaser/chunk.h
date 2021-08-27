#ifndef CHUNK_H
#define CHUNK_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>

#include "obstacle.h"
#include "collectible.h"
#include "enemy.h"
#include "SimplexNoise/SimplexNoise.h"

//class loads tiles, and displays them in chunks of 16 by 16 tiles
//the location of obstacles, collectibles, and enemies is determined
class Chunk {
    public:

        //constructor recieves the seed and the x and y coordinates of chunk
        //tile images as well as the location of tiles, obstacles, collectibles, and enemies are loaded
        Chunk(int x, int y, int seed) {

            //chunk coordinates are saved in coords variables
            coords[0] = x;
            coords[1] = y;

            //water tile is loaded and stored as an allegro bitmap
            waterImage = al_load_bitmap("static/tiles/water.png");
            if (waterImage == nullptr) {
                al_show_native_message_box(display, "Error", "static/tiles/water.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
            }
            tileImages[0] = waterImage;

            //sand tile is loaded and stored as an allegro bitmap
            sandImage = al_load_bitmap("static/tiles/sand.png");
            if (sandImage == nullptr) {
                al_show_native_message_box(display, "Error", "static/tiles/sand.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
            }
            tileImages[1] = sandImage;
            tileImages[3] = sandImage;

            //grass tile is loaded and stored as an allegro bitmap
            grassImage = al_load_bitmap("static/tiles/grass.png");
            if (grassImage == nullptr) {
                al_show_native_message_box(display, "Error", "static/tiles/grass.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
            }
            tileImages[2] = grassImage;
            tileImages[4] = grassImage;

            //mud tile is loaded and stored as an allegro bitmap
            mudImage = al_load_bitmap("static/tiles/mud.png");
            if (mudImage == nullptr) {
                al_show_native_message_box(display, "Error", "static/tiles/mud.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
            }
            tileImages[5] = mudImage;

            //lava tile is loaded and stored as an allegro bitmap
            lavaImage = al_load_bitmap("static/tiles/lava.png");
            if (lavaImage == nullptr) {
                al_show_native_message_box(display, "Error", "static/tiles/darkStone.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
            }
            tileImages[6] = lavaImage;

            //for loops loop through all tiles in the chunk
            for (int i = 0; i < chunkSize; i++) {
                for (int j = 0; j < chunkSize; j++) {

                    //Simplex Noise function outputs values to dynamically generated a tilemap
                    val = SimplexNoise::noise((x + i * 1.0 / chunkSize) / 8, (y + j * 1.0 / chunkSize) / 8, seed);
                    //Simplex Noise function ouputs a float for temperature noise
                    temp = SimplexNoise::noise((x + i * 1.0 / chunkSize) / 8, (y + j * 1.0 / chunkSize) / 8, seed*2);

                    //based on Simplex Noise values, each tile type within the chunk is determined
                    if (val < -0.7) { //water
                        tilesVals[i][j] = 0;
                    } else if (val < -0.5) { //sand
                        tilesVals[i][j] = 1;
                    } else if (val < 0.5) { //grass or desert
                        //desert is displayed if Simplex Noise temperature for the given location is greater than 0.5
                        if (temp > 0.5) {
                            tilesVals[i][j] = 3; //desert
                        } else {
                            tilesVals[i][j] = 4; //grass
                        }
                    } else if (val < 0.7) { //mud
                        tilesVals[i][j] = 5;
                    } else if (val <= 1) { //lava
                        tilesVals[i][j] = 6;
                    }

                    //obstacles, collectibles, and enemies are randomly generated on tiles
                    if ((rand() % 99) == 0 && tilesVals[i][j] != 6){    //1 in 100 chance of an obstacle of type 1 appearing (for all tiles except lava)
                        obstacle1Vector.push_back(Obstacle(chunkSize * tileSize * coords[0] + i * tileSize, chunkSize * tileSize * coords[1] + j *tileSize, tilesVals[i][j], 1));
                    } else if ((rand() % 99) == 0 && tilesVals[i][j] != 6) {    //1 in 100 chance of an obstacle of type 2 appearing (for all tiles except lava)
                        obstacle2Vector.push_back(Obstacle(chunkSize * tileSize * coords[0] + i * tileSize, chunkSize * tileSize * coords[1] + j *tileSize, tilesVals[i][j], 2));
                    } else if ((rand() % 99) == 0) {    //1 in 100 chance of an enemy appearing in any tile type
                        enemyVector.push_back(Enemy(chunkSize * tileSize * coords[0] + i * tileSize, chunkSize * tileSize * coords[1] + j *tileSize, tilesVals[i][j]));
                    } else if ((rand() % 299) == 0 && tilesVals[i][j] != 0 && tilesVals[i][j] != 6) {   //1 in 300 chance of a coin appearing in any tiles except water or lava
                        coinVector.push_back(Collectible(chunkSize * tileSize * coords[0] + i * tileSize, chunkSize * tileSize * coords[1] + j *tileSize, 1));
                    } else if ((rand() % 1499) == 0 && tilesVals[i][j] != 0 && tilesVals[i][j] != 6) {  //1 in 1500 chance of a fruit appearing in any tiles except water or lava
                        fruitVector.push_back(Collectible(chunkSize * tileSize * coords[0] + i * tileSize, chunkSize * tileSize * coords[1] + j *tileSize, 2));
                    } else if ((rand() % 1999) == 0 && tilesVals[i][j] != 0 && tilesVals[i][j] != 6) {  //1 in 2000 chance of a power up appearing in any tiles except water or lava
                        powerUpVector.push_back(Collectible(chunkSize * tileSize * coords[0] + i * tileSize, chunkSize * tileSize * coords[1] + j *tileSize, 3));
                    } else if ((rand() % 499) == 0 && (tilesVals[i][j] == 0 || tilesVals[i][j] == 6)) { //1 in 500 chance of a power up appearing in water or lava
                        powerUpVector.push_back(Collectible(chunkSize * tileSize * coords[0] + i * tileSize, chunkSize * tileSize * coords[1] + j *tileSize, 3));
                    }
                }
            }
        };

        //variables are declared
        int tilesVals[16][16];  //contains tile type for given tile coordinates of chunk
        int coords[2];  //contains chunk coordinates
        static const int chunkSize = 16;    //size of chunks (16 by 16 tiles)
        static const int tileSize = 50;     //size of tiles (50 by 50 pixels)
        float val;  //recieves Simplex Noise value
        float temp; //recieves Simplex Noise temperature value
        ALLEGRO_BITMAP *grassImage; //used to load grass tile image
        ALLEGRO_BITMAP *sandImage;  //used to load sand tile image
        ALLEGRO_BITMAP *waterImage; //used to load water tile image
        ALLEGRO_BITMAP *mudImage;   //used to load mud tile image
        ALLEGRO_BITMAP *lavaImage;  //used to load lava tile image
        ALLEGRO_BITMAP *tileImages[7];  //contains loaded tile images
        std::vector<Obstacle> obstacle1Vector;  //vector for obstacles of type 1
        std::vector<Obstacle> obstacle2Vector;  //vector for obstacles of type 2
        std::vector<Enemy> enemyVector; //vector for enemies
        std::vector<Collectible> coinVector;    //vector for coins
        std::vector<Collectible> fruitVector;   //vector for fruits
        std::vector<Collectible> powerUpVector;  //vector for power ups

        //method recieves x and y position of player
        //the method draws tiles on the screen
        void drawTiles(int posX, int posY) {

            //the offset from the characters position is calculated
            int xOff = coords[0] * tileSize * chunkSize - posX;
            int yOff = coords[1] * tileSize * chunkSize - posY;

            //the tile type and offset from character position are used to draw the tile on the screen
            for (int i = 0; i < chunkSize; i++) {
                for (int j = 0; j < chunkSize; j++) {
                    al_draw_bitmap(tileImages[tilesVals[i][j]], 1280 / 2 + (xOff + i * tileSize), 720 / 2 + (yOff + j * tileSize), 0);
                }
            }
        };

        //method recieves player coordinates and the time which they should be drawn
        //method draws obstacles either below of above the player depending on the draw time
        //this allows the player to pass in front of obstacles or behind obstacles
        void drawObstacles(int posX, int posY, int btmY, int drawTime) {

            for (int i = 0; i < obstacle1Vector.size(); i++) {

                //type 1 obstacles are drawn below the character if drawTime is 1 or above the character if drawTime is 2
                if (obstacle1Vector[i].pos[1] + obstacle1Vector[i].displacement[1] < btmY && drawTime == 1) {
                    obstacle1Vector[i].draw(posX, posY);
                } else if (obstacle1Vector[i].pos[1] + obstacle1Vector[i].displacement[1] >= btmY && drawTime == 2) {
                    obstacle1Vector[i].draw(posX, posY);
                }
            }
            for (int i = 0; i < obstacle2Vector.size(); i++) {

                //type 2 obstacles are drawn below the character if drawTime is 1 or above the character if drawTime is 2
                if (obstacle2Vector[i].pos[1] + obstacle2Vector[i].displacement[1] < btmY && drawTime == 1) {
                    obstacle2Vector[i].draw(posX, posY);
                } else if (obstacle2Vector[i].pos[1] + obstacle2Vector[i].displacement[1] >= btmY && drawTime == 2) {
                    obstacle2Vector[i].draw(posX, posY);
                }
            }

        };

        //method recieves x and y position of player and the frame number
        //method draw coins, fruit, and power ups
        void drawCollectibles(int posX, int posY, int frameNum) {

            //collectibles are drawn using player coordinates
            for (int i = 0; i < coinVector.size(); i++) {
                coinVector[i].draw(posX, posY, frameNum % 10);
            }
            for (int i = 0; i < fruitVector.size(); i++) {
                fruitVector[i].draw(posX, posY, 0);
            }
            for (int i = 0; i < powerUpVector.size(); i++) {
                powerUpVector[i].draw(posX, posY, (frameNum / 2) % 6);
            }
        };

        //method recieves x and y position of player and the frame number
        //method uses these positions to draws enemies
        void drawEnemies(int posX, int posY, int frameNum) {

            //enemies are drawn using player coordinates
            for (int i = 0; i < enemyVector.size(); i++) {
                enemyVector[i].draw(posX, posY, frameNum % 2);
            }
        };
};

#endif
