// AVLSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include <iostream>
#include <utility>
#include <typeinfo>
#include <string>
#include "Set.hpp"



template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet() noexcept;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    struct AVLNode
    {
        ElementType value;
        AVLNode* LeftNode = nullptr;
        AVLNode* RightNode = nullptr;
    };
    int sz;
    void copy_this_foo_recursively(const AVLNode* target, AVLNode* og);
    void copy_this_dying_foo_recursively(AVLNode* target, AVLNode* og);
    void delete_this_foo_entirely_recursively(AVLNode* node);
    int find_this_foos_height(AVLNode* node) const;
    void recurse_pre(VisitFunction visit, AVLNode* node) const;
    void recurse_post(VisitFunction visit, AVLNode* node) const;
    void recurse_in(VisitFunction visit, AVLNode* node) const;
    AVLNode* head_ptr;


};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
    :sz{0}, head_ptr{nullptr}
{
    // make size 0
    // make head pointer null
}

template <typename ElementType>
void AVLSet<ElementType>::delete_this_foo_entirely_recursively(AVLNode* node)
{
    // base case
    if (node != nullptr && node->RightNode == nullptr && node->LeftNode == nullptr)
    {
        // empty node deletion
        delete node;
    }
    // recursion case left
    else if (node->LeftNode != nullptr)
    {
        // left node recursion
        delete_this_foo_entirely_recursively(node->LeftNode);
    }
    //recursion case right
    else if (node->RightNode != nullptr)
    {
        // right node recursion
        delete_this_foo_entirely_recursively(node->RightNode);
    }
}

template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    if (head_ptr != nullptr)
    {
        // only delete if head isnt null
        delete_this_foo_entirely_recursively(head_ptr);
    }
    else
    {
        // else just delete the head pointer
        delete head_ptr;
    }
}


template <typename ElementType>
void AVLSet<ElementType>::copy_this_foo_recursively(const AVLNode* target, AVLNode* og)
{
    while (target != nullptr)
    {
        // og is the new node
        og = new AVLNode;
        og->value = target->value;
        if (target->LeftNode != nullptr)
        {
            // recursive call to left
            copy_this_foo_recursively(target->LeftNode, og->LeftNode);
        }
        else if (target->RightNode != nullptr)
        {
            // recursive call to right
            copy_this_foo_recursively(target->RightNode, og->RightNode);
        }
    }
}

template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
    // set to null // will reassign in copy
    sz = s.sz;
    head_ptr = nullptr;
    // use copy recursion
    copy_this_foo_recursively(s.head_ptr, head_ptr);
    
}

