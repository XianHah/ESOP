#include "filter.h"
#include <numeric>
#include <iostream>
mean_filter::mean_filter(int filter_size){
    filter_buff.resize(filter_size);
}

float mean_filter::feed(float val){
    //put
    filter_buff.erase(filter_buff.begin());
    filter_buff.push_back(val);
    //get 
    int sum=accumulate(filter_buff.begin(),filter_buff.end(),0);
    return float(sum)/filter_buff.size();
}

mean_filter::~mean_filter(){

}

void mean_filter::mean_filter_test(){
    for(int e : filter_buff){
        std::cout<<e<<",";
    }
    std::cout<<std::endl;
}