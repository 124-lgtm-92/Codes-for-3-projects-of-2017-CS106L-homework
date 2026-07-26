/**
 * File: KDTree.h
 * Author: (your name here)
 * ------------------------
 * An interface representing a kd-tree in some number of dimensions. The tree
 * can be constructed from a set of data and then queried for membership and
 * nearest neighbors.
 */

#ifndef KDTREE_INCLUDED
#define KDTREE_INCLUDED

#include "Point.h"
#include "BoundedPQueue.h"
#include <stdexcept>
#include <cmath>
#include <memory>
#include<unordered_map>
#include<tuple>

 // "using namespace" in a header file is conventionally frowned upon, but I'm
 // including it here so that you may use things like size_t without having to
 // type std::size_t every time.
using namespace std;

template <size_t N, typename ElemType>
class KDTree {
public:
    // Constructor: KDTree();
    // Usage: KDTree<3, int> myTree;
    // ----------------------------------------------------
    // Constructs an empty KDTree.
    KDTree();

    // Destructor: ~KDTree()
    // Usage: (implicit)
    // ----------------------------------------------------
    // Cleans up all resources used by the KDTree.
    ~KDTree();

    // KDTree(const KDTree& rhs);
    // KDTree& operator=(const KDTree& rhs);
    // Usage: KDTree<3, int> one = two;
    // Usage: one = two;
    // -----------------------------------------------------
    // Deep-copies the contents of another KDTree into this one.
    KDTree(const KDTree& rhs);
    KDTree& operator=(const KDTree& rhs);

    // size_t dimension() const;
    // Usage: size_t dim = kd.dimension();
    // ----------------------------------------------------
    // Returns the dimension of the points stored in this KDTree.
    size_t dimension() const;

    // size_t size() const;
    // bool empty() const;
    // Usage: if (kd.empty())
    // ----------------------------------------------------
    // Returns the number of elements in the kd-tree and whether the tree is
    // empty.
    size_t size() const;
    bool empty() const;

    // bool contains(const Point<N>& pt) const;
    // Usage: if (kd.contains(pt))
    // ----------------------------------------------------
    // Returns whether the specified point is contained in the KDTree.
    bool contains(const Point<N>& pt) const;

    // void insert(const Point<N>& pt, const ElemType& value);
    // Usage: kd.insert(v, "This value is associated with v.");
    // ----------------------------------------------------
    // Inserts the point pt into the KDTree, associating it with the specified
    // value. If the element already existed in the tree, the new value will
    // overwrite the existing one.
    void insert(const Point<N>& pt, const ElemType& value);

    // ElemType& operator[](const Point<N>& pt);
    // Usage: kd[v] = "Some Value";
    // ----------------------------------------------------
    // Returns a reference to the value associated with point pt in the KDTree.
    // If the point does not exist, then it is added to the KDTree using the
    // default value of ElemType as its key.
    ElemType& operator[](const Point<N>& pt);

    // ElemType& at(const Point<N>& pt);
    // const ElemType& at(const Point<N>& pt) const;
    // Usage: cout << kd.at(v) << endl;
    // ----------------------------------------------------
    // Returns a reference to the key associated with the point pt. If the point
    // is not in the tree, this function throws an out_of_range exception.
    ElemType& at(const Point<N>& pt);
    const ElemType& at(const Point<N>& pt) const;

    // ElemType kNNValue(const Point<N>& key, size_t k) const
    // Usage: cout << kd.kNNValue(v, 3) << endl;
    // ----------------------------------------------------
    // Given a point v and an integer k, finds the k points in the KDTree
    // nearest to v and returns the most common value associated with those
    // points. In the event of a tie, one of the most frequent value will be
    // chosen.
    ElemType kNNValue(const Point<N>& key, size_t k) const;



private:
    // TODO: Add implementation details here.
    size_t nodenum = 0;
    struct Node
    {

