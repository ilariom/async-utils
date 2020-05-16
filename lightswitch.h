#ifndef lightswitch_h
#define lightswitch_h

#include <functional>

namespace async
{

class lightswitch
{
public:
    lightswitch(const std::function<void()>& fn)
        : fn_(fn)
    { }
    
    void lights_on();
    void lights_off();
    
private:
    std::function<void()> fn_;
    int counter_ = 0;
};

inline void lightswitch::lights_on()
{
    counter_++;
    
    if (counter_ == 0)
    {
        fn_();
    }
}
    
inline void lightswitch::lights_off()
{
    counter_--;
    
    if (counter_ == 0)
    {
        fn_();
    }
}

} // namespace async

#endif