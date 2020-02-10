#ifndef __TRIGGER_H__
#define __TRIGGER_H__
#endif 

typedef void (*cb)(void);
class trigger{
private:
    float h_threshold;          //high threshold 
    float l_threshold;          //low  threshold
   
    cb lh_cb;                   //low to high callback
    cb hl_cb;                   //high to low callback
    
    bool state=false;
    bool last_state=false;

    void internal_cb(void);

public:
    trigger(int ht=10,int lt=5,cb lb=nullptr,cb hb=nullptr);
    ~trigger();

    trigger& set_h_threshold(float h){h_threshold=h; return *this;}
    float get_h_threshold(){return h_threshold;}

    trigger& set_l_threshold(float l){l_threshold=l; return *this;}
    float get_l_threshold(){return l_threshold;}
    trigger& set_cb(cb lh,cb hl){lh_cb=lh;hl_cb=hl;return *this;}
    
    

    trigger& feed(float);
    void trigger_test(void);
    bool get_state(){return state;}

};