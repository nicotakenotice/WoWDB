#ifndef CONTAINER_H
#define CONTAINER_H

template<class T>
class Container {
private:
    unsigned int capacity;
    unsigned int size;
    T* array;

    // Methods
    void resize();

public:
    // Constructors/destructors
    Container();
    Container(const Container& cnt);
    ~Container();

    // Getters/setters
    unsigned int getCapacity() const;
    unsigned int getSize() const;

    // Operators overloading
    Container& operator=(const Container& cnt);
    bool operator==(const Container& cnt) const;
    bool operator!=(const Container& cnt) const;
    T& operator[](unsigned int i) const;

    // Methods
    void clear();
    int search(const T& obj) const;
    void insert(const T& obj);
    void remove(const T& obj);
    void remove(int pos);
};

/******************** CONSTRUCTORS/DESTRUCTORS ********************/
template<class T>
Container<T>::Container() : capacity(10), size(0), array(new T[capacity]) {}

template<class T>
Container<T>::Container(const Container& cnt) : capacity(cnt.capacity), size(cnt.size), array(new T[capacity]) {
    for (unsigned int i = 0; i < size; ++i)
        array[i] = cnt.array[i];
}

template<class T>
Container<T>::~Container<T>() { if (array) delete[] array; }


/******************** GETTERS/SETTERS ********************/
template<class T>
unsigned int Container<T>::getCapacity() const { return capacity; }

template<class T>
unsigned int Container<T>::getSize() const { return size; }


/******************** OPERATORS OVERLOADING ********************/
template<class T>
Container<T>& Container<T>::operator=(const Container& cnt) {
    if (this == &cnt)
        return *this;
    if (size > 0) delete[] array;
    capacity = cnt.capacity;
    size = cnt.size;
    array = new T[capacity];
    for (unsigned int i = 0; i < size; ++i)
        array[i] = cnt.array[i];
    return *this;
}

template<class T>
bool Container<T>::operator==(const Container& cnt) const {
    if (array && cnt.array) {
        if (size == cnt.size) {
            for (unsigned int i = 0; i < size; ++i) {
                if (array[i] != cnt.array[i])
                    return false;
            }
            return true;
        }
    }
    return false;
}

template<class T>
bool Container<T>::operator!=(const Container& cnt) const {
    return !(*this == cnt);
}

template<class T>
T& Container<T>::operator[](unsigned int i) const { return array[i]; }


/******************** METHODS ********************/
template<class T>
void Container<T>::clear() {
    delete[] array;
    capacity = 10;
    size = 0;
    array = new T[capacity];
}

template<class T>
void Container<T>::resize() {
    capacity = capacity + 10;
    T* temp = new T[capacity];
    for (unsigned int i = 0; i < size; ++i)
        temp[i] = array[i];
    delete[] array;
    array = temp;
}

template<class T>
int Container<T>::search(const T& obj) const {
    for (unsigned int i = 0; i < size; ++i) {
        if (array[i] == obj)
            return static_cast<int>(i);
    }
    return -1;
}

template<class T>
void Container<T>::insert(const T& obj) {
    if (size + 1 > capacity)
        resize();
    size++;
    array[size-1] = obj;
}

template<class T>
void Container<T>::remove(const T& obj) {
    if (size > 0) {
        int pos = search(obj);
        if (pos != -1) {
            unsigned int u_pos = static_cast<unsigned int>(pos);
            T* temp = new T[capacity];
            unsigned int j = 0;
            for (unsigned int i = 0; i < size; ++i) {
                if (i != u_pos) {
                    temp[j] = array[i];
                    j++;
                }
            }
            size--;
            delete[] array;
            array = temp;
        }
    }
}

template<class T>
void Container<T>::remove(int pos) {
    if (pos >=0 && pos < static_cast<int>(size)) {
        unsigned int u_pos = static_cast<unsigned int>(pos);
        T* temp = new T[capacity];
        unsigned int j = 0;
        for (unsigned int i = 0; i < size; ++i) {
            if (i != u_pos) {
                temp[j] = array[i];
                j++;
            }
        }
        size--;
        delete[] array;
        array = temp;
    }
}

#endif // CONTAINER_H
