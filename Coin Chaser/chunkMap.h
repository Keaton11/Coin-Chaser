#ifndef CHUNKMAP_H
#define CHUNKMAP_H

#include <stdio.h>
#include <map>

#include <allegro5/allegro.h>

#include "chunk.h"

//class stores chunks using maps
class ChunkMap {
    private:

        //variables are declared
        std::map<int, std::map<int, Chunk>> tileMap;    //map of map to store tiles in each chunk
        int seed;  //seed which determines how the dynamically generated background will look

    public:

        //constructor randomizes the seed to generate a different dynamically generated background
        ChunkMap() {
            seed = rand() % 9999 + 1;
        };

        //method recieves chunks
        //chunks are stored in a chunkmap
        void store(Chunk c) {

            //if a map for the given y coordinate is present, the chunk is stored under this map and is paired with the given x coordinate
            if (tileMap.count(c.coords[1])) {
                tileMap[c.coords[1]].insert(std::make_pair(c.coords[0], c));

            //if a map for the given y coordinate is not present, a map is created for the y cordinate
            //the chunk is stored under this map and is paired with the given x coordinate
            } else {
                std::map<int, Chunk> buffer;
                tileMap[c.coords[1]] = buffer;
                tileMap[c.coords[1]].insert(std::make_pair(c.coords[0], c));
            }
        };

        //method recieves x and y position of chunk
        //method determines if it has already been stored
        //true is returned if a chunk has been stored and false is returned if one hasn't
        bool isStored(int x, int y) {

            //if statements determines whether map has been created for the given chunk coordinates
            if (tileMap.count(y) && tileMap[y].count(x)) {
                return true;
            } else {
                return false;
            }
        };

        //method recieves x and y positions of a chunk
        //x and y positions are used to fetch the given chunk
        //the chunk at that position is returned
        Chunk* fetch(int x, int y) {

            //the chunk for the given x and y positions is fetched
            return &(tileMap.find(y)->second.find(x)->second);
        };

        //method recieves x and y positions of a chunk
        //method either fetches or generates a chunk
        //the chunk at that position is returned
        Chunk* fetchOrGen(int x, int y) {

            //if the chunk is already stored, the chunk is fetched
            if (this->isStored(x, y)){
                return this->fetch(x, y);

            //if the chunk has not yet been stored, the chunk is generated, then stored
            } else {
                Chunk c(x, y, seed);
                this->store(c);
                return this->fetch(x, y);
            }
        };
};

#endif
