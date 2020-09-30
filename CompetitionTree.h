//
// Created by Tsuf Cohen on 2019-06-11.
//

#ifndef DS_WET_2_COMPETITIONTREE_H
#define DS_WET_2_COMPETITIONTREE_H


#include "CompetitionNode.h"

class CompetitionTree {
    size_t size;                // number of lectures.
    CompetitionNode* root;
//------------------------------------------------------------------------------

public:

/**-------------------------------Constructor-----------------------------------
 *
 * @param size - can be used for building an almost-full-empty-tree.
 */
    explicit CompetitionTree(const size_t& size = 0) : size(size), root(NULL) {
        if(size) {
            root = new CompetitionNode;
            root->AddEmptyNode(size-1);
        }
    }

//------------------------------------------------------------------------------
    CompetitionTree(const CompetitionTree& to_copy) = delete;

    CompetitionTree& operator=(const CompetitionTree& other) = delete;
//------------------------------------------------------------------------------
/**------------------------------CleanTheTree-----------------------------------
 * a method to clean the tree.
 * all of the tree's nodes will be deleted
 */
    void CleanTheTree() {
        if(root!=NULL) {
            size = 0;
            delete root;
            root = NULL;
        }
    }
//------------------------------------------------------------------------------
/**------------------------------DeleteAllInfo----------------------------------
 * all of the information stored in the tree and sub-trees will be deleted and
 * it's memory freed
 */
    void DeleteAllInfo() {
        if(root!=NULL) {
            root->DeleteInformation(root);
        }
    }
//------------------------------------------------------------------------------
/**--------------------------------Destructor-----------------------------------
 *
 * Destructor - delete the sub-trees first.
 * note: dont forget to use DeleteAllInfo in you wont to delete ti data.
 */
    ~CompetitionTree() {
        CleanTheTree();
    }
//------------------------------------------------------------------------------
/**---------------------------------Find----------------------------------------
 *  a method used to find information in the tree.
 * @param value - a value that the user want's to find.
 * @return pointer to the value or null if not inside.
 */
    Lecture* Find(Lecture& value) const {
        if(root==NULL) {
            return NULL;
        }
        const CompetitionNode* tmp = root->Find(value);
        if(tmp==NULL) {
            return NULL;
        }
        return tmp->GetLecture();
    }
//------------------------------------------------------------------------------
/**---------------------------------Add-----------------------------------------
 *
 * a method used to add an information to the tree.
 * @param value - a pointer to the value that the user want's to add
 * @return pointer to the information if success, null if already inside
 *          or if null argument.
 */
    Lecture* Add(Lecture* value) {
        if(value==NULL) {
            return NULL;
        }
        if(Find(*value)){
            return NULL;
        }
        if(root==NULL) {
            root = new CompetitionNode(value);
            size = 1;
            return root->GetLecture();
        }else {
            CompetitionNode* tmp = root->Add(value);
            if(tmp==NULL) {
                return NULL;
            }
            root = root->UpdateBalance();
            ++size;
            return tmp->GetLecture();
        }
    }
//------------------------------------------------------------------------------
/**---------------------------------Delete--------------------------------------
 *
 * a method used to delete an information from the tree.
 *
 * @param value - a value that the user want's to delete
 * @return true if success or false if not in the tree.
 */
    bool Delete(Lecture& value) {
        if(Find(value)==NULL) {
            return false;
        }
        Lecture* root_lecture = root->GetLecture();
        if(root_lecture && value.NumStudentsCompare(*root_lecture)==EQUAL) {
            root = root->DeleteRoot(root);
        }else {
            root = root->Delete(value);
        }
        if(root) {
            root = root->UpdateBalance();
        }
        --size;
        return true;
    }
//------------------------------------------------------------------------------
/**------------------------------------GetSize----------------------------------
 *
 * @return the size of the tree - number of nodes.
 */
    size_t GetSize() const {
        return size;
    }
//------------------------------------------------------------------------------
/**------------------------------------GetHeight--------------------------------
 *
 * @return the height of the tree.
 */
    size_t GetHeight() const {
        return root->GetHeight();
    }
//------------------------------------------------------------------------------
/**--------------------------------GetIfBalanced--------------------------------
 *
 * a method used to check if the tree is balanced.
 *
 * @return true if balanced - false if not.
 */
    bool GetIfBalanced() {
        if(root==NULL) {
            return true;
        }

        int balance_factor = root->GetBalanceFactor();

        return !(balance_factor<=-2 || 2<=balance_factor);
    }
//------------------------------------------------------------------------------
/**---------------------------------GetArrayInorder-----------------------------
 *
 * a method to get an array with the information of the tree nodes.
 * note: the information is sorted from min to max.
 * @return pointer to the array if success, null if failed.
 */
    Lecture** GetArrayInorder() const {
        if(root==NULL) {
            return NULL;
        }
        Lecture** array = new Lecture*[size];
        size_t counter = 0;
        if(root->CopyInorder(root, array, size, &counter)) {
            return array;
        }else {
            delete[] array;
            return NULL;
        }
    }
//------------------------------------------------------------------------------
/**--------------------------------AddArrayInorder------------------------------
 * works only with an empty tree and with a valid size array.
 * in case root is NULL, the method will build an almost-full-empty-tree with
 * the size of the array.
 * @param array - an array with the information to copy.
 * note: the array must have the size of the tree(number of nodes) or less.
 * @param array_size
 * @return true if success
 */
    bool AddArrayInorder(Lecture** array, size_t array_size) {
        if(array==NULL || array_size<=0) {
            return false;
        }

        size_t counter = 0;
        if(root==NULL) {                    // build an almost-full-empty-tree.
            size_t tmp = size;
            size = array_size;
            root = new CompetitionNode;
            root->AddEmptyNode(size-1);
            if(root->AddInorder(root, array, array_size, &counter)) {
                return true;
            }
            else {
                CleanTheTree();
                size = tmp;
                return false;
            }
        }else if(size<array_size) {
            return false;
        }else {
            return root->AddInorder(root, array, array_size, &counter);
        }
    }
//------------------------------------------------------------------------------
/**--------------------------------Merge----------------------------------------
 *
 *  merge two trees together into 'this' tree.
 * @param other - tree to merge with. will be clean after.
 * @return pointer to the new merged tree('this'), or NULL if failed.
 */
    CompetitionTree* Merge(CompetitionTree* other) {
        if(other==NULL || other->root==NULL) {
            return this;
        }
        if(root==NULL) {
            root=other->root;
            other->root=NULL;
            size=other->size;
            other->size=0;
            return this;
        }
        size_t merged_tree_size = size+other->size;
        Lecture** A = GetArrayInorder();
        Lecture** B = other->GetArrayInorder();
        Lecture** C = new Lecture* [merged_tree_size];
        size_t index_A = 0, index_B = 0;
        while((index_A<size) &&
                (index_B<other->size)) {      // merge A,B into C.
            if((A[index_A]->NumStudentsCompare(*B[index_B]))==SMALLER) {
                C[index_A+index_B] = A[index_A];
                ++index_A;
            }else if((A[index_A]->NumStudentsCompare(*B[index_B]))==BIGGER){
                C[index_A+index_B] = B[index_B];
                ++index_B;
            }else {
                delete[] A; delete[] B; delete[] C; return NULL;
            }
        }
        while(index_A<size) {
            C[index_A+index_B] = A[index_A];
            ++index_A;
        }
        while(index_B<other->size) {
            C[index_A+index_B] = B[index_B];
            ++index_B;
        }
        delete[] A; delete[] B;
        // create new almost-full-empty-tree and add C in an Inorder tour.
        CompetitionNode* tmp = root;
        root = NULL;
        if(AddArrayInorder(C, merged_tree_size)) {
            other->CleanTheTree();
            if(tmp) {
                delete tmp;
            }
            delete[] C;
            return this;
        }
        root = tmp;
        delete[] C;
        return NULL;
    }
//------------------------------------------------------------------------------
/**----------------------------PrintVisualTree----------------------------------
 *
 * recursive method for printing a visual Tree.
 * note: in an Inorder tour.
 * @param node - to print.
 * output example:
 Prints Visual AVL Tree:
lvl 0       1       2       3       4
-------------------------------------
            (100,000)
                    (200,000)
            (300,000)
                            (400,000)
    (500,000)
            (600,000)
    (700,000)
                    (800,000)
    (900,000)
            (910,000)
    (920,000)
-------------------------------------
 */
    void PrintVisualTree() {
        if(root==NULL) {
            return;
        }
        std::cout << " Prints Visual AVL Tree: " << std::endl;
        std::cout << "lvl ";
        for(size_t i = 0 ; i<=GetHeight() ; ++i) {
            std::cout << i << "       ";
        }
        std::cout << "\n-----";
        for(size_t i = 0 ; i<GetHeight() ; ++i) {
            std::cout << "--------";
        }
        root->PrintVisualNodes(root, GetHeight());
        std::cout << "\n-----";
        for(size_t i = 0 ; i<GetHeight() ; ++i) {
            std::cout << "--------";
        }
        std::cout << std::endl;
    }
//------------------------------------------------------------------------------
/**---------------------------------GetStudentsSum------------------------------
  *
  * @param numGroups
  */
    int GetStudentsSum(int numGroups) {
        if(root==NULL || numGroups<=0) {
            return -1;
        }
        return root->GetStudentsSum(numGroups);
    }
//------------------------------------------------------------------------------
/**------------------------GetAverageNumOfStudents------------------------------
  *
  * @param numGroups
  */
    float GetAverageNumOfStudents() {
        if(root==NULL) {
            return 0;
        }

        return root->GetAverageNumOfStudents();
    }
//------------------------------------------------------------------------------

};


#endif //DS_WET_2_COMPETITIONTREE_H
