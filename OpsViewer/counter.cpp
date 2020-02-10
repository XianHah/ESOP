#include "counter.h"
#include <string>
counter::counter(){
    // t.set_cb(triger_start_cb,num_increase_cb);
    // t.set_h_threshold(8);
    // t.set_l_threshold(2);

}

counter::~counter(){
}

void counter::roi_divide(){
    try
    {
        roi_box_list.clear();
        std::cout<<"roi_width:"<<roi_width<<std::endl;
        std::cout<<"roi_height:"<<roi_width<<std::endl;
        for(int i=0;i<roi_width/roi_height;i++){
            roi_box_list.push_back(cv::Rect(
                cv::Point(roi_pt_1.x+i*roi_height,roi_pt_1.y),
                cv::Point(roi_pt_1.x+(i+1)*roi_height,roi_pt_2.y))
                );
        }
    }
    catch(const std::exception& e)
    {
        std::cerr <<"roi divide:"<< e.what() << '\n';
    }
    
}

void counter::init(){
    roi_width=roi_pt_2.x-roi_pt_1.x;
    roi_height=roi_pt_2.y-roi_pt_1.y;
    roi_divide();
    num_reset();
}

void counter::num_increase_cb(){
    try
    {
        //auto adapt 
        if(obj_width_list.size()<learning_num && obj_width==0 && obj_size==0){
            obj_width_list.push_back(obj_temp_width);
            obj_size_list.push_back(size_num);
        }
        if(obj_width_list.size()==learning_num && obj_width==0&& obj_size==0){
            // std::sort(obj_width_list.begin(),obj_width_list.end());
            // obj_width=obj_width_list[learning_num/2];

            std::sort(obj_size_list.begin(),obj_size_list.end());
            for(int i=0;i<obj_size_list.size();i++)
                obj_size+=obj_size_list[i];
            obj_size/=obj_size_list.size();
        }

        if(obj_width==0 && obj_size==0)
            num++;
        else{
            num+=size_num/obj_size;
            if(size_num%obj_size/float(obj_size)>0.8)num++;
        }
        obj_temp_width=0;
        last_num=size_num;
        size_num=0;
        //re-fresh background
        background_flag=0;
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void counter::feed(cv::Mat img){
    try
    {   
        cv::Mat o_img;
        img.copyTo(o_img);
        if(o_img.channels()!=1){
            cv::cvtColor(o_img,o_img,CV_BGR2GRAY);
        }
        cv::GaussianBlur(o_img,o_img,cv::Size(21,21),0);
        cv::Canny(o_img,o_img,20,40);

        //get background
        if(background_flag!=1){
            // int bscore=0;
            // for(cv::Rect i : roi_box_list){
            //     bscore+=cv::countNonZero(o_img(i));
            // }
            // if(bscore>0)return;
            img.copyTo(roi_background);
            background_flag=1;
        }

        //check background right
        if(obj_size>obj_maxsize || size_num>obj_maxsize){
            reinit();
            return;
        }

        cv::Mat d;
        cv::absdiff(img,roi_background,d);

        cv::cvtColor(d,d,CV_BGR2GRAY);
        cv::threshold(d,d,30,255,CV_THRESH_OTSU);


        if(debug_window){
            cv::imshow("o",o_img);
        }
       
        //auto adapt
        if(obj_temp_width>0){
            obj_temp_width+=1;
        }

        roi_socre_list.clear();
        int te_score=0;
        int tt_score=0;
 
        for(cv::Rect i : roi_box_list){
            int e_score=cv::countNonZero(o_img(i));
            cv::Mat temp;
            int t_score=cv::countNonZero(d(i));
            if(e_score>0){
                te_score+=1;
                e_score=1;
            }
            if(t_score>roi_height*roi_height/2){
                tt_score+=1;
                t_score=1;
            }else{
                t_score=0;
            }

            roi_socre_list.push_back((t_score>0?t_score:e_score));
            t_score>0?size_num+=t_score:size_num+=e_score;
        }
        
        //filter and trigger 
        t.feed(filter.feed(MAX(te_score,tt_score)));

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

cv::Mat counter::draw(cv::Mat img){
    //draw rectrangle
    cv::rectangle(img,cv::Rect(cv::Point(roi_pt_1),cv::Point(roi_pt_2)),cv::Scalar(0,0,255),2);
    //draw box 
    for(cv::Rect i : roi_box_list){
        cv::rectangle(img,i,cv::Scalar(0,255,0),1);
    }
    int index=0;
    for(int i: roi_socre_list){
        if(i>0){
            cv::rectangle(img,roi_box_list[index],cv::Scalar(0,255,0),8);
        }
        index++;
    }

    //put text 
    cv::putText(img,"NUM:"+std::to_string(num),cv::Point(0,40),CV_FONT_HERSHEY_COMPLEX,1,cv::Scalar(255,255,0),1);
    cv::putText(img,"WIDTH:"+std::to_string(obj_width),cv::Point(0,80),CV_FONT_HERSHEY_COMPLEX,1,cv::Scalar(255,255,0),1);
    cv::putText(img,"tWIDTH:"+std::to_string(obj_temp_width),cv::Point(0,120),CV_FONT_HERSHEY_COMPLEX,1,cv::Scalar(255,255,0),1);
    cv::putText(img,"size:"+std::to_string(size_num),cv::Point(0,160),CV_FONT_HERSHEY_COMPLEX,1,cv::Scalar(255,255,0),1);
    cv::putText(img,"lsize:"+std::to_string(last_num),cv::Point(0,200),CV_FONT_HERSHEY_COMPLEX,1,cv::Scalar(255,255,0),1);
    cv::putText(img,"osize:"+std::to_string(obj_size),cv::Point(0,240),CV_FONT_HERSHEY_COMPLEX,1,cv::Scalar(255,255,0),1);
    return img;
}

void counter::reinit(){
    background_flag=0;
    obj_size=0;
    size_num=0;
    obj_width_list.clear();
}