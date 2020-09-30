
//
// Created by Tsuf Cohen on 2019-06-11.
//

#ifndef DS_WET_2_COURSE_H
#define DS_WET_2_COURSE_H

#include "Lecture.h"
#include "TreeAVL.h"
#include "CompetitionTree.h"
#include <iostream>

using std::ostream;
/*
 *
 * A class represents a course of the technion.
 * It maintains the course's lectures , stored in a tree.
 */
class Course {
    int courseID;
    TreeAVL<Lecture> lectures;
    CompetitionTree lectures_ranking;

//------------------------------------------------------------------------------

public:

/**-------------------------------Constructor-----------------------------------
 *
 * @param course - course ID
 */
    explicit Course(const int& course = 1) : courseID(course) {}
//------------------------------------------------------------------------------
/**--------------------------------Destructor---------------------------------*/
    ~Course() {
        lectures.DeleteAllInfo();
    }
//------------------------------------------------------------------------------
    Course(const Course& other) = delete;

    Course& operator=(const Course& other) = delete;
//------------------------------------------------------------------------------
/**---------------------------------Operators-----------------------------------
 *
 * @param other
 * @return
 */
    bool operator==(const Course& other) {
        return (courseID==other.courseID);
    }

    bool operator<(const Course& other) {
        return (courseID<other.courseID);
    }

    bool operator>(const Course& other) {
        return (courseID>other.courseID);
    }

    bool operator<=(const Course& other) {
        return (courseID<=other.courseID);
    }

    bool operator>=(const Course& other) {
        return (courseID>=other.courseID);
    }
//------------------------------------------------------------------------------
/**----------------------------------GetCourseId--------------------------------
 *
 * @return
 */
    int GetCourseId() const {
        return courseID;
    }
//------------------------------------------------------------------------------
/**-------------------------------GetNumOfLectures------------------------------
 *
 * @return
 */
    size_t GetNumOfLectures() const {
        return lectures.GetSize();
    }
//------------------------------------------------------------------------------
/**---------------------------------AddLecture----------------------------------
  *
  * @param group
  * @param room
  * @param hour
  * @param students
  * @return
  */
    bool AddLecture(int group, int room, int hour, int students) {
        Lecture* lec = new Lecture(group, room, hour, students);
        if(lectures.Add(lec)) {
            lectures_ranking.Add(lec);
            return true;
        }else {
            delete lec;
            return false;
        }
    }
//------------------------------------------------------------------------------
/**---------------------------------DeleteLecture-------------------------------
  *
  * @param group
  * @param room
  * @param hour
  * @return
  */
    bool DeleteLecture(int group, int room, int hour, int students) {
        Lecture lecture(group, room, hour, students);
        Lecture* ptr = lectures.Find(lecture);
        if(lectures.Delete(lecture)) {
            lectures_ranking.Delete(*ptr);
            delete ptr;
            return true;
        }else {
            return false;
        }
    }
//------------------------------------------------------------------------------
/**-------------------------------------IsEmpty---------------------------------
 *
 * @return true if empty, false if not
 */
    bool IsEmpty() {
        return this->lectures.GetSize()==0;
    }
//------------------------------------------------------------------------------
/**------------------------------GetAvgStudentsInLecture------------------------
  *
  * @param numGroups
  */
    float GetAvgStudentsInLecture() {
        return lectures_ranking.GetAverageNumOfStudents();
    }
//------------------------------------------------------------------------------
/**------------------------------------JoinTheCompetition-----------------------
  *
  * @param numGroups
  */
    int JoinTheCompetition(int numGroups) {
        if(numGroups<=0) {
            return -1;
        }
        return lectures_ranking.GetStudentsSum(numGroups);
    }
//------------------------------------------------------------------------------
/**----------------------------------Merge--------------------------------------
  *
  * @param other
  * @return
  */
    bool Merge(Course* other) {
        
         if(this->GetNumOfLectures() ==0 && other->GetNumOfLectures()==0)
            return true;
        
        if(this==other) {
            return false;
        }
       
        if(lectures.Merge(&other->lectures)!= nullptr) {
            return (lectures_ranking.Merge(&other->lectures_ranking) != nullptr);
        }else {
            return false;
        }
    }

//------------------------------------------------------------------------------
    friend ostream& operator<<(ostream& os, const Course& course){
        return os << "(" << course.courseID << ")";
    }
//------------------------------------------------------------------------------

};
/*
ostream& operator<<(ostream& os, const Course& course) {
    return os << "(" << course.courseID << ")";
}*/


#endif //DS_WET_2_COURSE_H
