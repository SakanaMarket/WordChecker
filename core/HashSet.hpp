// HashSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include <utility>
#include <memory>
#include <iostream>
#include <typeinfo>
#include "Set.hpp"



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet() noexcept;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.  In the case
    // where the array is resized, this function runs in linear time (with
    // respect to the number of elements, assuming a good hash function);
    // otherwise, it runs in constant time (again, assuming a good hash
    // function).
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;

    // Hashing and assigning index to place nodes into table

private:
    HashFunction hashFunction;
    struct LinkedListNode
    {
    	ElementType value;
    	LinkedListNode* next = nullptr;
    };
    // LinkedListNode** array_of_LL;
    LinkedListNode** array_of_LL;
    void delete_this_foos_linked_list();
    unsigned int cap;
    unsigned int current_sz;
    unsigned int sz_for_load_factor;
    void shove_it(const ElementType& element);
    bool* array_of_added_to_index;
};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}, array_of_LL{new LinkedListNode*[DEFAULT_CAPACITY]},
    cap{DEFAULT_CAPACITY}, current_sz{0}, sz_for_load_factor{0}, array_of_added_to_index{new bool[DEFAULT_CAPACITY]} // initializes the hash function
{
    // std::cout << "in constructor" << std::endl;
    for (int i = 0; i < cap; ++i)
    {
        // initializing new nodes for each index
        array_of_LL[i] = new LinkedListNode{};
        // initializing false for bool table
        array_of_added_to_index[i] = false;
    }

}

template <typename ElementType>
void HashSet<ElementType>::delete_this_foos_linked_list()
{
    // std::cout << "in constructor" << std::endl;
    for (int i = 0; i < cap; ++i)
    {
        // iterating through linked lists for every index in table
        LinkedListNode* current = array_of_LL[i];
        // std::cout << current->value << std::endl;
        if (current->next == nullptr)
        {
            delete current;
        }
        else
        {
            while (current != nullptr)
            {

                LinkedListNode* del = current;
                current = current->next;
                delete del;
            }
        }
    }
    // delete array of bool
    delete[] array_of_added_to_index;
    // delete array of linked lists
    delete[] array_of_LL;
}

