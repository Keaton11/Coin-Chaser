#ifndef ENEMY_H
#define ENEMY_H

#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

extern ALLEGRO_DISPLAY *display;

class Enemy {
    public:

        //constructor recives x and y coordinate of enemy, and the type of tile
        //constructor loads images of enemy
        Enemy(int x, int y, int tile) {

            //x and y coordinates of enemy are saved
            pos[0] = x;
            pos[1] = y;

            //the type of tile is saved
            tileType = tile;

            //enemy speed is set to 3
            speed = 3;

            //switch statement loads enemy image corresponding to the type of tile the enemy is on
            switch (tileType) {
                case 0:

                    //the water enemy images are loaded and their collision box size is saved
                    enemyImage[0] = al_load_bitmap("static/entities/enemies/waterEnemy1.png");
                    if (enemyImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/entities/enemies/waterEnemy1.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }

                    enemyImage[1] = al_load_bitmap("static/entities/enemies/waterEnemy2.png");
                    if (enemyImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/entities/enemies/waterEnemy2.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }

                    boxSize[0] = 45;
                    boxSize[1] = 60;
                    break;

                case 1:
                case 3:

                    //the sand enemy images are loaded and their collision box size is saved
                    enemyImage[0] = al_load_bitmap("static/entities/enemies/sandEnemy1.png");
                    if (enemyImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/entities/enemies/sandEnemy1.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }

                    enemyImage[1] = al_load_bitmap("static/entities/enemies/sandEnemy2.png");
                    if (enemyImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/entities/enemies/sandEnemy2.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }

                    boxSize[0] = 50;
                    boxSize[1] = 45;
                    break;

                case 2:
                case 4:

                    //the grass enemy images are loaded and their collision box size is saved
                    enemyImage[0] = al_load_bitmap("static/entities/enemies/grassEnemy1.png");
                    if (enemyImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/entities/enemies/grassEnemy1.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }

                    enemyImage[1] = al_load_bitmap("static/entities/enemies/grassEnemy2.png");
                    if (enemyImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/entities/enemies/grassEnemy2.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }

                    boxSize[0] = 50;
                    boxSize[1] = 50;
                    break;

                case 5:

                    //the mud enemy images are loaded and their collision box size is saved
                    enemyImage[0] = al_load_bitmap("static/entities/enemies/mudEnemy1.png");
                    if (enemyImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/entities/enemies/mudEnemy1.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }

                    enemyImage[1] = al_load_bitmap("static/entities/enemies/mudEnemy2.png");
                    if (enemyImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/entities/enemies/mudEnemy2.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }

                    boxSize[0] = 60;
                    boxSize[1] = 36;
                    break;

                case 6:

                    //the lava enemy images are loaded and their collision box size is saved
                    enemyImage[0] = al_load_bitmap("static/entities/enemies/lavaEnemy1.png");
                    if (enemyImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/entities/enemies/lavaEnemy1.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }

                    enemyImage[1] = al_load_bitmap("static/entities/enemies/lavaEnemy2.png");
                    if (enemyImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/entities/enemies/lavaEnemy2.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }

                    boxSize[0] = 50;
                    boxSize[1] = 55;
                    break;
            }

            //enemy sound effect is loaded as a sample instance
            al_reserve_samples(1);
            enemy_sound = al_load_sample("sounds/enemy.wav");
            if (!enemy_sound) {
                al_show_native_message_box(display, "Error", "Error", "Failed to initialize enemy_sound!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
            }
            enemy_sound_instance = al_create_sample_instance(enemy_sound);
            al_attach_sample_instance_to_mixer(enemy_sound_instance, al_get_default_mixer());
        };

        //variables are declared
        int tileType;   //type of tile the enemy is on
        int pos[2]; //x and y positions
        int boxSize[2]; //size of enemy collision box
        int speed;  //speed of enemy
        ALLEGRO_BITMAP *enemyImage[2];  //loads all enemy images
        ALLEGRO_SAMPLE *enemy_sound;    //sound effect when player is hit by enemy
        ALLEGRO_SAMPLE_INSTANCE *enemy_sound_instance;  //enemy sound instance plays sound effect when an enemy hits the player

        //method recieves x and y positon of the enemy and the frameNum
        //enemy is drawn at its position on the screen
        void draw(int posX, int posY, int frameNum) {

            //enemy position on the screen is determined
            int x = 1280 / 2 + pos[0] - posX;
            int y = 720 / 2 + pos[1] - posY;

            //enemy is displayed on the screen
            al_draw_bitmap(enemyImage[frameNum], x, y, 0);
        };
};

#endif
