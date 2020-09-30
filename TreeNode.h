//
// Created by Tsuf Cohen on 2019-06-15.
//

#ifndef TESTFORTREEAVL_TREENODE_H
#define TESTFORTREEAVL_TREENODE_H

#include <cstdio>
#include <iostream>

using std::size_t;

/**
 *
 * @tparam T - must have 'operator=','operator<' and 'operator=='.
 * note: 'T information' is the node's key, so the fields (in class T) that are
 *        used as keys must to be private and unable to be changed by methods.
 */
template <typename T /* typename Key, typename Data */ >
class TreeNode {
    // Key;
    T* information;
    size_t height;
    TreeNode* left;
    TreeNode* right;
//------------------------------------------------------------------------------
/**------------------------------UpdateHeight-----------------------------------
 *
 * a method used to update the node's height.
 */
    void UpdateHeight() {
        if(left==NULL && right==NULL) {
            height = 0;
            return;
        }

        size_t a = 0, b = 0;
        if(left) {
            a = left->height;
        }
        if(right) {
            b = right->height;
        }
        size_t max = (a<b) ? b : a;
        height = max+1;
    }
//------------------------------------------------------------------------------
/**----------------------------BaseRotateLeft-----------------------------------
 *
 * a method used to do left base rotation to the node.
 *
 * note: the method also updating the heights.
 * @return pointer to the updated root (return 'this' if no right-sub-tree).
 */
    TreeNode<T>* BaseRotateLeft() {
        if(right==NULL) {
            return this;
        }
        TreeNode<T>* new_root = right;
        right = new_root->left;
        new_root->left = this;

        this->UpdateHeight();
        new_root->UpdateHeight();
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
    TreeNode<T>* BaseRotateRight() {
        if(left==NULL) {
            return this;
        }
        TreeNode<T>* new_root = left;
        left = new_root->right;
        new_root->right = this;

        this->UpdateHeight();
        new_root->UpdateHeight();
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
    TreeNode<T>* DeleteNode(TreeNode<T>* node) {
        if(node==NULL) {
            return NULL;
        }
        TreeNode<T>* L = node->left;
        TreeNode<T>* R = node->right;
        if(L==NULL && R==NULL) {                       // node don't have son's.
            delete node;
            return NULL;
        }else if(L==NULL || R==NULL) {   // have only one son.
            TreeNode<T>* tmp = ((L==NULL) ? R : L);
            node->left = NULL;
            node->right = NULL; // so don't delete son by mistake.
            delete node;
            return tmp->UpdateBalance();
        }else {                   // have two son's.
            TreeNode<T>* current = node->right;
            while(current->left) {               // find the next Inorder.
                current = current->left;
            }
            T* tmp = node->information;           // swap the nodes information.
            node->information = current->information;
            current->information = tmp;

            if(current == node->right) {       // the right son is next Inorder.
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
 * @param information - the pointer to the value we want to save in the node.
 *          if NULL than we get an empty node.
 */
    explicit TreeNode(T* information = NULL) :
            information(information), height(0), left(NULL), right(NULL) {}
//------------------------------------------------------------------------------
/**--------------------------------Destructor-----------------------------------
 *
 * Destructor - delete the sub-trees first.
 */
    ~TreeNode() {
        delete left;
        delete right;
    }

//------------------------------------------------------------------------------
    TreeNode(const TreeNode<T>& other) = delete;

    TreeNode<T>& operator=(const TreeNode<T>& other) = delete;
//------------------------------------------------------------------------------
/**--------------------------------GetBalanceFactor-----------------------------
 *
 * a method used to calculate and get the balance factor of a node.
 *
 * note: (balance factor) = (left-sub-tree's height)-(right-sub-tree's height).
 * @return int - the balance factor.
 */
    int GetBalanceFactor() {
        UpdateHeight();
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
    TreeNode<T>* UpdateBalance() {
        if(left==NULL && right==NULL) {
            UpdateHeight();
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
 *  a method used to find information in the node or in the sub-trees.
 *
 * @param value - a value that the user want's to find.
 * @return pointer to the node with the value or null if not inside.
 */
    const TreeNode<T>* Find(T& value) const {
        const TreeNode<T>* node = this;
        T* info = NULL;
        while(node) {
            info = node->information;
            if(*info==value) {                  // value is found.
                return node;
            }else if(*info<value) {   // go to the right sub-tree.
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
 * a method used to add an information to the node or to the sub-trees.
 * note: if the node's information is NULL, the value is inserted immediately,
 *       without balancing or heights updating.
 * @param value - a pointer to the value that the user want's to add
 * @return pointer to the node with the value added or null if already inside
 *          or if null param.
 */
    TreeNode<T>* Add(T* value) {
        if(value==NULL) {
            return NULL;
        }
        if(information==NULL) {            // add information to an empty node.
            information = value;
            return this;
        }
        TreeNode<T>* new_node = NULL;
        if(*information>*value) {                // add in the left sub-tree.
            if(left==NULL) {
                left = new TreeNode<T>(value);
                new_node = left;
            }else {
                new_node = left->Add(value);
                left = left->UpdateBalance();   // balance in case it is needed.
            }
        }
        if(*information<*value) {           // add in the right sub-tree.
            if(right==NULL) {
                right = new TreeNode<T>(value);
                new_node = right;
            }else {
                new_node = right->Add(value);
                right = right->UpdateBalance(); // balance in case it is needed.
            }
        }
        UpdateHeight();
        return new_node;        // if information==value than new_node is null.
    }
//------------------------------------------------------------------------------
/**---------------------------------DeleteRoot----------------------------------
 *
 * a method used to delete the root only.
 * @param root - pointer to the root we want to delete.
 * @return pointer to the updated root, null if no son's or if null argument.
 */
    TreeNode<T>* DeleteRoot(TreeNode<T>* root) {
        return DeleteNode(root);
    }
//------------------------------------------------------------------------------
/**---------------------------------Delete--------------------------------------
 *
 * a method used to delete an information from the node or from the sub-trees.
 *
 * @param value - a value that the user want's to delete
 * @return pointer to the updated root or null if no more nodes left.
 */
    TreeNode<T>* Delete(T& value) {
        if(*information<value) {                // delete in the right sub-tree.
            if(right) {
                if((*(right->information))==value) {
                    right = DeleteNode(right);
                }else {
                    right = right->Delete(value);
                }
            }
        }
        if(*information>value) {            // delete in the left sub-tree.
            if(left) {
                if((*(left->information))==value) {
                    left = DeleteNode(left);
                }else {
                    left = left->Delete(value);
                }
            }
        }
        return UpdateBalance();
    }
//------------------------------------------------------------------------------
/**---------------------------------GetInformation------------------------------
 *
 * note: 'T information' is the node's key, so the fields (in class T) that are
 *        used as keys must to be private and unable to be changed by methods.
 * @return pointer to the information.
 */
    T* GetInformation() const {
        return information;
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
 * recursive method for copying the node's and it's sub-trees information.
 * note: Inorder tour
 * @param treeNode - a node to copy it's information.
 * @param array - an array to save the information.
 * note: the array must have the size of the tree(number of nodes).
 * @param array_size
 * @param counter - used for the array index indicator.
 * note: must be initialized with 0.
 * @return true if success
 */
    bool CopyInorder(
            TreeNode<T>* treeNode,
            T** array,
            size_t array_size,
            size_t* counter) const {
        if(treeNode==NULL || array==NULL || array_size<=0 || counter==NULL) {
            return false;
        }
        if(*counter==array_size) {
            return false;
        }
        CopyInorder(treeNode->left, array, array_size, counter);
        array[*counter] = treeNode->information;
        ++(*counter);
        CopyInorder(treeNode->right, array, array_size, counter);
        return true;
    }
//------------------------------------------------------------------------------
/**--------------------------------AddInorder-----------------------------------
 *
 * recursive method for adding information to an almost-full-empty-tree.
 * note: add in an Inorder tour.
 * @param treeNode - a node to copy the information.
 * @param array - an array with the information to copy.
 * note: the array must have the size of the tree(number of nodes) or less.
 * @param array_size
 * @param counter - used for the array index indicator.
 * note: must be initialized with 0.
 * @return true if success
 */
    bool AddInorder(
            TreeNode<T>* treeNode,
            T** array,
            size_t array_size,
            size_t* counter) {
        if(treeNode==NULL || array==NULL || array_size<=0 || counter==NULL) {
            return false;
        }
        if(treeNode->information!=NULL || *counter==array_size) {
            return false;
        }
        AddInorder(treeNode->left, array, array_size, counter);
        treeNode->information = array[*counter];
        ++(*counter);
        AddInorder(treeNode->right, array, array_size, counter);
        return true;
    }
//------------------------------------------------------------------------------
/**--------------------------------DeleteInformation----------------------------
 *
 * recursive method for deleting the node's and it's sub-trees information.
 * note: Inorder tour.
  * @param treeNode - a node to delete it's information.
  */
    void DeleteInformation(TreeNode<T>* treeNode) {
        if(treeNode==NULL) {
            return;
        }

        DeleteInformation(treeNode->left);
        if(treeNode->information!=NULL) {
            delete treeNode->information;
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
        if(num_of_nodes_to_add<0 || information!=NULL) {
            return false;
        }
        if(num_of_nodes_to_add==0) {
            return true;
        }else if(num_of_nodes_to_add==1) {
            if(left==NULL || right==NULL) {                // create a sub-tree.
                (left ? right : left) = new TreeNode<T>();
            }else {
                return false;
            }
        }else if(left==NULL &&
                right==NULL) {// in case num_of_nodes_to_add >= 2.
            left = new TreeNode<T>();               // create the left sub-tree.
            right = new TreeNode<T>();             // create the right sub-tree.
            size_t num_of_nodes_left = num_of_nodes_to_add-2;

            left->AddEmptyNode((num_of_nodes_left%2)+(num_of_nodes_left/2));
            right->AddEmptyNode(num_of_nodes_left/2);
        }else {                              // in case sub-trees already exists.
            return false;
        }
        UpdateHeight();                // if success - update height and finish.
        return true;
    }
//------------------------------------------------------------------------------
/**----------------------------------PrintVisualNodes---------------------------
  *
  * @param node - the node we want to print visual.
  * note: the method will print also the node's sub-tree.
  * @param level - the level of the node inside the tree.
  */
    void PrintVisualNodes(TreeNode<T>* node, size_t level) {
        if(node==NULL) {
            return;
        }

        PrintVisualNodes(node->left, level-1);

        std::cout << "\n    ";
        for(size_t i = 0 ; i<level ; ++i) {
            std::cout << "        ";
        }
        std::cout << *node->information;

        PrintVisualNodes(node->right, level-1);
    }
//------------------------------------------------------------------------------

};

#endif //TESTFORTREEAVL_TREENODE_H