template <typename ElementType>
void AVLSet<ElementType>::copy_this_dying_foo_recursively(AVLNode* target, AVLNode* og)
{
    while (target != nullptr)
    {
        // og is new node
        og = new AVLNode;
        og->value = std::move(target->value);
        if (target->LeftNode != nullptr)
        {
            //recursive call to left
            copy_this_foo_recursively(target->LeftNode, og->LeftNode);
        }
        else if (target->RightNode != nullptr)
        {
            // recursive call to right
            copy_this_foo_recursively(target->RightNode, og->RightNode);
        }

    }

}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
    sz = s.sz;
    head_ptr = nullptr;
    copy_this_dying_foo_recursively(s.head_ptr, head_ptr);

}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{

    if (head_ptr != nullptr)
    {
        // delete content of original
        delete_this_foo_entirely_recursively(head_ptr);
    }
    // make head null
    head_ptr = nullptr;

    //copy
    copy_this_foo_recursively(s.head_ptr, head_ptr);

    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    if (head_ptr != nullptr)
    {
        // delete content of original
        delete_this_foo_entirely_recursively(head_ptr);
    }
    // make head null
    head_ptr = nullptr;

    // copy dying
    copy_this_dying_foo_recursively(s.head_ptr, head_ptr);

    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    AVLNode* traversing_node = head_ptr;
    AVLNode* traversing_omnidirection = head_ptr;
    if (traversing_node == nullptr)
    {
        // if head is null already, make new node w/ element
        head_ptr = new AVLNode{element};
        // increase size
        sz++;
    }
    else
    {
        while (traversing_omnidirection != nullptr)
        {
            if (element < traversing_omnidirection->value)
            {
                // if less than, go to left node
                traversing_node = traversing_omnidirection;
                traversing_omnidirection = traversing_omnidirection->LeftNode;
            }
            else if (element > traversing_omnidirection->value)
            {
                // if greater than, go to right node
                traversing_node = traversing_omnidirection;
                traversing_omnidirection = traversing_omnidirection->RightNode;
            }
        }
        // new node
        AVLNode* add_this_foo = new AVLNode{element};
        if (element < traversing_node->value)
        {
            // if less than, make left node the new node
            traversing_node->LeftNode = add_this_foo;
            sz++;
        }
        else if (element > traversing_node->value)
        {
            // if greater than, make the right node the new node
            traversing_node->RightNode = add_this_foo;
            sz++;
        }
    }

}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    AVLNode* traversing_node = head_ptr;

    if (traversing_node == nullptr)
    {
        // if head is null, auto false
        return false;
    }
    else
    {
        // if not, iterate
        while (traversing_node != nullptr)
        {
            if (element < traversing_node->value)
            {
                traversing_node = traversing_node->LeftNode;
            }
            else if (element > traversing_node->value)
            {
                traversing_node = traversing_node->RightNode;
            }
            else
            {
                // returns true if not less than nor greater than (so equal)
                return true;
            }
        }
        return false;
    }
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    // return size counter
    return sz;
}

template <typename ElementType>
int AVLSet<ElementType>::find_this_foos_height(AVLNode* node) const
{
    // null works
    if (node == nullptr)
    {
        return -1;
    }
    // everything else is 1 more than it should be when balanced
    else
    {
        int leftyLucy = find_this_foos_height(node->LeftNode);
        int righyThaiTea = find_this_foos_height(node->RightNode);
        if (leftyLucy > righyThaiTea)
        {
            
            return(leftyLucy+1);
        }
        else
        {
            
            return (righyThaiTea+1);
        }
    }
}

template <typename ElementType>
int AVLSet<ElementType>::height() const
{
    // if empty return -1
    if (head_ptr == nullptr)
    {
        return -1;
    }
    // recursion
    else
    {
        return find_this_foos_height(head_ptr);   
    }
    
}

template <typename ElementType>
void AVLSet<ElementType>::recurse_pre(VisitFunction visit, AVLNode* node) const
{
    // recursion for preorder
    AVLNode* traversing_node = node;
    if (traversing_node == nullptr)
    {

        return;
    }
    else
    {
        visit(traversing_node->value);
        recurse_pre(visit, traversing_node->LeftNode);
        recurse_pre(visit, traversing_node->RightNode);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    // use recursive function for preorder
    recurse_pre(visit, head_ptr);

}


template <typename ElementType>
void AVLSet<ElementType>::recurse_in(VisitFunction visit, AVLNode* node) const
{
    //recursion for inorder
    AVLNode* traversing_node = node;
    if (traversing_node == nullptr)
    {

        return;
    }
    else
    {
        recurse_in(visit, traversing_node->LeftNode);
        visit(traversing_node->value);
        recurse_in(visit, traversing_node->RightNode);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    //use recursive function for inorder
    recurse_in(visit, head_ptr);
}

template <typename ElementType>
void AVLSet<ElementType>::recurse_post(VisitFunction visit, AVLNode* node) const
{
    // recursion for postorder
    AVLNode* traversing_node = node;
    if (traversing_node == nullptr)
    {

        return;
    }
    else
    {
        recurse_post(visit, traversing_node->LeftNode);
        recurse_post(visit, traversing_node->RightNode);
        visit(traversing_node->value);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    // use recursive function for postorder
    recurse_post(visit, head_ptr);
}



#endif // AVLSET_HPP

