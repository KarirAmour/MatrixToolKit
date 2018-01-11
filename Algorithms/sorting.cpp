#include <vector>
#include <iostream>

using namespace std;




void printVector(const std::vector<int> &vec) {
    for (auto ele : vec) {
        std::cout << ele << " ";
    }
    std::cout << std::endl;
}


typedef std::vector<int>::iterator VecIter;
typedef std::vector<int> Vector;


VecIter &merge_sort_iter(VecIter &start, VecIter &end) {

    if (start - end <= 1) {
        return start; // return iterator?
    } else if (start - end == 2) {
        if (*start >= *end) {
            int tmp = *start;
            *start = *end;
            *end = tmp;
        }
        return start;
    }

    VecIter midpoint =  start + (start - end) / 2;

    VecIter &left = merge_sort_iter(start, midpoint);
    midpoint += 1;
    VecIter &right = merge_sort_iter(midpoint, end);

    Vector merge;
    while (left < midpoint and right < end) {
        if (*left > *right) {
            merge.push_back(*right);
            ++right;
        } else {
            merge.push_back(*left);
            ++left;
        }
    }
    // Push remaining sorted items
    if (left == midpoint) { 
        while (right < end) {
            merge.push_back(*right);
            ++right;
        }
    } else if (right == end) { 
        while (left < midpoint) {
            merge.push_back(*left);
            ++left;
        }
    }

    VecIter loop = start;
    VecIter item = merge.begin();
    while (loop < end) {
        *loop = *item;
        ++loop;
        ++item;
    }

    return start;
    
}





int main() {


    std::vector<int> lst1 = {1, 2, 5, 3, 2, 1, 6, 8, 4, 0, 6};
    printVector(lst1);
    VecIter start = lst1.begin();
    VecIter end = lst1.end();
    merge_sort_iter(start, end);
    printVector(lst1);

    return 0;
}