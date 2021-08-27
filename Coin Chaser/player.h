#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "chunk.h"

extern ALLEGRO_DISPLAY *display;

//class updates the player's chunk position, loads the character, and draws the character
class Player {
    public:
        //constructor sets power up time to zero
        Player(){
            powerUpTime = 0;
        };

        //variables are declared
        int posX;
        int posY;
        int chunkCoordsX;
        int chunkCoordsY;
        int chunkTileX;
        int chunkTileY;
        int powerUpTime;
        ALLEGRO_BITMAP *player[9][5];
        ALLEGRO_SAMPLE *lava_sound;
        ALLEGRO_SAMPLE_INSTANCE *lava_sound_instance;

        //method updates the player's chunk coordinates and chunk number
        void updateChunk() {

            //chunk coordinates and chunk tiles are updated based on the players position
            chunkCoordsX = std::floor(float(posX) / (Chunk::chunkSize * Chunk::tileSize));
            chunkCoordsY = std::floor(float(posY) / (Chunk::chunkSize * Chunk::tileSize));
            chunkTileX = abs((abs(chunkCoordsX * Chunk::chunkSize * Chunk::tileSize) - abs(posX)) / Chunk::tileSize);
            chunkTileY = abs((abs(chunkCoordsY * Chunk::chunkSize * Chunk::tileSize) - abs(posY)) / Chunk::tileSize);
        };

