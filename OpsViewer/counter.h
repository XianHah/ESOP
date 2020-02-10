#ifndef __COUNTER_H__
#define __COUNTER_H__
#endif 
#include <math.h>
#include <vector>
#include <opencv2/opencv.hpp>

#include "trigger.h"
#include "filter.h"


class counter{
private:
    //select an ROI
    cv::Point roi_pt_1;
    cv::Point roi_pt_2;
    int roi_width;
    int roi_height;

    //divide roi into many single rectangle 
    void roi_divide();
    std::vector<cv::Rect> roi_box_list;
    std::vector<int>  roi_socre_list;
    
    cv::Mat roi_background;
    bool background_flag=0;

    //obj info
    int obj_width;
    std::vector<int> obj_width_list;
    int obj_temp_width=0;
    int size_num=0;         //current size
    int last_num=0;         //last size
    int obj_size=0;
    int obj_maxsize=4000;
    int obj_minsize=50;
    std::vector<int> obj_size_list;

    bool is_adjusting;


    //filter and trigger 
    mean_filter filter;
    trigger t;

    //num
    int num;
    int learning_num=10;


    //other for debug and test
    bool debug_window=false;

public:
    counter();
    ~counter();
    
    counter& set_roi(cv::Point pt1,cv::Point pt2){roi_pt_1=pt1;roi_pt_2=pt2;return *this;}
    cv::Rect get_roi(){cv::Rect roi(roi_pt_1,roi_pt_2);return roi;}

    counter& set_obj_width(int w){obj_width=w;return *this;}
    counter& set_obj_size(int s){obj_size=s;return *this;}
    counter& set_obj_maxsize(int s){obj_maxsize=s;return *this;}
    counter& set_obj_minsize(int s){obj_minsize=s;return *this;}
    int get_obj_width(){return obj_width;}
    

    counter& set_trigger_lh_thresold(int l,int h){t.set_h_threshold(h).set_l_threshold(h);return *this;}
    int get_trigger_h(){return t.get_h_threshold();}
    int get_trigger_l(){return t.get_l_threshold();}

    int get_num(){return num;}
    counter& num_reset(){num=0;return *this;}
    counter& set_leaning_num(int val){learning_num=val; return *this;}
    int get_leaning_num(){return learning_num;}

    void triger_start_cb(){obj_temp_width++;}
    void num_increase_cb();
    counter& set_trigger_cb(cb lh,cb hl){t.set_cb(lh,hl); return *this;}

    void init();
    void reinit();
    void feed(cv::Mat);
    cv::Mat draw(cv::Mat);
    counter& enable_debug_window(){debug_window=true;return *this;}
    counter& disable_debug_window(){debug_window=false;return *this;}

};
