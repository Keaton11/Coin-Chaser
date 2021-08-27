#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

extern ALLEGRO_DISPLAY *display;

class Collectible {
    public:

        //constructor recives x and y coordinate of collectible, and the type of collectible
        //constructor loads image(s) of collectible
        Collectible(int x, int y, int collectible) {

            //x and y coordinates of collectible are saved
            pos[0] = x;
            pos[1] = y;

            //the type of collectible is saved
            type = collectible;

            //switch statement loads image corresponding to the type of collectible
            switch (type) {
                case 1:

                    //coin images are saved under an Allegro bitmap array
                    //the displacement of each coin is saved
                    collectibleImage[0] = al_load_bitmap("static/collectibles/coin1.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/collectibles/coin1.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[0] = 0;

                    collectibleImage[1] = al_load_bitmap("static/collectibles/coin2.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/collectibles/coin2.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[1] = 0;

                    collectibleImage[2] = al_load_bitmap("static/collectibles/coin3.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/collectibles/coin3.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[2] = 2.5;

                    collectibleImage[3] = al_load_bitmap("static/collectibles/coin4.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/collectibles/coin4.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[3] = 6;

                    collectibleImage[4] = al_load_bitmap("static/collectibles/coin5.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/collectibles/coin5.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[4] = 13.5;

                    collectibleImage[5] = al_load_bitmap("static/collectibles/coin6.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/collectibles/coin6.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[5] = 20;

                    collectibleImage[6] = al_load_bitmap("static/collectibles/coin7.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/collectibles/coin7.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[6] = 13.5;

                    collectibleImage[7] = al_load_bitmap("static/collectibles/coin8.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/collectibles/coin8.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[7] = 6;

                    collectibleImage[8] = al_load_bitmap("static/collectibles/coin9.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/collectibles/coin9.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[8] = 2.5;

                    collectibleImage[9] = al_load_bitmap("static/collectibles/coin10.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/collectibles/coin10.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[9] = 0;

                    //coin sound effect is loaded as a sample instance
                    al_reserve_samples(1);
                    coin_sound = al_load_sample("sounds/coin.wav");
                    if (!coin_sound) {
                        al_show_native_message_box(display, "Error", "Error", "Failed to initialize coin_sound!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    coin_sound_instance = al_create_sample_instance(coin_sound);
                    al_attach_sample_instance_to_mixer(coin_sound_instance, al_get_default_mixer());
                    break;

                case 2:

                    //fruit image is saved under as an Allegro bitmap
                    //the displacement of the fruit image is saved
                    collectibleImage[0] = al_load_bitmap("static/collectibles/fruit.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/collectibles/fruit.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[0] = 0;
                    break;

                case 3:

                    //power up images are saved under an Allegro bitmap array
                    //the displacement of each power up is saved
                    collectibleImage[0] = al_load_bitmap("static/collectibles/powerUp1.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/powerUp1.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[0] = 0;

                    collectibleImage[1] = al_load_bitmap("static/collectibles/powerUp2.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/powerUp2.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[1] = 0;

                    collectibleImage[2] = al_load_bitmap("static/collectibles/powerUp3.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/powerUp3.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[2] = 0;

                    collectibleImage[3] = al_load_bitmap("static/collectibles/powerUp4.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/powerUp4.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[3] = 0;

                    collectibleImage[4] = al_load_bitmap("static/collectibles/powerUp5.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/powerUp5.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[4] = 0;

                    collectibleImage[5] = al_load_bitmap("static/collectibles/powerUp6.png");
                    if (collectibleImage == nullptr) {
                        al_show_native_message_box(display, "Error", "static/powerUp6.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                    }
                    displacement[5] = 0;
                    break;
            }
        };

        //variables are declared
        int pos[2]; //x and y positions
        int type;   //type of collectible (coin, fruit or power up)
        float displacement[10]; //image collision box displacement
        ALLEGRO_BITMAP *collectibleImage[10]; //loads all collectible images
        ALLEGRO_SAMPLE *coin_sound; //sound effect when coin is collected
        ALLEGRO_SAMPLE_INSTANCE *coin_sound_instance;   //coin sound instance is used to play coin sound effect

        //method recieves x and y positon of the collectible and the frameNum
        //collectible is drawn at its position on the screen
        void draw(int posX, int posY, int frameNum) {

            //collectible position on the screen is determined
            int x = 1280 / 2 + pos[0] - posX + displacement[frameNum];
            int y = 720 / 2 + pos[1] - posY;

            //collectible is displayed on the screen
            al_draw_bitmap(collectibleImage[frameNum], x, y, 0);
        };
};

#endif
