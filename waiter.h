#ifndef waiter_h
#define waiter_h

#include <functional>
#include <vector>
#include <algorithm>

namespace estd
{

class waiter
{
public:
    void take_order(int priority, const std::function<void()>& client);
    void serve();

private:
    struct client
    {
        int priority;
        std::function<void()> fn;
    };
    
private:
    std::vector<client> clients_;
};

inline void waiter::take_order(int priority, const std::function<void()>& fn)
{
    clients_.push_back({ priority, fn });
}

inline void waiter::serve()
{
    std::sort(clients_.begin(), clients_.end(), [] (const client& a, const client& b) {
        return a.priority <= b.priority;
    });
    
    for (client& client : clients_)
    {
        client.fn();
    }
}

} // namespace estd

#endif