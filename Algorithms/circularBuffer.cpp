#include <iostream>
#include <vector>

using namespace std;

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

class CircularBuffer {

    int *buffer;
    int tail;
    size_t capacity;

public:
    CircularBuffer(size_t size) : capacity{size}, tail{0}, buffer{nullptr} {
        this->buffer = new int[capacity];
    }
    ~CircularBuffer() { delete[] this->buffer; }

    void naive_put(std::vector vec) {

        for (size_t i = 0; i < vec.size(); ++i) {
            this->data[(this->tail + i) % this->capacity] = vec.at(i);
        }
        this->tail = (this->tail + vec.size()) % this->capacity;
    }

    void less_naive_put(std::vector vec) {
        int start = 0;
        if (vec.size() > this->capacity) {
            start = vec.size() - this->capacity;
        }
        int i = this->tail;
        while (i < this->capacity) {
            this->data[i] = vec.at(i);
            ++i;
        }
        for (size_t i = 0; i < vec.size(); ++i) {
            this->data[(this->tail + i) % this->capacity] = vec.at(i);
        }
        this->tail = (this->tail + vec.size()) % this->capacity;
    }
    size_t size() const { return this->capacity; }
    
};

int main() {

    cout << "Hello World" << endl;

    return 0;
}