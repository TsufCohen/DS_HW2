//
// Created by Chen on 05/06/2019.
//

#ifndef DS_WET_2_ROOM_H
#define DS_WET_2_ROOM_H

#define HOURS 10

//#include <stdlib.h>
#include <iostream>
#include <stdbool.h>


class Room {

private:
    int roomID;
    int reservedHours; //the num of reserved hours .
    int* coursesArr;// מס הקורס שיש להם הרצאות בחדר לפי שעות
    int* groupsArr;// מס הקורס שיש להם הרצאות בחדר לפי שעות
    int* numStudentsArr;


public:

    Room(int id);
    Room(const Room& r);
    ~Room();

    bool isFree(int hour) const;
    void getLecture(int hour, int* course, int* group, int* students) const;
    bool removeLecture(int hour);
    int getRoomID() const;
    bool addLecture(int hour, int course, int group, int students);
    int getReservedHours() const;

    friend bool operator==(const Room& r1,const Room& r2);



};





#endif //DS_WET_2_ROOM_H
