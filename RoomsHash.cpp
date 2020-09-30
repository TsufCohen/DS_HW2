//
// Created by Chen on 09/06/2019.
//

#include "RoomsHash.h"

//give the index of the room in the array
int hashFunc(int roomID, int size){
    return roomID%size;
}

RoomsHash::RoomsHash(): numRooms(0), size(3){

    try {
        rooms = new RoomsList *[3];
        for (int i = 0; i < 3; i++) {
            rooms[i] = nullptr;
        }
    }catch(std::bad_alloc& e){
        throw std::bad_alloc();
    }
}

RoomsHash::~RoomsHash(){

    for(int i=0; i<size;i++){
        delete rooms[i];
    }
    delete [] rooms;
}

//change the size of the array
void RoomsHash::changeArr(int old){

        RoomsList **temp = new RoomsList *[size];
        for (int i = 0; i < size; i++) {
            temp[i] = nullptr;
        }

        for (int i = 0; i < old; i++) {
            if (rooms[i] == nullptr)
                continue;
            Node *ptr = rooms[i]->getFirst();
            while (ptr != nullptr) {

                int index = hashFunc(ptr->key, size);
                if (temp[index] == nullptr) {

                    try {
                        temp[index] = new RoomsList(ptr->room);
                    }catch(std::bad_alloc& e){
                        throw std::bad_alloc();
                    }

                }else {
                    temp[index]->addNode(ptr->room);
                }
                ptr = ptr->next;
            }
        }

        for (int i = 0; i < old; i++) {
            delete rooms[i];
        }
        delete[] rooms;
        rooms = temp;


}
//decrease the array's size
void RoomsHash::decreaseArr() {

    int old = size;
    if (size % 2 != 0)
        size = size/2 + 1;
    else
        size= size/2;

    try {
        changeArr(old);
    }catch(std::bad_alloc& e){
        throw std::bad_alloc();
    }
}
//increase the array's size
void RoomsHash::increaseArr() {

    int old = size;
    size*= 2;
    try {
        changeArr(old);
    }catch(std::bad_alloc& e){
        throw std::bad_alloc();
    }
}

bool RoomsHash::insert(const int roomID){

    int index = hashFunc(roomID, size);
    if(rooms[index] == nullptr){
        numRooms++;
        try{
            rooms[index] = new RoomsList(roomID);
        }catch(std::bad_alloc& e){
            throw std::bad_alloc();
        }
        return true;
    }

    if(rooms[index]->addNode(roomID)== true){
        numRooms++;
        if(double(numRooms/size)>=2) {
            increaseArr();
        }
        return true;
    }
    return false;
}

bool RoomsHash::remove(const int roomID){

    int index = hashFunc(roomID, size);
    if(rooms[index] == nullptr){
        return false;
    }

    if(rooms[index]->removeNode(roomID)== true){
        numRooms--;
        /////need to check if the cast is ok !!!
        if(double(numRooms/size)<= 0.5 )// need to decrese the array
            decreaseArr();
        return true;
    }
    return false;
}

Room* RoomsHash::find(const int roomID){
    int index = hashFunc(roomID,size);
    if(rooms[index] == nullptr)
        return nullptr;
    return rooms[index]->findRoom(roomID);
}