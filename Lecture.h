//
// Created by Tsuf Cohen on 2019-06-11.
//

#ifndef DS_WET_2_LECTURE_H
#define DS_WET_2_LECTURE_H


#include <cstdio>
#include <iostream>

using std::ostream;

//------------------------------------------------------------------------------
typedef enum {
    SMALLER = -1, BIGGER = 1, EQUAL = 0
}RelationResult;

class Lecture {
    int groupID; // first Key
    int roomID;
    int hour;    // second Key
    int numStudents;
//------------------------------------------------------------------------------

public:

/**-------------------------------Constructor-----------------------------------
 *
 * @param hour - hour of the lecture
 * @param room - room ID
 */
    Lecture(
            const int& group,
            const int& room,
            const int& hour,
            const int& students) :
            groupID(group), roomID(room), hour(hour), numStudents(students) {}
//------------------------------------------------------------------------------
/**--------------------------------Destructor---------------------------------*/
    ~Lecture() = default;
//------------------------------------------------------------------------------
    Lecture(const Lecture& other) = default;

    Lecture& operator=(const Lecture& other) = default;
//------------------------------------------------------------------------------
/**---------------------------------Operators-----------------------------------
  *
  * @param other
  * @return
  */
    bool operator==(const Lecture& other) {
        return ((groupID==other.groupID) && (hour==other.hour));
    }

    bool operator<(const Lecture& other) {
        if(groupID==other.groupID) {
            return (hour<other.hour);
        }else {
            return (groupID<other.groupID);
        }
    }

    bool operator<=(const Lecture& other) {
        return ((*this==other) || (*this<other));
    }

    bool operator>(const Lecture& other) {
        return !(*this<=other);
    }

    bool operator>=(const Lecture& other) {
        return ((*this>other) || (*this==other)) ;
    }
//------------------------------------------------------------------------------
/**----------------------------NumStudentsCompare-------------------------------
  *
  * @param other Lecture to compare with by number of students
  * @return values:
  *         BIGGER - if 'this' is bigger than 'other'
  *         EQUAL - if equal
  *         SMALLER - if 'this' is smaller than 'other'
  */
    RelationResult NumStudentsCompare(const Lecture& other) {
        if((*this==other) && (numStudents==other.numStudents)) {
            return EQUAL;
        }else if(numStudents==other.numStudents) {
            return (*this<other) ? SMALLER : BIGGER;
        }else {
            return (numStudents<other.numStudents) ? SMALLER : BIGGER;
        }
    }
//------------------------------------------------------------------------------
/**------------------------------------GetGroupID-------------------------------
 *
 * @return
 */
    int GetGroupID() const {
        return groupID;
    }
//------------------------------------------------------------------------------
/**------------------------------------GetRoomID--------------------------------
 *
 * @return
 */
    int GetRoomID() const {
        return roomID;
    }
//------------------------------------------------------------------------------
/**------------------------------------GetHour----------------------------------
 *
 * @return
 */
    int GetHour() const {
        return hour;
    }
//------------------------------------------------------------------------------
/**------------------------------------GetNumStudents---------------------------
 *
 * @return
 */
    int GetNumStudents() const {
        return numStudents;
    }

//------------------------------------------------------------------------------
    friend ostream& operator<<(ostream& os, const Lecture& lecture){
        return os << "(" << lecture.groupID << "," << lecture.roomID << ","
                  << lecture.hour << "," << lecture.numStudents << ")";
    }
//------------------------------------------------------------------------------

};
/*
ostream& operator<<(ostream& os, const Lecture& lecture) {
    return os << "(" << lecture.groupID << "," << lecture.roomID << ","
              << lecture.hour << "," << lecture.numStudents << ")";
}*/

#endif //DS_WET_2_LECTURE_H
