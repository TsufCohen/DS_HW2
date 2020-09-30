//
// Created by Chen on 05/06/2019.
//

#ifndef DS_WET_2_ROOMSLIST_H
#define DS_WET_2_ROOMSLIST_H

#include "Room.h"
#include  <stdlib.h>
#include <iostream>

struct Node{

    int key;
    Room room;
    Node* next;
    Node* father;
};

class RoomsList{
private:
    int numRooms;
    Node * head;


    Node* findNode(int roomID);


public:


    RoomsList(const Room& r);
    RoomsList(const int roomID);
    ~RoomsList();

    bool addNode(const Room& r);
    bool addNode(const int roomID);
    bool removeNode(const int roomID);
    int getNumRooms()const;
    Room* findRoom(int roomID);

    Node* getFirst();


    //Node& find(const int roomID);

    void printList();


};

#endif //DS_WET_2_ROOMSLIST_H
