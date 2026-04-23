#ifndef SRC_HPP
#define SRC_HPP
#include <iostream>

struct Permutation { // 置换
    int* mapping; // 我觉得这里存啥你应该知道
    size_t size; // 上面那个数组的长度

    Permutation(size_t size) {
        // TODO: 初始化一个长度为size的置换，内容为恒等置换。
        this->size = size;
        mapping = new int[size];
        for (size_t i = 0; i < size; ++i) {
            mapping[i] = i;
        }
    }

    Permutation(const int* mapping, size_t size) {
        // TODO: 初始化一个置换，内容为mapping，大小为size
        this->size = size;
        this->mapping = new int[size];
        for (size_t i = 0; i < size; ++i) {
            this->mapping[i] = mapping[i];
        }
    }

    ~Permutation() {
        // TODO: 析构函数，合理管理你的内存😈
        delete[] mapping;
    }

    void apply(int* permutation) const {
        // TODO：对于大小为size的数组permutation，将置换应用到它上，直接修改permutation指向的数组。
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
        // TODO: 返回两个置换的乘积，即this*other。保证this和other的大小相同。
        int* result_mapping = new int[size];
        for (size_t i = 0; i < size; ++i) {
            result_mapping[i] = this->mapping[other.mapping[i]];
        }
        Permutation result(result_mapping, size);
        delete[] result_mapping;
        return result;
    }

    Permutation inverse() const {
        // TODO: 返回该置换的逆置换
        int* inverse_mapping = new int[size];
        for (size_t i = 0; i < size; ++i) {
            inverse_mapping[mapping[i]] = i;
        }
        Permutation result(inverse_mapping, size);
        delete[] inverse_mapping;
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Permutation& p) {
        // 这里重载了输出流，使得输出更方便。这里不需要修改代码。
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

struct Transposition { // 对换
    int a, b;

    Transposition() {
        // TODO: 初始化对换为恒等对换
        a = b = 0;
    }

    Transposition(int a, int b)  {
        // TODO: 初始化对换为{a,b}
        this->a = a;
        this->b = b;
    }

    void apply(int* permutation, size_t size) const {
        // TODO: 将对换应用permutation上，直接修改permutation指向的数组。保证size>a且size>b。
        if (a >= 0 && a < size && b >= 0 && b < size) {
            int temp = permutation[a];
            permutation[a] = permutation[b];
            permutation[b] = temp;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Transposition& t) {
        // 这里重载了输出流，使得输出更方便。这里不需要修改代码。
        os << "{" << t.a << " " << t.b << "}";
        return os;
    }

    Permutation toPermutation(size_t size) const {
        // TODO: 将对换转换为大小为size的置换。保证size>a且size>b。返回一个置换。
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

struct Cycle { // 轮换
    int* elements;// 我也觉得这里存啥你应该知道
    size_t size;

    Cycle(const int* elements, size_t size) {
        // TODO: 初始化一个轮换，内容为elements，大小为size
        this->size = size;
        this->elements = new int[size];
        for (size_t i = 0; i < size; ++i) {
            this->elements[i] = elements[i];
        }
    }

    ~Cycle() {
        // TODO: 析构函数，合理管理你的内存😈
        delete[] elements;
    }

    void apply(int* permutation, size_t size) const {
        // TODO: 将轮换应用大小为size的permutation上，直接修改permutation指向的数组。保证size>=elements[i]。
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
        // 这里重载了输出流，使得输出更方便。这里不需要修改代码。
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
        // TODO: 将轮换转换为大小为size的置换。保证size>=elements[i]。返回一个置换。
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