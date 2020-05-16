#ifndef one_time_function_h
#define one_time_function_h

#include <functional>

namespace async
{

class one_time_function_execute
{
protected:
    bool has_executed() const { return executed_; }
    void set_executed() const { executed_ = true; }
    void reset_executed() const { executed_ = false; }
    
private:
    mutable bool executed_ = false;
};

template<typename... args_types>
class one_time_function final : private one_time_function_execute
{
public:
    one_time_function(const std::function<void(args_types...)>& fn) : fn_(fn) { }
    
public:
    void operator()(args_types... args) const
    {
        if (has_executed())
        {
            return;
        }
        
        fn_(std::forward<args_types>(args)...);
        set_executed();
    }
    
private:
    std::function<void(args_types...)> fn_;
};

template<>
class one_time_function<void> final : private one_time_function_execute
{
public:
    one_time_function(const std::function<void()>& fn) : fn_(fn) { }
    
public:
    void operator()() const
    {
        if (has_executed())
        {
            return;
        }
        
        fn_();
        set_executed();
    }
    
private:
    std::function<void()> fn_;
};


} // namespace async

#endif