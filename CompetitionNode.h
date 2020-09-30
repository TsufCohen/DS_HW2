//
// Created by Tsuf Cohen on 2019-06-11.
//

#ifndef DS_WET_2_COMPETITIONNODE_H
#define DS_WET_2_COMPETITIONNODE_H

#include "Lecture.h"

#include <cstdio>
#include <iostream>

using std::size_t;

class CompetitionNode {
    // standard AVL members
    Lecture* lecture;
    size_t height;
    CompetitionNode* left;
    CompetitionNode* right;
    // Rank tree extras
    int nodes_in_subtree;
    int students_sum_in_subtree;

//------------------------------------------------------------------------------
/**------------------------------UpdateProperties-------------------------------
 *
 * a method used to update the node's height, nodes_in_subtree and students_sum_in_subtree.
 */
    void UpdateProperties() {
        if(left==NULL && right==NULL) {
            height = 0;
            nodes_in_subtree = 1;
            if(lecture!=NULL){
                students_sum_in_subtree = lecture->GetNumStudents();
            }
            return;
        }

        int nodes = 1;
        int students = 0;
        if(lecture!=NULL){
            students = lecture->GetNumStudents();
        }

        size_t a = 0, b = 0;

        if(left!=NULL) {
            a = left->height;
            nodes += left->nodes_in_subtree;
            students += left->students_sum_in_subtree;
        }
        if(right!=NULL) {
            b = right->height;
            nodes += right->nodes_in_subtree;
            students += right->students_sum_in_subtree;
        }
        size_t max = (a<b) ? b : a;
        height = max+1;
        nodes_in_subtree = nodes;
        students_sum_in_subtree = students;
    }
//------------------------------------------------------------------------------
/**----------------------------BaseRotateLeft-----------------------------------
 *
 * a method used to do left base rotation to the node.
 *
 * note: the method also updating the heights.
 * @return pointer to the updated root (return 'this' if no right-sub-tree).
 */
    CompetitionNode* BaseRotateLeft() {
        if(right==NULL) {
            return this;
        }
        CompetitionNode* new_root = right;
        right = new_root->left;
        new_root->left = this;

        this->UpdateProperties();
        new_root->UpdateProperties();
        return new_root;
    }
//------------------------------------------------------------------------------
/**------------------------------BaseRotateRight--------------------------------
 *
 * a method used to do right base rotation to the node.
 *
 * note: the method also updating the heights.
 * @return pointer to the updated root (return 'this' if no left-sub-tree).
 */
    CompetitionNode* BaseRotateRight() {
        if(left==NULL) {
            return this;
        }
        CompetitionNode* new_root = left;
        left = new_root->right;
        new_root->right = this;

        this->UpdateProperties();
        new_root->UpdateProperties();
        return new_root;
    }
//------------------------------------------------------------------------------
/**---------------------------------DeleteNode----------------------------------
 *
 * a method used to delete a given node, mainly the root.
 *  note: we use this with the Delete method and DeleteRoot method only.
 * @param node - pointer to the node we want to delete.
 * @return pointer to the updated root of the node's sub-tree.
 *                  null if no son's or if null argument.
 */
    CompetitionNode* DeleteNode(CompetitionNode* node) {
        if(node==NULL) {
            return NULL;
        }
        CompetitionNode* L = node->left;
        CompetitionNode* R = node->right;
        if(L==NULL && R==NULL) {                       // node don't have son's.
            delete node;
            return NULL;
        }else if(L==NULL || R==NULL) {   // have only one son.
            CompetitionNode* tmp = ((L==NULL) ? R : L);
            node->left = NULL;
            node->right = NULL; // so don't delete son by mistake.
            delete node;
            return tmp->UpdateBalance();
        }else {                   // have two son's.
            CompetitionNode* current = node->right;
            while(current->left) {               // find the next Inorder.
                current = current->left;
            }
            Lecture* tmp = node->lecture;           // swap the nodes lecture.
            node->lecture = current->lecture;
            current->lecture = tmp;

            if(current==node->right) {       // the right son is next Inorder.
                node->right = DeleteNode(current);
            }else {
                node->right = node->right->Delete(*tmp);
            }
            return node->UpdateBalance();
        }
    }
//------------------------------------------------------------------------------

public:

/**-------------------------------Constructor-----------------------------------
 *
 * the constructor of the class.
 * note: the typename T must have 'operator=','operator<', 'operator<' and 'operator=='.
 * @param lecture - the pointer to the value we want to save in the node.
 *          if NULL than we get an empty node.
 */
    explicit CompetitionNode(Lecture* lecture = NULL) :
            lecture(lecture),
            height(0),
            left(NULL),
            right(NULL),
            nodes_in_subtree(1),
            students_sum_in_subtree(0) {
        if(lecture!=NULL) {
            students_sum_in_subtree = lecture->GetNumStudents();
        }
    }
//------------------------------------------------------------------------------
/**--------------------------------Destructor-----------------------------------
 *
 * Destructor - delete the sub-trees first.
 */
    ~CompetitionNode() {
        delete left;
        delete right;
    }

//------------------------------------------------------------------------------
    CompetitionNode(const CompetitionNode& other) = delete;

