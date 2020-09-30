//
// Created by Tsuf Cohen on 2019-06-05.
//

#ifndef DS_WET_2_UGSCHEDULE_H
#define DS_WET_2_UGSCHEDULE_H
#include "library2.h"
#include "UnionFind.h"
#include "RoomsHash.h"
#include <iostream>

class UGschedule {

    int n;
    UnionFind courses;
    RoomsHash rooms;

public:
    UGschedule(int n);
    ~UGschedule() = default;

    /** Description: return the num of courses in the system
     * */
    int getN() const;


/** Description:   Adds a new room.
 * Input:         roomID - The ID of the room to add.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS == NULL, roomID <= 0
 *                FAILURE - If roomId is already in the DS.
 *                SUCCESS - Otherwise.
 */
    StatusType addRoom(int roomID);


/** Description:   Deletes a room.
 * Input:         roomID - The ID of the room to delete.
 * Output:        None.
 * Return Values: INVALID_INPUT - If DS == NULL, roomID <= 0
 *                FAILURE - If roomId is not in the DS.
 *                SUCCESS - Otherwise.
 */
    StatusType deleteRoom(int roomID) ;

/** Description:   Adds a new lecture.
 * Input:         courseID - The ID of the course.
 *                groupID - The ID of the lecture group.
 *                roomID - The ID of the room, e.g. (Taub) 6
 *                hour - The hour when the lecture is taking place.
 *                numStudents - The amount of students registered to that group.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS == NULL, groupID < 0,
 *                                  courseID > n, courseID < 1,
 *                                  numStudents < 0, roomID <= 0,
 *                                  hour > m, hour < 1
 *                FAILURE - If there already exists a lecture of this group in the given hour
 *                          If the room isn't in the DS or there's already a lecture taking place the room
 *                              in the given hour
 *
 *                SUCCESS - Otherwise.
 */
    StatusType addLecture(int courseID, int groupID, int roomID, int hour, int numStudents) ;

/** Description:   Deletes a lecture from the DS.
 * Input:         roomID - The ID of the room, e.g. (Taub) 6
 *                hour - The hour when the lecture is taking place.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS == NULL
 *                                if hour < 1 or hour > m.
 *                                if roomID <= 0
 *                FAILURE - If the room isn't in the DS or there's already a lecture taking place the room
 *                              in the given hour
 *                SUCCESS - Otherwise.
 */
    StatusType deleteLecture(int hour, int roomID);

/** Description:   Merges the two courses.
 * Input:         courseID1 - The id of the first course.
 * 				  courseID2 - The id of the second course.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS == NULL, or if courseID(1/2) < 1, or if courseID(1/2) > n.
 *                FAILURE - If both ID's refer to the same course, or both courses have a lecture of the same groupId at the same hour
 *                SUCCESS - Otherwise.
 */
    StatusType mergeCourses(int courseID1, int courseID2) ;

/** Description:   Conduct a competition between the two courses.
 * Input:         courseID1 - The first course in the competition.
 * 				  courseID2 - The second course in the competition.
 * 				  numGroups - The numbers of groups each course is allowed to use.
 * Output:        winner - A pointer to a to a variable that you should contain the id of the winner course.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT -   If any of the pointers is NULL
 *                                  If DS == NULL, or if courseID(1/2) <= 0, or if courseID(1/2) > n or numGroups <= 0.
 *                FAILURE - If the two ID's refer to the same course.
 *                SUCCESS - Otherwise.
 */
    StatusType competition(int courseID1, int courseID2, int numGroups, int * winner);

/** Description:   Returns the average amount of students in each group of the corresponding course.
 * Input:         roomID - The ID of the room, e.g. (Taub) 6
 *                hour - The hour when the lecture is taking place.
 * Output:        average - A pointer to a to a variable that you should contain the requested value.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If any of the pointers is NULL
 *                                or if hour < 1, hour < m,
 								  or roomID <= 0
 *                FAILURE - If there's no room with that ID in DS, or no lecture
 *                          is taking place in the given hour
 *                SUCCESS - Otherwise.
 */
    StatusType getAverageStudentsInCourse(int hour, int roomID, float* average) ;


};


#endif //DS_WET_2_UGSCHEDULE_H