template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    // destructor
    // std::cout << "in deconstructor" << std::endl;
    delete_this_foos_linked_list();
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>},
    array_of_LL{new LinkedListNode*[s.cap]}, cap{s.cap}, current_sz{s.current_sz}, sz_for_load_factor{s.sz_for_load_factor},
    array_of_added_to_index{new bool[s.cap]}
{
    // ^^ initializing new
    // std::cout << "in copy" << std::endl;
    for (int i = 0; i < cap; ++i)
    {
        // setting everything to default
        array_of_LL[i] = new LinkedListNode{s.array_of_LL[i]->value};
        array_of_LL[i]->next = nullptr;
        array_of_added_to_index[i] = false;
    }
    for (int i = 0; i < cap; ++i)
    {
        // iterating
        LinkedListNode* current = s.array_of_LL[i]->next;
        LinkedListNode* this_node = array_of_LL[i]->next;

        while (current != nullptr)
        {
            this_node = new LinkedListNode{current->value, nullptr};
            array_of_added_to_index[i] = true;
            current = current->next;
            this_node = this_node->next;
        }   
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>},
    array_of_LL{new LinkedListNode*[s.cap]}, cap{s.cap}, current_sz{s.current_sz}, sz_for_load_factor{s.sz_for_load_factor},
    array_of_added_to_index{new bool[s.cap]}
{
    // ^^ initializing new
    // std::cout << "in copy dying" << std::endl;
    for (int i = 0; i < cap; ++i)
    {
        // setting everything to default
        array_of_LL[i] = new LinkedListNode{std::move(s.array_of_LL[i]->value)};
        array_of_LL[i]->next = nullptr;
        array_of_added_to_index[i] = false;
    }
    for (int i = 0; i < cap; ++i)
    {
        // iterating
        LinkedListNode* current = s.array_of_LL[i]->next;
        LinkedListNode* this_node = array_of_LL[i]->next;

        while (current != nullptr)
        {
            this_node = new LinkedListNode{std::move(current->value), nullptr};
            array_of_added_to_index[i] = true;
            current = current->next;
            this_node = this_node->next;
        }
    }
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    // std::cout << "in assignment" << std::endl;
    // if right value is not the same reference to current hash's reference
    // copy over right's value into this value
    if (this != &s)
    {
        // delete content first
        delete_this_foos_linked_list();

        // copying over values
        hashFunction = s.hashFunction;
        cap = s.cap;
        current_sz = s.current_sz;

        // array is new because it was deleted first thing
        array_of_LL = new LinkedListNode*[cap];
        array_of_added_to_index = new bool[cap];
        for (int i = 0; i < cap; ++i)
        {
            // initializing
            array_of_LL[i] = new LinkedListNode{s.array_of_LL[i]->value};
            array_of_LL[i]->next = nullptr;
            array_of_added_to_index[i] = false;
        }
        for (int i = 0; i < cap; ++i)
        {
            // iterating
            LinkedListNode* current = s.array_of_LL[i]->next;
            LinkedListNode* this_node = array_of_LL[i]->next;

            while (current != nullptr)
            {
                this_node = new LinkedListNode{current->value, nullptr};
                array_of_added_to_index[i] = true;
                current = current->next;
                this_node = this_node->next;
            }   
        }
    }
    return *this;
    
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    // std::cout << "in dying assignment" << std::endl;
    if (this != &s)
    {
        // delete content first
        delete_this_foos_linked_list();

        // copying over values
        hashFunction = std::move(s.hashFunction);
        cap = std::move(s.cap);
        current_sz = std::move(s.current_sz);

        // array is new because it was deleted first thing
        array_of_LL = new LinkedListNode*[cap];
        array_of_added_to_index = new bool[cap];
        for (int i = 0; i < cap; ++i)
        {
            // initializing
            array_of_LL[i] = new LinkedListNode{std::move(s.array_of_LL[i]->value)};
            array_of_LL[i]->next = nullptr;
            array_of_added_to_index[i] = false;
        }
        for (int i = 0; i < cap; ++i)
        {
            // iterating
            LinkedListNode* current = s.array_of_LL[i]->next;
            LinkedListNode* this_node = array_of_LL[i]->next;

            while (current != nullptr)
            {
                this_node = new LinkedListNode{std::move(current->value), nullptr};
                array_of_added_to_index[i] = true;
                current = current->next;
                this_node = this_node->next;
            }
        }
    }
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    // std::cout << "in isImplemented" << std::endl;
    return true;
}

template <typename ElementType>
void HashSet<ElementType>::shove_it(const ElementType& element)
{
    // std::cout << "in shove_it" << std::endl;

    // hash the element to look for particular index
    int index = static_cast<int>(hashFunction(element)%cap);
    // std::cout << "cap " << cap << "index " << index << "lf sz " << sz_for_load_factor << "current sz " << current_sz << std::endl;
    
    // Assign an indexed node to look into
    LinkedListNode* look_in_this_indexed_table = array_of_LL[index];
    if (array_of_added_to_index[index] == false)
    {
        // if the bool is false, make new node with element
        look_in_this_indexed_table->value = element;
        // set bool to true
        array_of_added_to_index[index] = true;

        // increment sizes // only increment load factor if new index
        current_sz++;
        sz_for_load_factor++;
    }
    else 
    { 
        // if the element isn't contained
        if (contains(element) == false)
        {
            // if the next poitner isn't null
            if (look_in_this_indexed_table->next == nullptr)
            {
                // make it a new node
                look_in_this_indexed_table->next = new LinkedListNode{element, nullptr};
            }
            else
            {
                // iterate over to find next null
                while (look_in_this_indexed_table->next != nullptr)
                {
                    look_in_this_indexed_table = look_in_this_indexed_table->next;
                }
                look_in_this_indexed_table->next = new LinkedListNode{element, nullptr};

            }
            // set bool to true
            array_of_added_to_index[index] = true;
            //increment
            current_sz++;
        }

    }

}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    if ((sz_for_load_factor / cap) >= 0.8)
    {
        // old capacity
        unsigned int old_cap = cap;
        // new capacity
        cap = old_cap*2; // DOUBLES THE CAPACITY
        // new pointer to hash table
        LinkedListNode** current_hash_table = array_of_LL; // old hash table // delete this
        bool* current_added_table = array_of_added_to_index; // old bool table // delete this too
        // Making a new hash table
        array_of_LL = new LinkedListNode*[cap];
        // reset sz to 0
        current_sz = 0;
        sz_for_load_factor = 0;
        // Making new bool table
        array_of_added_to_index = new bool[cap];
        for (int x = 0; x < cap; ++x)
        {
            // making all index a new node
            array_of_LL[x] = new LinkedListNode{};
            array_of_LL[x]->next = nullptr;
            array_of_added_to_index[x] = false;
        }
        // shoving the old elements into new table
        for (int i = 0; i < old_cap; ++i)
        {
            LinkedListNode* indexed_old = current_hash_table[i];
            if (current_added_table[i] == true)
            {
                while (indexed_old->next != nullptr)
                {
                    shove_it(indexed_old->value);
                    indexed_old = indexed_old->next;
                }
            }
        }

        // shoving the new element in
        shove_it(element);
        // Destruct the old
        for (int i = 0; i < old_cap; ++i)
        {
            LinkedListNode* current = current_hash_table[i];
            if (current_added_table[i] == false)
            {
                delete current;
            }
            else
            {
                while (current->next != nullptr)
                {

                    LinkedListNode* del = current;
                    current = current->next;
                    delete del;
                }
            }
        }
        delete[] current_added_table;
        delete[] current_hash_table;
            
    }
    // regular shove it
    else
    {
        shove_it(element);
    }
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    // index
    int hashed_index = static_cast<int>(hashFunction(element)%cap);
    // specifically look in hashed index
    LinkedListNode* indexed_table = array_of_LL[hashed_index];
    LinkedListNode* next;
    // iterate to find true
    while (indexed_table != nullptr)
    {
        if (indexed_table->value == element)
        {
            return true;
        }
        next = indexed_table->next;
        indexed_table = next;
        
    }
    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    // return sz counter
    return current_sz;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    // returns 0 if index is out of range
    if (index > cap)
    {
        return 0;
    }
    // starts counter
    else
    {
        int counter = 0;
        LinkedListNode* indexed_table = array_of_LL[index];
        LinkedListNode* next;
        while (indexed_table->next != nullptr)
        {
            // std::cout << indexed_table->value << std::endl;
            counter++;
            next = indexed_table->next;
            indexed_table = next;
        }
        // assurance for correct number
        if (array_of_added_to_index[index] == true)
        {
            counter++;
        }
        return counter;
    }
    return 0;
    
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    // iterate to find node at index
    LinkedListNode* indexed_table = array_of_LL[index];
    while (indexed_table != nullptr)
    {
        if (indexed_table->value == element)
        {
            return true;
        }
        else
        {
            indexed_table = indexed_table->next;
        }
    }
    return false;
}



#endif // HASHSET_HPP

