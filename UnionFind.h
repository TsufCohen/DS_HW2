//
// Created by Tsuf Cohen on 2019-06-05.
//

#ifndef DS_WET_2_UNIONFIND_H
#define DS_WET_2_UNIONFIND_H


#include "Lecture.h"
#include "Course.h"
#include <iostream>

class UnionFind {
    int n;
    int *size;
    int *parent;
    Course **courses;

    int find(int courseID);

    // the courses are by index
    //Course** courses;
public:
    UnionFind(int n);

    ~UnionFind();

    Course *findCourse(const int courseID);

    bool unionC(int c1, int c2);
};
#endif //DS_WET_2_UNIONFIND_H
