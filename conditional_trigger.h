#ifndef conditional_trigger_h
#define conditional_trigger_h

#include "variant/variant.h"
#include <unordered_map>
#include <functional>

namespace async
{

class conditional_trigger
{
public:
    conditional_trigger() = default;
    conditional_trigger(
        const std::function<void(conditional_trigger&)>& fn, 
        const std::function<bool()>& cond = nullptr
    )
        : fn_(fn), cond_(cond)
    { }
    
    void operator()(bool trigger = false);
    void set_property(const std::string& key, const estd::variant& value);
    estd::variant& get_property(const std::string& key);
    const estd::variant& get_property(const std::string& key) const;
    
    void invalidate() { is_valid_ = false; }
    
private:
    std::function<void(conditional_trigger&)> fn_;
    std::function<bool()> cond_;
    std::unordered_map<std::string, estd::variant> local_storage_;
    bool is_valid_ = true;
};

inline void conditional_trigger::operator()(bool trigger)
{
    if (is_valid_ && (trigger || (cond_ && cond_())))
    {
        fn_(*this);
    }
}

inline void conditional_trigger::set_property(const std::string& key, const estd::variant& value)
{
    local_storage_[key] = value;
}

inline estd::variant& conditional_trigger::get_property(const std::string& key)
{
    return local_storage_[key];
}

inline const estd::variant& conditional_trigger::get_property(const std::string& key) const
{
    return local_storage_.at(key);
}

} // namespace async

#endif