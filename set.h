#ifndef SET_H
#define SET_H
#include <iostream>
#include "bplustree.h"
using namespace std;

template <typename V>
class Set
{
public:
    class Iterator
    {
    public:
        friend class Set;

        Iterator(typename BPlusTree<V>::Iterator _it) { _treeIt = _it; }

        Iterator operator ++()
        {
            if(!_treeIt.is_null())
                ++_treeIt;

            return *this;
        }

        Iterator operator ++(int unused)
        {
            Iterator temp = *this;
            this->operator++();
            return temp;
        }

        V & operator *() { return *_treeIt; }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) { return(lhs._treeIt == rhs._treeIt);}
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) { return(lhs._treeIt != rhs._treeIt); }

    private:
        typename BPlusTree<V>::Iterator _treeIt;
    };

    Set(): _set(false) {}

    //Intersection / Union
    void intersection(Set<V>& other, Set<V>& overlap);
    Set<V>& operator+=(Set<V>& other);

    //Capacity
    int size() const;
    bool empty() const;

    //Modifiers
    bool insert(const V& v);
    bool erase(const V& v);
    void clear();

    //Operations:
    bool contains(const V &v) const;
    bool isValid(){return _set.isValid();}

    friend ostream& operator<<(ostream& outs, const Set<V>& printMe)
    {
        outs<<printMe._set<<endl;
        return outs;
    }

    //Iterators
    Iterator begin(){return Set<V>::Iterator(_set.begin());}
    Iterator end(){return Set<V>::Iterator(_set.end());}

private:
    BPlusTree<V> _set;
};

//preconditions: none
//postconditions: common indicies of this and other will be
// inserted to overlap.
template <typename V>
void Set<V>::intersection(Set<V>& other, Set<V>& overlap)
{
    Set<V>::Iterator it = this->begin();
    Set<V>::Iterator it2 = other.begin();
    Set<V>::Iterator end = this->end();

    while(it != end && it2 != end)
    {
        if(*it2 == *it)
        {
            overlap.insert(*it2);
            ++it;
            ++it2;
        }
        else if(*it2 < *it)
            ++it2;
        else
            ++it;
    }
}

//preconditions: none
//postconditions: indicies in other will be merged into this set
// no duplicates will be inserted.
template <typename V>
Set<V>& Set<V>::operator+=(Set<V>& other)
{
    Set<V>::Iterator it = other.begin();
    while(it != other.end())
    {
        this->insert(*it);
        ++it;
    }
    return *this;
}

//preconditions: none
//postconditions: return the size of the BTree (i.e., the map)
template <typename V>
int Set<V>::size() const
{
    return _set.size();
}

//preconditions: none
//postconditions: if the BTree is empty, return true, otherwise false.
template <typename V>
bool Set<V>::empty() const
{
    return (_set.size() == 0);
}

//preconditions: none
//postconditions: if duplicates are allowed in the BTree, or the value
// associated with the recieved key is still default constructed
// (indicating that get could not find an existing pair, so it inserted a new one)
// then reassign the value the recieved value (v), and return true.
// otherwise, return false.
template<typename V>
bool Set<V>::insert(const V &val)
{
    if(!_set.contains(val))
        return _set.insert(val);
    else
        return false;
}

//preconditions: none
//postconditions: removes the pair with the recieved key from the map,
// returning true if the pair was removed, otherwise false.
template<typename V>
bool Set<V>::erase(const V &val)
{
    return _set.remove(val);
}

//preconditions: none
//postconditions: calls clear on the BTree, erasing all items from it.
template<typename V>
void Set<V>::clear()
{
    _set.clearTree();
}

//preconditions: none
//postconditions: returns true if the val exists in the _set, otherwise false.
template<typename V>
bool Set<V>::contains(const V &val) const
{
    return _set.contains(val);
}

#endif // SET_H
