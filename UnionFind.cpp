//
// Created by Chen on 13/06/2019.
//
#include "UnionFind.h"

UnionFind::UnionFind(int n) : n(n) {
    try {
        size = new int[n];
        parent = new int[n];
        courses = new Course*[n];

    }catch(std::bad_alloc& e){
        delete[] size;
        delete[] parent;
        delete[] courses;
    }
    try{
        for (int i = 0; i < n; ++i) {
            size[i] = 1;
            parent[i] = -1;
            courses[i] = new Course(i + 1);
        }
    }catch(std::bad_alloc& e){
        delete[] size;
        delete[] parent;
        for(int i= 0; i<n; i++)
            delete courses[i];
        delete[] courses;
    }

}
UnionFind::~UnionFind() {

    delete[] size;
    delete[] parent;
    for(int i = 0 ; i<n ; ++i) {
        delete courses[i];
    }
    delete[] courses;
}
int UnionFind::find(int courseID){

    int next = parent[courseID];
    //std::cout<<"find next "<<next<<std::endl;
    int index = courseID;
    while(next!=-1){
        index = next;
        next = parent[next];
       // std::cout<<"find next "<<next<<std::endl;

    }
    return index;
}

Course* UnionFind::findCourse(int courseID){

    int index = find(courseID-1);
    int curr = courseID-1;

    while(curr != index){
        int temp = parent[curr];
        parent[curr] = index;
        curr = temp;
    }
    return courses[index];
}
bool UnionFind::unionC(int c1, int c2){

    int i1= find(c1-1);
    int i2= find(c2-1);
    //std::cout<<"c1 is "<<i1<<" c2 is "<<i2<<std::endl;
    if(i1 == i2)
        return false;

    bool check;

        if (size[i1] < size[i2]) {
            check = courses[i2]->Merge(courses[i1]);
            if(check){
                size[i2] = size[i2]+size[i1];
                size[i1] =-1;
                parent[i1]= i2;
                delete courses[i1];
                courses[i1] = nullptr;
            }
        } else {
           // std::cout<<"in"<<std::endl;
            check = courses[i1]->Merge(courses[i2]);
            if(check) {
                size[i1] = size[i2] + size[i1];
                size[i2] = -1;
                parent[i2] = i1;
                delete courses[i2];
                courses[i2] = nullptr;
            }
        }
    return check;
}

