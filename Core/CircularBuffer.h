#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <stdexcept>


template <class T>
class CircularBuffer
{
    private:
        // Flags
        unsigned int current;
        unsigned int allocated;     // Contador de cantidad de alocaciones, tras reseteo. Sube hasta size

        // Raw data
        unsigned int size;
        T* data;

    public:
        CircularBuffer(const unsigned int size);
        ~CircularBuffer();

        // Set with push at front
        void push(const T& new_element);
        void erase();

        // Getters
        T front() const;
        T operator()(const unsigned int index) const;
        bool full() const;
};


// Definitions ----------------------------------------------------------
template <class T>
CircularBuffer<T>::CircularBuffer(const unsigned int size) : size(size), current(size - 1), allocated(0)        // Para asegurarse de 1ra alocacion correcta
{
    data = new T[size];
}

template <class T>
CircularBuffer<T>::~CircularBuffer()
{
    delete[] data;
}


// Setters
template <class T>
void CircularBuffer<T>::push(const T& new_element)
{
    current = (current + 1) % size;
    data[current] = new_element;

    if (allocated < size)
        allocated++;
}


// Doesn't actually erase the data, just alters the flags
template <class T>
void CircularBuffer<T>::erase()
{
    current = size - 1;
    allocated = 0;
}


// Getters
template <class T>
T CircularBuffer<T>::front() const
{
    return data[current];
}

// 0 -> first. size-1 -> front
template <class T>
T CircularBuffer<T>::operator()(const unsigned int index) const
{
    if (index >= size)
        throw std::out_of_range("Index out of range");

    unsigned int position = (current + 1 + index) % 32;
    return data[position];
}

template <class T>
bool CircularBuffer<T>::full() const
{
    return allocated == size;
}


#endif // CIRCULARBUFFER_H
