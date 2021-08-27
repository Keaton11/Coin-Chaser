#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

extern ALLEGRO_DISPLAY *display;

class Obstacle {
    public:

        //constructor recives x and y coordinate of obstacle, the type of tile, and the type of obstacle
        //constructor loads image of obstacle
        Obstacle(int x, int y, int tileVal, int obstacle) {

            //x and y coordinates of obstacle are saved
            pos[0] = x;
            pos[1] = y;

            //the type of tile is saved
            tileType = tileVal;

            //switch statement loads obstacle image corresponding to the type of tile the obstacle is on
            switch (tileType) {
                case 0:

                    //the seaweed image is loaded and its displacement and collision box size are saved
                    obstacleImage = al_load_bitmap("static/obstacles/seaweed.png");
                    if (obstacleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/obstacles/seaweed.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }

                    displacement[0] = 10;
                    displacement[1] = 35;
                    boxSize[0] = 25;
                    boxSize[1] = 25;
                    break;

                case 1:

                    //the palm tree image is loaded and its displacement and collision box size are saved
                    obstacleImage = al_load_bitmap("static/obstacles/palmTree.png");
                    if (obstacleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/obstacles/palmTree.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }

                    displacement[0] = 15;
                    displacement[1] = 58;
                    boxSize[0] = 15;
                    boxSize[1] = 15;
                    break;

                case 3:

                    //switch statement loads image corresponding to the type of obstacle
                    switch (obstacle) {
                        case 1:

                            //the cactus image is loaded and its displacement and collision box size are saved
                            obstacleImage = al_load_bitmap("static/obstacles/cactus.png");
                            if (obstacleImage == nullptr) {
                                al_show_native_message_box(display, "Error", "static/obstacles/cactus.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                            }

                            displacement[0] = 11;
                            displacement[1] = 55;
                            boxSize[0] = 20;
                            boxSize[1] = 20;
                            break;

                        case 2:

                            //the tumbleweed image is loaded and its displacement and collision box size are saved
                            obstacleImage = al_load_bitmap("static/obstacles/tumbleweed.png");
                            if (obstacleImage == nullptr) {
                                al_show_native_message_box(display, "Error", "static/obstacles/tumbleweed.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                            }

                            displacement[0] = 4;
                            displacement[1] = 7;
                            boxSize[0] = 40;
                            boxSize[1] = 40;
                        break;
                    }
                    break;

                case 2:
                case 4:

                    //switch statement loads image corresponding to the type of obstacle
                    switch (obstacle) {
                        case 1:

                            //the tree image is loaded and its displacement and collision box size are saved
                            obstacleImage = al_load_bitmap("static/obstacles/tree.png");
                            if (obstacleImage == nullptr) {
                                al_show_native_message_box(display, "Error", "static/obstacles/tree.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                            }

                            displacement[0] = 25;
                            displacement[1] = 55;
                            boxSize[0] = 25;
                            boxSize[1] = 25;
                            break;

                        case 2:

                            //the flower image is loaded and its displacement and collision box size are saved
                            obstacleImage = al_load_bitmap("static/obstacles/flower.png");
                            if (obstacleImage == nullptr) {
                                al_show_native_message_box(display, "Error", "static/obstacles/flower.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                            }

                            displacement[0] = 10;
                            displacement[1] = 35;
                            boxSize[0] = 15;
                            boxSize[1] = 15;
                        break;
                    }
                    break;

                case 5:

                    //the rock image is loaded and its displacement and collision box size are saved
                    obstacleImage = al_load_bitmap("static/obstacles/rock.png");
                    if (obstacleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/obstacles/rock.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }

                    displacement[0] = 2;
                    displacement[1] = 0;
                    boxSize[0] = 45;
                    boxSize[1] = 45;
                    break;

             }
        };

        //variables are declared
        int pos[2]; //x and y positions
        int displacement[2];    //image collision box displacement
        int boxSize[2]; //size of obstacle collision box
        int tileType;   //type of tile the obstacle is on
        ALLEGRO_BITMAP *obstacleImage;  //loads obstacle image


        //method recieves x and y positon of the obstacle and the frameNum
        //obstacle is drawn at its position on the screen
        void draw(int posX, int posY) {

            //obstacle position on the screen is determined
            int x = 1280 / 2 + pos[0] - posX;
            int y = 720 / 2 + pos[1] - posY;

            //obstacle is displayed on the screen
            al_draw_bitmap(obstacleImage, x, y, 0);
        };
};

#endif