    CompetitionNode& operator=(const CompetitionNode& other) = delete;
//------------------------------------------------------------------------------
/**--------------------------------GetBalanceFactor-----------------------------
 *
 * a method used to calculate and get the balance factor of a node.
 *
 * note: (balance factor) = (left-sub-tree's height)-(right-sub-tree's height).
 * @return int - the balance factor.
 */
    int GetBalanceFactor() {
        UpdateProperties();
        int a = 0, b = 0;
        if(left) {
            a = (1+(int)(left->height));
        }
        if(right) {
            b = (1+(int)(right->height));
        }
        return (a-b);
    }
//------------------------------------------------------------------------------
/**------------------------------UpdateBalance----------------------------------
 *
 * a method used to balance the AVL node with it's sub-trees.
 *
 * note: the method also updating the heights.
 * @return pointer to the updated root (return 'this' if already balanced).
 */
    CompetitionNode* UpdateBalance() {
        if(left==NULL && right==NULL) {
            UpdateProperties();
            return this;
        }
        switch(GetBalanceFactor()) {
            case 2:
                if(left->GetBalanceFactor()>=0) {
                    return BaseRotateRight();                     // LL rotate.
                }else {
                    left = left->BaseRotateLeft();                // LR rotate.
                    return BaseRotateRight();
                }
            case -2:
                if(right->GetBalanceFactor()<=0) {
                    return BaseRotateLeft();                      // RR rotate.
                }else {
                    right = right->BaseRotateRight();
                    return BaseRotateLeft();                      // RL rotate.
                }
            default:return this;
        }
    }
//------------------------------------------------------------------------------
/**------------------------------Find-------------------------------------------
 *
 *  a method used to find lecture in the node or in the sub-trees.
 *
 * @param value - a value that the user want's to find.
 * @return pointer to the node with the value or null if not inside.
 */
    const CompetitionNode* Find(Lecture& value) const {
        const CompetitionNode* node = this;
        Lecture* info = NULL;
        while(node) {
            info = node->lecture;
            if(info && value.NumStudentsCompare(*info)==EQUAL) {                  // value is found.
                return node;
            }else if(info && value.NumStudentsCompare(*info)==BIGGER) {   // go to the right sub-tree.
                node = node->right;
            }else {                                  // go to the left sub-tree.
                node = node->left;
            }
        }
        return NULL;
    }
//------------------------------------------------------------------------------
/**---------------------------------Add-----------------------------------------
 *
 * a method used to add an lecture to the node or to the sub-trees.
 * note: if the node's lecture is NULL, the value is inserted immediately,
 *       without balancing or heights updating.
 * @param value - a pointer to the value that the user want's to add
 * @return pointer to the node with the value added or null if already inside
 *          or if null param.
 */
    CompetitionNode* Add(Lecture* value) {
        if(value==NULL) {
            return NULL;
        }
        if(lecture==NULL) {            // add information to an empty node.
            lecture = value;
            UpdateProperties();
            return this;
        }
        CompetitionNode* new_node = NULL;
        if(value->NumStudentsCompare(*lecture)==SMALLER) {                // add in the left sub-tree.
            if(left==NULL) {
                left = new CompetitionNode(value);
                new_node = left;
            }else {
                new_node = left->Add(value);
                left = left->UpdateBalance();   // balance in case it is needed.
            }
        }
        if(value->NumStudentsCompare(*lecture)==BIGGER) {           // add in the right sub-tree.
            if(right==NULL) {
                right = new CompetitionNode(value);
                new_node = right;
            }else {
                new_node = right->Add(value);
                right = right->UpdateBalance(); // balance in case it is needed.
            }
        }
        UpdateProperties();
        return new_node;        // if lecture==value than new_node is null.
    }
//------------------------------------------------------------------------------
/**---------------------------------DeleteRoot----------------------------------
 *
 * a method used to delete the root only.
 * @param root - pointer to the root we want to delete.
 * @return pointer to the updated root, null if no son's or if null argument.
 */
    CompetitionNode* DeleteRoot(CompetitionNode* root) {
        return DeleteNode(root);
    }
//------------------------------------------------------------------------------
/**---------------------------------Delete--------------------------------------
 *
 * a method used to delete a lecture from the node or from the sub-trees.
 *
 * @param value - a value that the user want's to delete
 * @return pointer to the updated root or null if no more nodes left.
 */
    CompetitionNode* Delete(Lecture& value) {
        if(value.NumStudentsCompare(*lecture)==BIGGER) {        // delete in the right sub-tree.
            if(right) {
                if(value.NumStudentsCompare(*(right->lecture))==EQUAL) {
                    right = DeleteNode(right);
                }else {
                    right = right->Delete(value);
                }
            }
        }
        if(value.NumStudentsCompare(*lecture)==SMALLER) {       // delete in the left sub-tree.
            if(left) {
                if(value.NumStudentsCompare(*(left->lecture))==EQUAL) {
                    left = DeleteNode(left);
                }else {
                    left = left->Delete(value);
                }
            }
        }
        return UpdateBalance();
    }
//------------------------------------------------------------------------------
/**---------------------------------GetLecture----------------------------------
 *
 * note: 'T lecture' is the node's key, so the fields (in class T) that are
 *        used as keys must to be private and unable to be changed by methods.
 * @return pointer to the lecture.
 */
    Lecture* GetLecture() const {
        return lecture;
    }
//------------------------------------------------------------------------------
/**---------------------------------GetHeight-----------------------------------
 *
 * @return the height of to the node.
 */
    size_t GetHeight() const {
        return height;
    }
//------------------------------------------------------------------------------
/**--------------------------------CopyInorder----------------------------------
 *
 * recursive method for copying the node's and it's sub-trees lecture.
 * note: Inorder tour
 * @param treeNode - a node to copy it's lecture.
 * @param array - an array to save the lecture.
 * note: the array must have the size of the tree(number of nodes).
 * @param array_size
 * @param counter - used for the array index indicator.
 * note: must be initialized with 0.
 * @return true if success
 */
    bool CopyInorder(
            CompetitionNode* treeNode,
            Lecture** array,
            size_t array_size,
            size_t* counter) const {
        if(treeNode==NULL || array==NULL || array_size<=0 || counter==NULL) {
            return false;
        }
        if(*counter==array_size) {
            return false;
        }
        CopyInorder(treeNode->left, array, array_size, counter);
        array[*counter] = treeNode->lecture;
        ++(*counter);
        CopyInorder(treeNode->right, array, array_size, counter);
        return true;
    }
//------------------------------------------------------------------------------
/**--------------------------------AddInorder-----------------------------------
 *
 * recursive method for adding lecture to an almost-full-empty-tree.
 * note: add in an Inorder tour.
 * @param treeNode - a node to copy the lecture.
 * @param array - an array with the lecture to copy.
 * note: the array must have the size of the tree(number of nodes) or less.
 * @param array_size
 * @param counter - used for the array index indicator.
 * note: must be initialized with 0.
 * @return true if success
 */
    bool AddInorder(
            CompetitionNode* treeNode,
            Lecture** array,
            size_t array_size,
            size_t* counter) {
        if(treeNode==NULL || array==NULL || array_size<=0 || counter==NULL) {
            return false;
        }
        if(treeNode->lecture!=NULL || *counter==array_size) {
            return false;
        }
        AddInorder(treeNode->left, array, array_size, counter);
        treeNode->lecture = array[*counter];
        ++(*counter);
        AddInorder(treeNode->right, array, array_size, counter);
        treeNode->UpdateProperties();
        return true;
    }
//------------------------------------------------------------------------------
/**--------------------------------DeleteInformation----------------------------
 *
 * recursive method for deleting the node's and it's sub-trees lecture.
 * note: Inorder tour.
  * @param treeNode - a node to delete it's lecture.
  */
    void DeleteInformation(CompetitionNode* treeNode) {
        if(treeNode==NULL) {
            return;
        }

        DeleteInformation(treeNode->left);
        if(treeNode->lecture!=NULL) {
            delete treeNode->lecture;
        }
        DeleteInformation(treeNode->right);
    }
//------------------------------------------------------------------------------
/**------------------------------AddEmptyNode-----------------------------------
 *
 * a method used to add a node to the node's sub-trees.
 * note: this method is used only to build an almost-full-empty-tree.
 * @param num_of_nodes_to_add - number of nodes we want to add to 'this'.
 * @return true - success, false - the root node is not empty or have two sons.
  */
    bool AddEmptyNode(size_t num_of_nodes_to_add) {
        if(num_of_nodes_to_add<0 || lecture!=NULL) {
            return false;
        }
        if(num_of_nodes_to_add==0) {
            return true;
        }else if(num_of_nodes_to_add==1) {
            if(left==NULL || right==NULL) {                // create a sub-tree.
                (left ? right : left) = new CompetitionNode();
            }else {
                return false;
            }
        }else if(left==NULL &&
                right==NULL) {// in case num_of_nodes_to_add >= 2.
            left =
                    new CompetitionNode();               // create the left sub-tree.
            right =
                    new CompetitionNode();             // create the right sub-tree.
            size_t num_of_nodes_left = num_of_nodes_to_add-2;

            left->AddEmptyNode((num_of_nodes_left%2)+(num_of_nodes_left/2));
            right->AddEmptyNode(num_of_nodes_left/2);
        }else {                              // in case sub-trees already exists.
            return false;
        }
        UpdateProperties();                // if success - update height and finish.
        return true;
    }
//------------------------------------------------------------------------------
/**----------------------------------PrintVisualNodes---------------------------
  *
  * @param node - the node we want to print visual.
  * note: the method will print also the node's sub-tree.
  * @param level - the level of the node inside the tree.
  */
    void PrintVisualNodes(CompetitionNode* node, size_t level) {
        if(node==NULL) {
            return;
        }

        PrintVisualNodes(node->left, level-1);

        std::cout << "\n    ";
        for(size_t i = 0 ; i<level ; ++i) {
            std::cout << "        ";
        }
        std::cout << *node->lecture;

        PrintVisualNodes(node->right, level-1);
    }
//------------------------------------------------------------------------------
/**------------------------------------GetStudentsSum---------------------------
 *
 * @param numGroups
 * @return
 */
    int GetStudentsSum(int numGroups) {
        if(numGroups<=0) {
            return 0;
        }else if(numGroups>=nodes_in_subtree) {
            return students_sum_in_subtree;
        }else if(right) {
            if(numGroups>=(1+(right->nodes_in_subtree))) {
                return (lecture->GetNumStudents())+
                        (right->students_sum_in_subtree)+
                        (left->GetStudentsSum(
                                numGroups-(1+right->nodes_in_subtree)));
            }else {
                return right->GetStudentsSum(numGroups);
            }
        }else {
            return lecture->GetNumStudents();
        }
    }
//------------------------------------------------------------------------------
/**---------------------------GetAverageNumOfStudents---------------------------
 *
 * @return
 */
    float GetAverageNumOfStudents() {
        return ((float)students_sum_in_subtree/nodes_in_subtree);
    }
//------------------------------------------------------------------------------

};


#endif //DS_WET_2_COMPETITIONNODE_H
