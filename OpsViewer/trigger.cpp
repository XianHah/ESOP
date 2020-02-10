#include "trigger.h"
#include <iostream>
trigger::trigger(int ht,int lt,cb lb,cb hb):h_threshold(ht),l_threshold(lt),lh_cb(lb),hl_cb(hb)
{
}

trigger::~trigger(){
    
}

void trigger::internal_cb(void){
    ;
}

trigger& trigger::feed(float val){
    try
    {
        if(last_state==false && val>h_threshold){
            //0->1
            state=true;
            //do somthing 
            if(lh_cb!=nullptr){
                lh_cb();
                std::cout<<"0->1:"<<val<<std::endl;
            }
        }else if (last_state==false)
        {
            //0->0
            state=false;
        }

        if(last_state==true && val<=l_threshold){
            //1->0
            state=false;
            //do somrthing 
            if(hl_cb!=nullptr){
                hl_cb();
                std::cout<<"1->0:"<<val<<std::endl;
            }
        }else if (last_state==true)
        {
            //1->1
            state=true;
        }
        last_state=state;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return *this;
}

void trigger::trigger_test(void){
    ;
}