        Point<N> p1;
        ElemType val;
        Node(Point<N>obj1, ElemType obj2) :val(obj2)
        {
            copy(obj1.begin(), obj1.end(), p1.begin());
        }
        shared_ptr<Node> left = NULL;
        shared_ptr<Node> right = NULL;
    };
    shared_ptr<Node> root = NULL;
    shared_ptr<Node> findNode(const Point<N>& pt)const;
    void Recursivelysearch(const shared_ptr<Node>& Root, const Point<N>& key, size_t tag, BoundedPQueue<Point<N>>& obj)const;
    void Recursivelycopy(const shared_ptr<Node>&currroot,shared_ptr<Node>&target);
};

/** KDTree class implementation details */

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree() {

}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::~KDTree() {

}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::dimension() const {
    // TODO: Fill this in.
    return N;
}

// TODO: finish the implementation of the rest of the KDTree class
template<size_t N, typename ElemType>
size_t KDTree<N, ElemType>::size() const
{
    return nodenum;
}

template<size_t N, typename ElemType>
bool KDTree<N, ElemType>::empty() const
{
    return nodenum == 0;
}

template<size_t N, typename ElemType>
bool KDTree<N, ElemType>::contains(const Point<N>& pt) const
{
    if (findNode(pt) != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template<size_t N, typename ElemType>
void KDTree<N, ElemType>::insert(const Point<N>& pt, const ElemType& value)
{
    if (root == NULL)
    {
        root = make_shared<Node>(pt, value);
        nodenum += 1;
    }
    else
    {
        shared_ptr<Node>curr = root;
        if (pt == (*curr).p1)
        {
            (*curr).val = value;
            return;
        }
        size_t tag = 0;
        bool light1 = true;
        bool light2 = true;
        while (light1 && light2)
        {
            while (pt[(tag) % N] < (*curr).p1[(tag) % N])//注意N可能为0的情况
            {
                if ((*curr).left == NULL)
                {
                    (*curr).left = make_shared<Node>(pt, value);
                    nodenum += 1;
                    light1 = false;
                    break;
                }
                else
                {
                    if (pt == (*((*curr).left)).p1)
                    {
                        (*((*curr).left)).val = value;
                        light1 = false;
                        break;
                    }
                    curr = (*curr).left;
                    tag += 1;

                }

            }
            if (!light1)
            {
                break;
            }
            while (pt[(tag) % N] >= (*curr).p1[(tag) % N])
            {
                if ((*curr).right == NULL)
                {
                    (*curr).right = make_shared<Node>(pt, value);
                    nodenum += 1;
                    light2 = false;
                    break;
                }
                else
                {
                    if (pt == (*((*curr).right)).p1)
                    {
                        (*((*curr).right)).val = value;
                        light2 = false;
                        break;
                    }
                    curr = (*curr).right;
                    tag += 1;
                }
            }
            if (!light2)
            {
                break;
            }
        }
    }

}

template<size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::operator[](const Point<N>& pt)
{
    if (findNode(pt) == NULL)
    {
        insert(pt, ElemType{});
    }
    return (*findNode(pt)).val;
}

template<size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::at(const Point<N>& pt)
{
    if (findNode(pt) == NULL)
    {
        throw out_of_range("KDTree::at: point not found in tree");
    }
    return (*findNode(pt)).val;
}

template<size_t N, typename ElemType>
const ElemType& KDTree<N, ElemType>::at(const Point<N>& pt)const
{
    if (findNode(pt) == NULL)
    {
        throw out_of_range("KDTree::at: point not found in tree");
    }
    return (*findNode(pt)).val;
}
template<size_t N, typename ElemType>
shared_ptr<typename KDTree<N, ElemType>::Node> KDTree<N, ElemType>::findNode(const Point<N>& pt) const
{
    if (root == NULL)
    {
        return NULL;
    }
    else
    {
        shared_ptr<Node>curr = root;
        size_t tag = 0;

        if ((*curr).p1 == pt)
        {
            return curr;
        }
        while (true)
        {
            while (pt[(tag) % N] < (*curr).p1[(tag) % N])//注意N可能为0的情况
            {
                if ((*curr).left == NULL)
                {
                    return NULL;
                }
                else
                {
                    if (pt == (*((*curr).left)).p1)
                    {
                        return (*curr).left;
                    }
                    curr = (*curr).left;
                    tag += 1;

                }

            }

            while (pt[(tag) % N] >= (*curr).p1[(tag) % N])
            {
                if ((*curr).right == NULL)
                {
                    return NULL;
                }
                else
                {
                    if (pt == (*((*curr).right)).p1)
                    {
                        return (*curr).right;
                    }
                    curr = (*curr).right;
                    tag += 1;
                }
            }

        }
    }

}

template<size_t N, typename ElemType>
ElemType KDTree<N, ElemType>::kNNValue(const Point<N>& key, size_t k) const
{
    BoundedPQueue<Point<N>>bpq(k);
    Recursivelysearch(root, key, 0, bpq);
    unordered_map<ElemType,int>countfrequency;
    auto copy=bpq.makecopy();
    for (auto beginner = copy.begin(); beginner != copy.end(); ++beginner)
    {
        tuple<double, Point<N>>my_tuple = *beginner;
        auto result = countfrequency.insert({(*findNode(get<1>(my_tuple))).val,1});
        if (result.second == false)
        {
            countfrequency[(*findNode(get<1>(my_tuple))).val] += 1;
        }
    }
    auto beginner = countfrequency.begin();
    auto pos = beginner;
    for (; beginner != countfrequency.end(); ++beginner)
    {
        if ((*beginner).second > (*pos).second)
        {
            pos = beginner;
        }
    }
    return (*pos).first;

}
template<size_t N, typename ElemType>
void KDTree<N, ElemType>::Recursivelysearch(const shared_ptr<Node>& Root, const Point<N>& key, size_t tag, BoundedPQueue<Point<N>>& obj)const
{
    size_t copytag = tag;
    shared_ptr<Node>curr = Root;
    while (curr != NULL)
    {
        double distance = Distance((*curr).p1,key);
        obj.enqueue((*curr).p1, distance);
        if (obj.size() < obj.maxSize() || fabs((*curr).p1[copytag % N] - key[copytag % N]) < obj.worst())
        {
            shared_ptr<Node>copyleft = (*curr).left;
            shared_ptr<Node>copyright = (*curr).right;
            Recursivelysearch(copyleft, key, copytag + 1, obj);
            Recursivelysearch(copyright, key, copytag + 1, obj);
            return;
        }
        else
        {
            if (key[copytag % N] < (*curr).p1[copytag % N])
            {
                curr = (*curr).left;
                copytag += 1;
            }
            else
            {
                curr = (*curr).right;
                copytag += 1;
            }
        }
    }
    return;
}
template<size_t N, typename ElemType>
KDTree<N,ElemType>::KDTree(const KDTree<N,ElemType>& rhs)
{
    (*this).nodenum=rhs.nodenum;
    Recursivelycopy(rhs.root,(*this).root);
}
template<size_t N, typename ElemType>
KDTree<N,ElemType>& KDTree<N,ElemType>::operator=(const KDTree<N,ElemType>& rhs)
{
    if(this!=&rhs)
    {
        (*this).nodenum=rhs.nodenum;
        Recursivelycopy(rhs.root,(*this).root);
    }
    return *this;
}
template<size_t N,typename ElemType>
void KDTree<N,ElemType>::Recursivelycopy(const shared_ptr<Node>&currroot,shared_ptr<Node>&target)
{
    if(currroot==NULL)
    {
        return;
    }
    else
    {
        target=make_shared<Node>(*currroot);
        Recursivelycopy((*currroot).left,(*target).left);
        Recursivelycopy((*currroot).right,(*target).right);
        return;
    }
}

#endif // KDTREE_INCLUDED
