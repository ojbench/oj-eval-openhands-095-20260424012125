
#ifndef SRC_HPP
#define SRC_HPP
#include <iostream>

struct Permutation {
    int* mapping;
    size_t size;

    Permutation(size_t size) {
        this->size = size;
        mapping = new int[size];
        for (size_t i = 0; i < size; ++i) {
            mapping[i] = i;
        }
    }

    Permutation(const int* mapping, size_t size) {
        this->size = size;
        this->mapping = new int[size];
        for (size_t i = 0; i < size; ++i) {
            this->mapping[i] = mapping[i];
        }
    }

    ~Permutation() {
        delete[] mapping;
    }

    void apply(int* permutation) const {
        int* temp = new int[size];
        for (size_t i = 0; i < size; ++i) {
            temp[i] = permutation[mapping[i]];
        }
        for (size_t i = 0; i < size; ++i) {
            permutation[i] = temp[i];
        }
        delete[] temp;
    }

    Permutation operator*(const Permutation& other) const {
        int* result_mapping = new int[size];
        for (size_t i = 0; i < size; ++i) {
            result_mapping[i] = this->mapping[other.mapping[i]];
        }
        Permutation result(result_mapping, size);
        delete[] result_mapping;
        return result;
    }

    Permutation inverse() const {
        int* inverse_mapping = new int[size];
        for (size_t i = 0; i < size; ++i) {
            inverse_mapping[mapping[i]] = i;
        }
        Permutation result(inverse_mapping, size);
        delete[] inverse_mapping;
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Permutation& p) {
        os << "[";
        for (size_t i = 0; i < p.size; ++i) {
            os << p.mapping[i];
            if (i < p.size - 1) {
                os << " ";
            }
        }
        os << "]";
        return os;
    }
};

struct Transposition {
    int a, b;

    Transposition() {
        a = b = 0;
    }

    Transposition(int a, int b) {
        this->a = a;
        this->b = b;
    }

    void apply(int* permutation, size_t size) const {
        if (a >= 0 && a < size && b >= 0 && b < size) {
            int temp = permutation[a];
            permutation[a] = permutation[b];
            permutation[b] = temp;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Transposition& t) {
        os << "{" << t.a << " " << t.b << "}";
        return os;
    }

    Permutation toPermutation(size_t size) const {
        int* mapping = new int[size];
        for (size_t i = 0; i < size; ++i) {
            mapping[i] = i;
        }
        if (a >= 0 && a < size && b >= 0 && b < size) {
            mapping[a] = b;
            mapping[b] = a;
        }
        Permutation result(mapping, size);
        delete[] mapping;
        return result;
    }
};

struct Cycle {
    int* elements;
    size_t size;

    Cycle(const int* elements, size_t size) {
        this->size = size;
        this->elements = new int[size];
        for (size_t i = 0; i < size; ++i) {
            this->elements[i] = elements[i];
        }
    }

    ~Cycle() {
        delete[] elements;
    }

    void apply(int* permutation, size_t size) const {
        if (this->size == 0) return;
        
        int first_value = permutation[elements[0]];
        for (size_t i = 0; i < this->size - 1; ++i) {
            if (elements[i] < size && elements[i+1] < size) {
                permutation[elements[i]] = permutation[elements[i+1]];
            }
        }
        if (elements[this->size - 1] < size) {
            permutation[elements[this->size - 1]] = first_value;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Cycle& c) {
        os << "{";
        for (size_t i = 0; i < c.size; ++i) {
            os << c.elements[i];
            if (i < c.size - 1) {
                os << " ";
            }
        }
        os << "}";
        return os;
    }

    Permutation toPermutation(size_t size) const {
        int* mapping = new int[size];
        for (size_t i = 0; i < size; ++i) {
            mapping[i] = i;
        }
        
        for (size_t i = 0; i < this->size; ++i) {
            if (elements[i] < size) {
                size_t next_i = (i + 1) % this->size;
                if (elements[next_i] < size) {
                    mapping[elements[i]] = elements[next_i];
                }
            }
        }
        
        Permutation result(mapping, size);
        delete[] mapping;
        return result;
    }
};

#endif
