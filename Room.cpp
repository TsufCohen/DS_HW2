//
// Created by Chen on 05/06/2019.
//

#include "Room.h"

Room::Room(int id): roomID(id), reservedHours(0){

    try{
        coursesArr = new int[HOURS];
        groupsArr = new int[HOURS];
        numStudentsArr = new int[HOURS];
        for(int i = 0 ;i< HOURS;i++){
           coursesArr[i]= -1;
           groupsArr[i]= -1;
            numStudentsArr[i]= -1;
      }
    }catch(std::bad_alloc& e){
        delete[] coursesArr;
        delete[] groupsArr;
        delete[] numStudentsArr;
        throw std::bad_alloc();
    }
}

Room::Room(const Room& r): roomID(r.roomID), reservedHours(r.reservedHours){

    try {
        coursesArr = new int[HOURS];
        groupsArr = new int[HOURS];
        numStudentsArr =  new int[HOURS];
        for (int i = 0; i < HOURS; i++) {
            coursesArr[i] = r.coursesArr[i];
            groupsArr[i] = r.groupsArr[i];
            numStudentsArr[i]=r.numStudentsArr[i];
        }
    }catch(std::bad_alloc& e){
        delete[] coursesArr;
        delete[] groupsArr;
        delete[] numStudentsArr;

        throw std::bad_alloc();
    }
}

Room::~Room(){

    delete[] coursesArr ;
    delete[] groupsArr ;
    delete[] numStudentsArr;

}

bool Room::isFree(int hour) const{
    if( groupsArr[hour-1]== -1)
        return true;
    return false;
}

void Room::getLecture(int hour, int* course, int* group, int* students) const{
    *course = coursesArr[hour-1];
    *group = groupsArr[hour-1];
    *students = numStudentsArr[hour-1];

}

bool Room::removeLecture(int hour){

    if( coursesArr[hour-1] == -1)
        return false;

    coursesArr[hour-1] = -1;
    groupsArr[hour-1] = -1;
    numStudentsArr[hour-1] = -1;
    reservedHours--;

    return true;
}

int Room::getRoomID() const{
    return roomID;
}

bool Room::addLecture(int hour, int course, int group, int students){

    if( coursesArr[hour-1] != -1)
        return false;

    coursesArr[hour-1] = course;
    groupsArr[hour-1] = group;
    numStudentsArr[hour-1] = students;
    reservedHours++;

    return true;
}

int Room::getReservedHours()const{
    return reservedHours;
}

bool operator==(const Room& r1,const Room& r2){
    if(r1.roomID==r2.roomID)
        return true;
    return false;
}

