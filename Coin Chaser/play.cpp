#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "header.h"
#include "chunk.h"
#include "chunkMap.h"
#include "player.h"

//this function recieves the Allegro event queue, the regular font, the title font, and the allegro display
//this function displays the tiles, obstacles, collectibles as well as the player on the screen and allows the player to move
//this function returns true if the play wants to play again and false if they do not
bool play(ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *regular_font, ALLEGRO_FONT *title_font, ALLEGRO_DISPLAY *display) {

    //variables are declared
    int dx = 0; //difference in x position
    int dy = 0; //difference in y position
    int coins = 0;  //counts the number of coins the player has collected
    int r = 2;  //chunk map render radius
    int frameNum;   //counts the number of frames
    int health = 100;   //player's health percentage
    int healthBuffer;   //used to check if player lost any health
    char death[10]; //determine's player's cause of death based on last health loss
    bool powerUp = false;   //determines whether player has a power up on
    bool reset = true;  //determines is game has been reset
    bool doExit = false;    //used in loop to continue loop until otherwise instructed
    bool playAgain = false; //determines whether game will be played again
    ChunkMap chunkMap;  //chunk map
    entityPos playerPos;    //player position coordinates
    Player player;  //used for drawing playing and moving player

    //the character is loaded
    player.posX = 0;
    player.posY = 0;
    if (!player.load(display)) {
        doExit = true;
    }

    while (!doExit) {

        //Allegro event recieves player's key inputs and mouse clicks
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {

            //the player's chunk coordinates are updated
            player.updateChunk();

            //collision with power ups is checked
            if (collectibleCollision(r, 3, playerPos, chunkMap, player.chunkCoordsX, player.chunkCoordsY) > 0) {
                powerUp = true;
            } else {
                powerUp = false;
            }

            //player position is updated
            playerPos = player.updatePos(player.posX, player.posY, dx * player.speed(chunkMap, powerUp, player), dy * player.speed(chunkMap, powerUp, player));
            healthBuffer = health;

            //collision with obstacles and enemies is checked
            playerPos = obstacleCollision(r, playerPos, chunkMap, player.chunkCoordsX, player.chunkCoordsY);
            playerPos = enemyCollision(r, playerPos, chunkMap, player.chunkCoordsX, player.chunkCoordsY, health);

            //the cause of death is updated if the player was hit by an enemy
            if (healthBuffer != health) {
                strcpy(death, "enemy");
            }

            //player position is updated based on collisions, and enemies are moved towards the player
            player.posX = playerPos.leftX - 30;
            player.posY = playerPos.topY - 39;
            if (frameNum > 5) {
                moveEnemies(r, playerPos, chunkMap, player.chunkCoordsX, player.chunkCoordsY);
            }

            //new chunks are genereated, allowing obstacles below the player, enemies and collectibles to be displayed on the screen
            for (int i = 0; i < 2; i++) {
                for (int j = player.chunkCoordsX - r + 1; j < player.chunkCoordsX + r ; j++) {
                    for (int k = player.chunkCoordsY - r + 1; k < player.chunkCoordsY + r; k++) {
                        if (i == 0) {
                            chunkMap.fetchOrGen(j, k)->drawTiles(player.posX, player.posY);
                        } else {
                            chunkMap.fetch(j, k)->drawCollectibles(player.posX, player.posY, frameNum);
                            chunkMap.fetch(j, k)->drawEnemies(player.posX, player.posY, frameNum / 3);
                            chunkMap.fetch(j, k)->drawObstacles(player.posX, player.posY, playerPos.btmY - 64, 1);
                        }
                    }
                }
            }

            //the player and the obstacles above the player are displayed on the screen
            player.draw(dx, dy, reset, displayPlayerPos(frameNum, player.powerUpTime));
            for (int i = player.chunkCoordsX - r + 1; i < player.chunkCoordsX + r ; i++){
                for (int j = player.chunkCoordsY - r + 1; j < player.chunkCoordsY + r; j++){
                    chunkMap.fetch(i, j)->drawObstacles(player.posX, player.posY, playerPos.btmY - 64, 2);
                }
            }

            //the players health and coin count is updated based on collectible collision with coins and fruits
            coins += collectibleCollision(r, 1, playerPos, chunkMap, player.chunkCoordsX, player.chunkCoordsY);
            health += 5 * collectibleCollision(r, 2, playerPos, chunkMap, player.chunkCoordsX, player.chunkCoordsY);

            //if the players health goes above 100 from collecting fruit, it is reduced to 100
            if (health > 100) {
                health = 100;
            }

            //the players health is updated if the player has travelled through lava
            if (frameNum % 4 == 0) {
                healthBuffer = health;
                health -= player.lava(chunkMap, health);

                //the cause of death is updated if the player was in a lava tile
                if (healthBuffer != health) {
                    strcpy(death, "lava");
                }
            }

            //the player's progress and any effects are displayed on the screen
            displayProgress(health, coins, regular_font);
            player.displayEffects(chunkMap, player, health, powerUp, regular_font);

            //if the player has a power up, it's time is reduced by one every second
            if (player.powerUpTime > 0 && frameNum % 20 == 0) {
                player.powerUpTime--;
            }

            //the frame number is updated, and displayed on the screen
            frameNum = displayTimer(regular_font, reset);
            al_flip_display();

            //if the player has lost all their health, the game over screen is displayed
            if (health == 0) {
                playAgain = gameOver(event_queue, regular_font, title_font, death);
                doExit = true;

            //if the player has collected 20 coins, the highscore screen is displayed
            } else if (coins == 20) {
                playAgain = highscoreMenu(frameNum / 20, display, regular_font, title_font, event_queue);
                doExit = true;
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {

            //the character's position is adjusted based on keys pressed down by user
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT:
                case ALLEGRO_KEY_A:
                    dx = -1;
                    break;
                case ALLEGRO_KEY_UP:
                case ALLEGRO_KEY_W:
                    dy = -1;
                    break;
                case ALLEGRO_KEY_RIGHT:
                case ALLEGRO_KEY_D:
                    dx = 1;
                    break;
                case ALLEGRO_KEY_DOWN:
                case ALLEGRO_KEY_S:
                    dy = 1;
                    break;
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {

            //the character's position is adjusted based on keys lifted up by user
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT:
                case ALLEGRO_KEY_A:
                    dx = 0;
                    break;
                case ALLEGRO_KEY_UP:
                case ALLEGRO_KEY_W:
                    dy = 0;
                    break;
                case ALLEGRO_KEY_RIGHT:
                case ALLEGRO_KEY_D:
                    dx = 0;
                    break;
                case ALLEGRO_KEY_DOWN:
                case ALLEGRO_KEY_S:
                    dy = 0;
                    break;
            }

        //the play function is closed if the user has clicked the close button, the exit button, or the restart button
        } else if (button(ev, 1180, 1250, 650, 690) || checkExit(ev)) {
            return false;
        } else if (button(ev, 1120, 1250, 600, 640)) {
            return true;
        }
    }

    //true is returned if the game will be played again and false is returned if it will not
    if (playAgain) {
        return true;
    }
    return false;
}
