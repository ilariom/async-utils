#ifndef rendez_vous_h
#define rendez_vous_h

#include <functional>

namespace estd
{

class rendez_vous
{
public:
    rendez_vous(int times, const std::function<void()>& fn)
        : times_(times), fn_(fn)
    { }
    
    void operator()();
    
private:
    int times_;
    std::function<void()> fn_;
};

inline void rendez_vous::operator()()
{
    times_--;
    
    if (times_ == 0)
    {
        fn_();
    }
}

} // namespace estd

#endif