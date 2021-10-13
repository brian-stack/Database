#ifndef MULTIMAP_H
#define MULTIMAP_H
#include "bplustree.h"
#include "set.h"
#include "constants.h"
using namespace std;

template <typename K, typename V>
class MPair
{
public:
    //constructors:
    MPair() : _key() {}
    MPair(const K& k) : _key(k) {}
    MPair(const K& k, const V& v) : _key(k) { _values.push_back(v); }
    MPair(const K& k, const vector<V>& vlist) : _key(k), _values(vlist) {}

    //public accessors
    K key() { return _key; }
    Set<V>& values() { return _values; }

    friend std::ostream& operator <<(std::ostream& outs, const MPair<K, V>& printMe)
    {
        outs << printMe._key << ": " << printMe._values;
        return outs;
    }

    //comparison operators
    friend bool operator ==(const  MPair<K, V>& lhs, const MPair<K, V>& rhs) { return (lhs._key == rhs._key); }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs) { return (lhs._key < rhs._key); }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs) { return (lhs._key > rhs._key); }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){ return (lhs._key <= rhs._key); }
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs) { return (lhs._key >= rhs._key); }

private:
    K _key;
    Set<V> _values;
};

template <typename K, typename V>
class MMap
{
public:
    class Iterator
    {
    public:
        friend class MMap;

        Iterator(typename BPlusTree<MPair<K,V> >::Iterator it) : _treeIt(it) {}

        Iterator operator ++(int unused)
        {
            Iterator temp = *this;
            this->operator++();
            return temp;
        }

        Iterator operator ++()
        {
            _treeIt++;
            return *this;
        }

        MPair<K,V>& operator *() { return *_treeIt; }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) { return(lhs._treeIt == rhs._treeIt); }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) { return(lhs._treeIt != rhs._treeIt); }

    private:
        typename BPlusTree<MPair<K,V> >::Iterator _treeIt;
    };

public:
    MMap() : _mmap(true){}

    //  Capacity
    int size() const;
    bool empty() const;

    //  Accessors
    const Set<V>& operator[](const K& key) const;
    Set<V>& operator[](K key);
    Set<V> &get(const K& key);

    //  Modifiers
    bool insert(const K& k, const V& v);
    bool erase(const K& key);
    void clear();

    //  Operations:
    bool contains(const K& key) const;
    int count(const K& key);
    bool isValid();

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me)
    {
        outs<<print_me._mmap<<endl;
        return outs;
    }

    Iterator lowerBound(K key) { return Iterator(_mmap.lowerBoundIt(key)); }  //returns an iterator to the first item >= the key.
    Iterator upperBound(K key) { return Iterator(_mmap.upperBoundIt(key)); }  //returns an iterator to the first item > the key.
    Iterator begin() { return MMap<K,V>::Iterator(_mmap.begin());}            //returns an iterator to the first item in the map
    Iterator end() { return MMap<K,V>::Iterator(_mmap.end()); }               //returns an iterator to null.

    //returns an iterator at the target key, null if it does not exist.
    Iterator iteratorAtKey(K key) { return MMap<K,V>::Iterator(_mmap.getIteratorAtEntry(_mmap.find(key))); }

private:
    BPlusTree<MPair<K,V> > _mmap;    
};

//preconditions: none
//postconditions: returns the total number of keys in the MMap.
template<typename K, typename V>
int MMap<K,V>::size() const
{
    return _mmap.size();
}

//preconditions: none
//postconditions: returns true if the BTree is empty, otherwise false.
template<typename K, typename V>
bool MMap<K,V>::empty() const
{
    return (_mmap.size() == 0);
}

//preconditions: none
//postconditions: the set of the associated key will be
// returned from the BTree, if no MPair with the recieved key
// already exists, an Mpair containing an empty set will be inserted.
template<typename K, typename V>
const Set<V>& MMap<K,V>::operator[](const K &key) const
{
    return _mmap.get(MPair<K,V>(key)).values();
}

//preconditions: none
//postconditions: the set of the associated key will be
// returned from the BTree, if no MPair with the recieved key
// already exists, an Mpair containing an empty set will be inserted.
template<typename K, typename V>
Set<V>& MMap<K,V>::operator[](K key)
{
    return _mmap.get(MPair<K,V>(key)).values();
}

//preconditions: none
//postconditions: obtain the set of values associated with the recieved key,
// if it already exists, otherwise it will be created now.
// Then, insert the value to the set.
template<typename K, typename V>
bool MMap<K,V>::insert(const K &k, const V &v)
{
    Set<V> * temp = &(this->get(k));
    temp->insert(v);
    return true;
}

//preconditions: none
//postconditions: removes the Mpair with the recieved key from the map,
// returning true if the pair was removed, otherwise false.
template<typename K, typename V>
bool MMap<K,V>::erase(const K &key)
{
    return _mmap.remove(MPair<K,V>(key));
}

//preconditions: none
//postconditions: calls clear on the BTree, erasing all items from it.
template<typename K, typename V>
void MMap<K,V>::clear()
{
    _mmap.clearTree();
}

//preconditions: none
//postconditions: returns true if the target key exists in the Map, otherwise false.
template<typename K, typename V>
bool MMap<K,V>::contains(const K& key) const
{
    return _mmap.contains(MPair<K,V>(key));
}

//preconditions: none
//postconditions: the set of the associated key will be
// returned from the BTree, if no MPair with the recieved key
// already exists, an Mpair containing an empty set will be inserted.
template<typename K, typename V>
Set<V>& MMap<K,V>::get(const K& key)
{
    return _mmap.get(MPair<K,V>(key)).values();
}

//preconditions: none
//postconditions: returns the size of the vector associated with the key
template<typename K, typename V>
int MMap<K,V>::count(const K& key)
{
    return _mmap.get(MPair<K,V>(key)).values().size();
}

//preconditions: none
//postconditions: returns true if all the conditions
// required for a valid BTree are met, otherwise false.
template<typename K, typename V>
bool MMap<K,V>::isValid()
{
    return _mmap.isValid();
}

#endif // MULTIMAP_H
