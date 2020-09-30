//
// Created by Chen on 09/06/2019.
//

#ifndef DS_WET_2_ROOMSHASH_H
#define DS_WET_2_ROOMSHASH_H

#include "RoomsList.h"
#include <iostream>

class RoomsHash {
private:

    RoomsList** rooms;
    int numRooms;
    int size;
    void decreaseArr();
    void increaseArr();
    void changeArr(int old);

public:

    RoomsHash();
    ~RoomsHash();

    bool insert(const int roomID);
    bool remove(const int roomID);
    Room* find(const int roomID); //// check if reference is ok or return a pointer

    ///
    int getSize(){return size;}

};


#endif //DS_WET_2_ROOMSHASH_H
