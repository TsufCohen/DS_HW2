//
// Created by Chen on 05/06/2019.
//
#include "RoomsList.h"
#include <iostream>
using std::cout;
using std::endl;


RoomsList::RoomsList(const Room& r): numRooms(1){

    try {
        head = new Node{r.getRoomID(), r, nullptr, nullptr};
    }catch(std::bad_alloc& e){
        throw std::bad_alloc();
    }

}
RoomsList::RoomsList(const int roomID): numRooms(1){

    try {
        head = new Node{roomID, Room(roomID), nullptr, nullptr};
    }catch(std::bad_alloc& e){
        throw std::bad_alloc();
    }
}
RoomsList::~RoomsList(){

    while(head != nullptr){
        Node* ptr = head;
        head =  head->next;
        delete ptr;
    }
}

Node* RoomsList::findNode(int roomID){

    Node* ptr = head;
    while(ptr!= nullptr){
        if(ptr->key == roomID)
            return ptr;

        ptr = ptr->next;
    }
    return nullptr;
}

bool RoomsList::addNode(const Room& r){

    if(findNode(r.getRoomID())!= nullptr)
        return false;

    Node* temp = head;

    try {
        head = new Node{r.getRoomID(), r, temp, nullptr};
    }catch(std::bad_alloc& e){
        throw std::bad_alloc();
    }
    if(temp)
        temp->father = head;
    numRooms++;
    /*
    Node ptr = createNode(r);
    ptr.next = head;
    head->father = &ptr;
    *head = ptr;
     */
    return true;
}
bool RoomsList::addNode(const int roomID){

    return addNode(Room(roomID));
}

bool RoomsList::removeNode(const int roomID){

    Node * toRemove = findNode(roomID);
    if((toRemove == nullptr)|| toRemove->room.getReservedHours() != 0) {
        return false;
    }

    if(toRemove->father == nullptr){
        head = toRemove->next;
    }
    else{
        toRemove->father->next = toRemove->next;
    }
    if(toRemove->next != nullptr)
        toRemove->next->father = toRemove->father;

    delete toRemove;
    numRooms--;
    return true;
}
int RoomsList::getNumRooms()const{
    return numRooms;
}
Room* RoomsList::findRoom(int roomID){

    Node* node = findNode(roomID);
    if(node != nullptr)
        return &(node->room);

    return nullptr;
}

Node* RoomsList::getFirst(){
    return head;
}

/// for testss
void RoomsList::printList(){

    Node* curr = head;
    while(curr != nullptr){
        cout<<curr->key<<" ";
        curr = curr->next;
    }
    cout<<endl;
}

