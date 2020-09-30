//
// Created by Tsuf Cohen on 2019-06-15.
//

#ifndef TESTFORTREEAVL_TREEAVL_H
#define TESTFORTREEAVL_TREEAVL_H

#include "TreeNode.h"

using std::size_t;

/**
 *
 * @tparam T - must have 'operator=','operator<', 'operator<' and 'operator=='.
 * note: 'T information' is the node's key, so the fields (in class T) that are
 *        used as keys must to be private and unable to be changed by methods.
 */
template <typename T>
class TreeAVL {
    size_t size;                // number of nodes.
    TreeNode<T>* root;
//------------------------------------------------------------------------------

public:

/**-------------------------------Constructor-----------------------------------
 *
 * @param size - can be used for building an almost-full-empty-tree.
 */
    explicit TreeAVL(const size_t& size = 0) : size(size), root(NULL) {
        if(size) {
            root = new TreeNode<T>;
            root->AddEmptyNode(size-1);
        }
    }

//------------------------------------------------------------------------------
    TreeAVL(const TreeAVL<T>& to_copy) = delete;

    TreeAVL<T>& operator=(const TreeAVL<T>& other) = delete;
//------------------------------------------------------------------------------
/**------------------------------CleanTheTree-----------------------------------
 * a method to clean the tree.
 * all of the tree's nodes will be deleted
 */
    void CleanTheTree() {
        if(root != NULL) {
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
        if(root != NULL) {
            root->DeleteInformation(root);
        }
    }
//------------------------------------------------------------------------------
/**--------------------------------Destructor-----------------------------------
 *
 * Destructor - delete the sub-trees first.
 */
    ~TreeAVL() {
        CleanTheTree();
    }
//------------------------------------------------------------------------------
/**---------------------------------Find----------------------------------------
 *  a method used to find information in the tree.
 * @param value - a value that the user want's to find.
 * @return pointer to the value or null if not inside.
 */
    T* Find(T& value) const {
        if(root==NULL) {
            return NULL;
        }
        const TreeNode<T>* tmp = root->Find(value);
        if(tmp==NULL) {
            return NULL;
        }
        return tmp->GetInformation();
    }
//------------------------------------------------------------------------------
/**---------------------------------Add-----------------------------------------
 *
 * a method used to add an information to the tree.
 * @param value - a pointer to the value that the user want's to add
 * @return pointer to the information if success, null if already inside
 *          or if null argument.
 */
    T* Add(T* value) {
        if(root==NULL) {
            root = new TreeNode<T>(value);
            size = 1;
            return root->GetInformation();
        }else {
            TreeNode<T>* tmp = root->Add(value);
            if(tmp==NULL) {
                return NULL;
            }
            root = root->UpdateBalance();
            ++size;
            return tmp->GetInformation();
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
    bool Delete(T& value) {
        if(Find(value)==NULL) {
            return false;
        }

        if(value == (*(root->GetInformation()))) {
            root = root->DeleteRoot(root);
        }else {
            root = root->Delete(value);
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
    T** GetArrayInorder() const {
        if(root==NULL) {
            return NULL;
        }
        T** array = new T* [size];
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
    bool AddArrayInorder(T** array, size_t array_size) {
        if(array==NULL || array_size<=0) {
            return false;
        }

        size_t counter = 0;
        if(root==NULL) {                    // build an almost-full-empty-tree.
            size_t tmp = size;
            size = array_size;
            root = new TreeNode<T>;
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
    TreeAVL<T>* Merge(TreeAVL<T>* other) {
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
        T** A = GetArrayInorder();
        T** B = other->GetArrayInorder();
        T** C = new T* [merged_tree_size];
        size_t index_A = 0, index_B = 0;
        while((index_A<size) &&
                (index_B<other->size)) {      // merge A,B into C.
            if(*A[index_A]<*B[index_B]) {
                C[index_A+index_B] = A[index_A];
                ++index_A;
            }else if(*A[index_A]>*B[index_B]){
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
        TreeNode<T>* tmp = root;
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

};

#endif //TESTFORTREEAVL_TREEAVL_H
