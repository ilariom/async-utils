#ifndef collapse_function_h
#define collapse_function_h

#include <functional>
#include <chrono>

namespace async
{

class collapse_function_execute
{
public:
    collapse_function_execute() = delete;
    
protected:
    collapse_function_execute(std::chrono::milliseconds grace_period)
        : grace_period_(grace_period)
    { }
    
protected:
    bool is_locked() const
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto delta = now - start_time_;
        
        if (delta > grace_period_)
        {
            start_time_ = now;
            return false;
        }
        
        return true;
    }
    
private:
    std::chrono::milliseconds grace_period_;
    mutable std::chrono::high_resolution_clock::time_point start_time_;
};

template<typename... args_types>
class collapse_function final : private collapse_function_execute
{
public:
    collapse_function(
        std::chrono::milliseconds grace_period,
        const std::function<void(args_types...)>& fn
    )
        : collapse_function_execute(grace_period), fn_(fn)
    { }
    
public:
    void operator()(args_types... args) const
    {
        if (is_locked())
        {
            return;
        }
        
        fn_(std::forward<args_types>(args)...);
    }
    
private:
    std::function<void(args_types...)> fn_;
};

template<>
class collapse_function<void> final : private collapse_function_execute
{
public:
    collapse_function(
        std::chrono::milliseconds grace_period,
        const std::function<void()>& fn
    )
        : collapse_function_execute(grace_period), fn_(fn)
    { }
    
public:
    void operator()() const
    {
        if (is_locked())
        {
            return;
        }
        
        fn_();
    }
    
private:
    std::function<void()> fn_;
};

} // namespace async

#endif