        //method recieves the allegro display and loads character images as well as the lava sound effect
        //true returns if loading is successful, and false is returned if it is not
        bool load(ALLEGRO_DISPLAY *display) {

            //sprite sheet containing character images is loaded as an Allegro bitmap and clipped into smaller bitmaps
            player[0][0] = al_load_bitmap("static/entities/spriteSheet.png");
            if (player[0][0] == nullptr) {
                al_show_native_message_box(display, "Error", "static/entities/spriteSheet.png", "Could not load ", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                return false;
            }
            for (int i = 1; i < 9; i++) {
                for (int j = 0; j < 5; j++) {
                    player[i][j] = al_create_sub_bitmap(player[0][0], j * 64, (i - 1) * 64, 64, 64);
                }
            }

            //lava sample instance is loaded
            al_reserve_samples(1);
            lava_sound = al_load_sample("sounds/lava.wav");
            if (!lava_sound) {
                al_show_native_message_box(display, "Error", "Error", "Failed to initialize lava_sound!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                return false;
            }
            lava_sound_instance = al_create_sample_instance(lava_sound);
            al_attach_sample_instance_to_mixer(lava_sound_instance, al_get_default_mixer());
            return true;
        };

        //method recieves the direction the player is travelling, reset, the frame number, and the power up time
        //character image is drawn at the center of the screen facing the appropriate direction
        void draw(int dx, int dy, bool reset, int playerImagePos) {

            //if reset is true, the characters last position is set to 1
            static int drawnLast;
            if (reset) {
                drawnLast = 1;
            }

            //based on the direction the character is travelling and the frame number, a clipped image from the sprite sheet is displayed to simulate walking
            if (dx == 0 && dy == 1) {
                al_draw_bitmap(player[1][playerImagePos], (1280 / 2) - 32, (720 / 2) - 32, 0);
                drawnLast = 1;
            } else if (dx == -1 && dy == 1) {
                al_draw_bitmap(player[2][playerImagePos], (1280 / 2) - 32, (720 / 2) - 32, 0);
                drawnLast = 2;
            } else if (dx == -1 && dy == 0) {
                al_draw_bitmap(player[3][playerImagePos], (1280 / 2) - 32, (720 / 2) - 32, 0);
                drawnLast = 3;
            } else if (dx == -1 && dy == -1) {
                al_draw_bitmap(player[4][playerImagePos], (1280 / 2) - 32, (720 / 2) - 32, 0);
                drawnLast = 4;
            } else if (dx == 0 && dy == -1) {
                al_draw_bitmap(player[5][playerImagePos], (1280 / 2) - 32, (720 / 2) - 32, 0);
                drawnLast = 5;
            } else if (dx == 1 && dy == -1) {
                al_draw_bitmap(player[6][playerImagePos], (1280 / 2) - 32, (720 / 2) - 32, 0);
                drawnLast = 6;
            } else if (dx == 1 && dy == 0) {
                al_draw_bitmap(player[7][playerImagePos], (1280 / 2) - 32, (720 / 2) - 32, 0);
                drawnLast = 7;
            } else if (dx == 1 && dy == 1) {
                al_draw_bitmap(player[8][playerImagePos], (1280 / 2) - 32, (720 / 2) - 32, 0);
                drawnLast = 8;
            } else {
                //if no keys are being pressed, the character faces the direction they were last facing
                al_draw_bitmap(player[drawnLast][2], (1280 / 2) - 32, (720 / 2) - 32, 0);
            }

        };

        //method recieves the players x and y position, as well as the direction they are travelling
        //the players collsion box is determined and returned
        entityPos updatePos(int posX, int posY, int dx, int dy){

            //the player's collision box is created
            entityPos playerPos;
            playerPos.leftX = posX + dx + 30;
            playerPos.topY = posY + dy + 39;
            playerPos.rightX = posX + dx + 70;
            playerPos.btmY = posY + dy + 79;
            playerPos.center[0] = (playerPos.rightX + playerPos.leftX) / 2;
            playerPos.center[1] = (playerPos.topY + playerPos.btmY) / 2;
            return playerPos;
        };

        //method recieves chunkmap, power up, and player
        //the character's speed is calculated and returned as an integer
        int speed(ChunkMap &chunkMap, bool powerUp, Player &player) {

            //if a power up is on, the player's speed is set to 10 for 30 seconds
            int speed;
            if (powerUp) {  //if a power up has been activated
                player.powerUpTime = 30;
                speed = 10;
            } else if (player.powerUpTime > 0) {    //if a power up was alread activated
                speed = 10;
            } else {    //if no power up is being acivated or has been activated within 30 seconds
                speed = 5;
            }

            //if the player is standing on a water tile, their speed is multiplied by 0.8
            if (chunkMap.fetchOrGen(chunkCoordsX, chunkCoordsY)->tilesVals[chunkTileX][chunkTileY] == 0) {
                speed *= 0.8;
            }
            return speed;
        };

        //method recieves chunkmap and the player's health
        //the amount of health the character has lost if they entered lava is determined
        //the amount of health lost is returned as an integer
        int lava(ChunkMap &chunkMap, int health) {

            //if the player is standing in lava, a lava sound effect is played, and 1 is returned
            if (chunkMap.fetchOrGen(chunkCoordsX, chunkCoordsY)->tilesVals[chunkTileX][chunkTileY] == 6 && health == 0) {
                al_stop_sample_instance(lava_sound_instance);
                return 1;
            } else if (chunkMap.fetchOrGen(chunkCoordsX, chunkCoordsY)->tilesVals[chunkTileX][chunkTileY] == 6) {
                if (!al_get_sample_instance_playing(lava_sound_instance)) {
                    al_play_sample_instance(lava_sound_instance);
                }
                return 1;
            } else {
                al_stop_sample_instance(lava_sound_instance);
                return 0;
            }
        };

        //method recieves chunkmap, player, health, power up, and the regular font
        //method diplays the event that are present onto the screen
        void displayEffects(ChunkMap &chunkMap, Player &player, int health, bool powerUp, ALLEGRO_FONT *regular_font) {

            //variables are declared
            int screenOffset = 0;
            char powerUpString[5];
            char powerUpTime[30];

            //if the player has a power up, the seconds remaining of the power up is added to a string
            sprintf(powerUpString, "%d seconds", player.powerUpTime);
            strcpy(powerUpTime, "Power Up: 2x Speed for ");
            strcat(powerUpTime, powerUpString);

            //if the player has a power up, is standing in water, or is standing in lava, the effects are displayed on the screen
            if (speed(chunkMap, powerUp, player) > 5) {
                al_draw_text(regular_font, al_map_rgb(255, 255, 255), 33, 69, ALLEGRO_ALIGN_LEFT, powerUpTime);
                screenOffset = 40;
            }
            if (this->speed(chunkMap, powerUp, player) % 4 == 0) {
                al_draw_text(regular_font, al_map_rgb(255, 255, 255), 33, 69 + screenOffset, ALLEGRO_ALIGN_LEFT, "Water: -20% Speed");
            } else if (this->lava(chunkMap, health) == 1) {
                al_draw_text(regular_font, al_map_rgb(255, 255, 255), 33, 69 + screenOffset, ALLEGRO_ALIGN_LEFT, "Lava: -5% Health/Second");
            }
        };
};

#endif
