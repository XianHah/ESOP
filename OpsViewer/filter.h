#ifndef __FILTER_H__
#define __FILTER_H__
#endif
#include <vector>

class mean_filter{
private:
    std::vector<int> filter_buff;
public:
    mean_filter(int filter_size=10);
    float feed(float);
    ~mean_filter();
    void mean_filter_test();
};

