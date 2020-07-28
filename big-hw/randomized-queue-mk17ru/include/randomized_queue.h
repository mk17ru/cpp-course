#pragma once
#include<array>
#include <algorithm>
#include <random>
#include <iterator>
#include<vector>
#include <functional>
#include <chrono>

namespace gen {
    static std::random_device rd;
}

template <class T>
class randomized_queue {

public:
    class iterator {
        friend randomized_queue;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = const T;
        using pointer = value_type *;
        using reference = value_type &;
        using iterator_category = std::forward_iterator_tag;

        iterator(randomized_queue<T> &, size_t);

        T &operator*();

        iterator operator++(int);

        iterator operator--(int);

        iterator &operator++();

        iterator &operator--();

        T *operator -> ();

        friend bool operator == (const iterator & lhs, const iterator & rhs)
        {
            return lhs.cur_pos == rhs.cur_pos;
        }
        friend bool operator != (const iterator & lhs, const iterator  & rhs)
        {
            return !(lhs == rhs);
        }

    private:
        randomized_queue<T> & queue;
        size_t cur_pos;
        std::vector<int> perm;
    };

    class const_iterator {
        friend randomized_queue;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = const T;
        using pointer = value_type *;
        using reference = value_type &;
        using iterator_category = std::forward_iterator_tag;
        const_iterator(const randomized_queue<T> &, size_t);

        const T &operator*();

        const const_iterator operator++(int);

        const_iterator operator--(int);

        const_iterator &operator++();

        const_iterator &operator--();

        const T *operator -> ();

        friend bool operator == (const const_iterator & lhs, const const_iterator  & rhs)
        {
            return lhs.cur_pos == rhs.cur_pos;
        }
        friend bool operator != (const const_iterator & lhs, const const_iterator  & rhs)
        {
            return !(lhs == rhs);
        }

    private:
        const randomized_queue<T> & queue;
        size_t cur_pos;
        std::vector<int> perm;
    };

    public:

        size_t size() const;

        T & sample();

        T const & sample() const;

        void enqueue(const T &);

        void enqueue(T &&);

        T dequeue();

        bool empty() const;

        iterator begin();

        iterator end();

        const_iterator begin() const;

        const_iterator end() const;

        const_iterator cbegin();

        const_iterator cend();

    private:
        std::vector<T> data;
        static std::mt19937 g;
};

template<typename T>
std::mt19937 randomized_queue<T>::g(gen::rd());

template<typename T>
bool randomized_queue<T>::empty() const
{
    return size() == 0;
}

template<typename T>
T const & randomized_queue<T>::sample() const
{
    std::uniform_int_distribution<int> dist(0, size() - 1);
    return data[dist(g)];
}

template<typename T>
size_t randomized_queue<T>::size() const
{
    return data.size();
}

template<typename T>
void randomized_queue<T>::enqueue(const T &x)
{
    data.push_back(x);
}

template<typename T>
void randomized_queue<T>::enqueue(T &&x)
{
    data.push_back(std::move(x));
}

template<typename T>
T & randomized_queue<T>::sample()
{
    std::uniform_int_distribution<int> dist(0, size() - 1);
    return data[dist(g)];
}

template<typename T>
T randomized_queue<T>::dequeue()
{
    std::uniform_int_distribution<int> dist(0, size() - 1);
    T ans = std::exchange(data[dist(g)], std::move(data[size() - 1]));
    data.pop_back();
    return ans;
}

template<typename T>
typename randomized_queue<T>::iterator randomized_queue<T>::begin()
{
    return iterator(*this, 0);
}

template<typename T>
typename randomized_queue<T>::iterator randomized_queue<T>::end()
{
    return iterator(*this, size());
}

template<typename T>
typename randomized_queue<T>::const_iterator randomized_queue<T>::begin() const
{
    return const_iterator(*this, 0);
}

template<typename T>
typename randomized_queue<T>::const_iterator randomized_queue<T>::end() const
{
    return const_iterator(*this, size());
}

template<typename T>
typename randomized_queue<T>::const_iterator randomized_queue<T>::cbegin()
{
    return const_iterator(*this, 0);
}

template<typename T>
typename randomized_queue<T>::const_iterator randomized_queue<T>::cend()
{
    return const_iterator(*this, size());
}


template<class T>
randomized_queue<T>::iterator::iterator(randomized_queue<T> & queue, size_t position)
: queue(queue), cur_pos(position)
{
    if (position == 0) {
        perm.resize(queue.size());
        size_t len = queue.size();
        for (size_t i = 0; i < len; ++i) {
            perm[i] = i;
        }
        std::shuffle(perm.begin(), perm.end(), g);
    }
}

template<class T>
randomized_queue<T>::const_iterator::const_iterator(const randomized_queue<T> & queue, size_t const position)
        : queue(queue), cur_pos(position)
{
    if (position == 0) {
        perm.resize(queue.size());
        size_t len = queue.size();
        for (size_t i = 0; i < len; ++i) {
            perm[i] = i;
        }
        std::shuffle(perm.begin(), perm.end(), g);
    }
}

template<class T>
T &randomized_queue<T>::iterator::operator*(){
    return queue.data[perm[cur_pos]];
}

template<class T>
const T &randomized_queue<T>::const_iterator::operator*(){
    return queue.data[perm[cur_pos]];
}

template<class T>
typename randomized_queue<T>::iterator &randomized_queue<T>::iterator::operator++() {
    ++cur_pos;
    return *this;
}

template<class T>
typename randomized_queue<T>::const_iterator &randomized_queue<T>::const_iterator::operator++() {
    ++cur_pos;
    return *this;
}

template<class T>
typename randomized_queue<T>::iterator &randomized_queue<T>::iterator::operator--() {
    --cur_pos;
    return *this;
}

template<class T>
typename randomized_queue<T>::const_iterator &randomized_queue<T>::const_iterator::operator--() {
    --cur_pos;
    return *this;
}

template<class T>
typename randomized_queue<T>::iterator randomized_queue<T>::iterator::operator++(int) {
    iterator iter = *this;
    ++*this;
    return iter;
}

template<class T>
const typename randomized_queue<T>::const_iterator randomized_queue<T>::const_iterator::operator++(int) {
    const_iterator iter = *this;
    ++*this;
    return iter;
}

template<class T>
typename randomized_queue<T>::iterator randomized_queue<T>::iterator::operator--(int) {
    iterator iter = *this;
    --*this;
    return iter;
}

template<class T>
typename randomized_queue<T>::const_iterator randomized_queue<T>::const_iterator::operator--(int) {
   const_iterator iter = *this;
    --*this;
    return iter;
}

template<class T>
T *randomized_queue<T>::iterator::operator->() {
    return queue.data[perm[cur_pos]].operator -> ();
}

template<class T>
const T *randomized_queue<T>::const_iterator::operator->() {
    return queue.data[perm[cur_pos]].operator -> ();
}


