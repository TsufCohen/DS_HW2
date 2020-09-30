//
// Created by Chen on 14/06/2019.
//
#include "UGschedule.h"

UGschedule::UGschedule(int n):n(n), courses(n), rooms(){}

int UGschedule::getN() const{
    return this->n;
}

StatusType UGschedule::addRoom(int roomID) {

    if (rooms.find(roomID))
        return FAILURE;

    try {
        rooms.insert(roomID);
        return SUCCESS;

    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }

}

StatusType UGschedule::deleteRoom(int roomID){

    Room* r = rooms.find(roomID);
    if(!r || r->getReservedHours()!=0)
        return FAILURE;

    rooms.remove(roomID);
    return SUCCESS;
}

StatusType UGschedule::addLecture(int courseID, int groupID, int roomID, int hour, int numStudents) {

    Room* r= rooms.find(roomID);
    if(!r || !(r->isFree(hour)))
        return FAILURE;

    Course* c = courses.findCourse(courseID);
    try{
        if(c->AddLecture(groupID, roomID, hour, numStudents)){
            r->addLecture(hour, courseID, groupID, numStudents);
            return SUCCESS;
        }
        else return FAILURE;

    }catch(std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
}

StatusType UGschedule::deleteLecture(int hour, int roomID){

    Room* r= rooms.find(roomID);
    if(!r || r->isFree(hour))
        return FAILURE;

    int courseID, groupID, numStudents;
    r->getLecture(hour, &courseID, &groupID, &numStudents);
    r->removeLecture(hour);
    Course* c = courses.findCourse(courseID);
    c->DeleteLecture(groupID, roomID, hour, numStudents);
    return SUCCESS;
}

StatusType UGschedule::mergeCourses(int courseID1, int courseID2){

    try {
        if (courses.unionC(courseID1, courseID2))
            return SUCCESS;
        else
            return FAILURE;
    }catch (std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
}
StatusType UGschedule::competition(int courseID1, int courseID2, int numGroups, int * winner){

    Course* c1 = courses.findCourse(courseID1);
    Course* c2 = courses.findCourse(courseID2);

    if(c1 == c2)
        return FAILURE;

    int count1 = c1->JoinTheCompetition(numGroups);
    int count2 = c2->JoinTheCompetition(numGroups);

    if(count1 > count2){
        *winner =courseID1;

    }else if(count1 < count2){
        *winner =courseID2;
    }
    else if(courseID1 > courseID2){
        *winner =courseID1;
    } else{
        *winner =courseID2;
    }
    return SUCCESS;
}

///////////////////////////////TO DO !!!!!!!/////////////////
/// NEED TO FINISH THE INTERNAL FUNC : c->getAvr();
/////////////////////
StatusType UGschedule::getAverageStudentsInCourse(int hour, int roomID, float* average){

    Room* r= rooms.find(roomID);
    if(!r || r->isFree(hour))
        return FAILURE;

    int courseID, groupID, students;
    r->getLecture(hour, &courseID, &groupID, &students);
    Course* c = courses.findCourse(courseID);
    *average = c->GetAvgStudentsInLecture();
    return SUCCESS;

}