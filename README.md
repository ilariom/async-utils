# async-utils
Asynchronous programming may be though as a type of concurrent programming on single threaded environments, where functions are atomic. This project aims to provide a set of utilities in C++ to simplify dealing with some typical hard-to-follow execution paths and to lift callback organization.
Any type that takes parametric arguments, do so to allow the underlying callbacks to be called with parameters of these types.

## Conditional Trigger
A `conditional_trigger` executes the wrapped function only when a condition is met. This condition can be passed as another function at construction.
```cpp
// An external source of truth
int counter = 0;

// Construct by passing the callback first, then the condition
async::conditional_trigger ct {
    [] (async::conditional_trigger&) {
        std::cout << "Triggered!" << std::endl;
    },
    [&counter] () -> bool {
        return counter % 2 == 0;
    }
};

// Print "triggered" only when counter is even
for (auto k = 0; k < 10; ++k)
{
    counter++;
    std::cout << "Counter is " << (counter % 2 == 0? "even" : "odd") << std::endl;
    ct();
}

// Pass additional properties to `ct`. 
// This acts as a local storage, accessible through the callback argument
ct.set_property("a_string", "a_value");
ct.set_property("an_int", 1);
auto& var = ct.get_property("an_int");
int n = var.as_integer();
```
## Rendez-Vous
`rendez_vous` are barriers. You can decide that a function should be called only at the n-th time.
```cpp
std::vector<int> v { 1, 2, 3, 4, 5 };

async::rendez_vous rv (
    v.size(),
    [] {
        std::cout << "Triggered!" << std::endl;
    }
);

// We expect "Triggered!" to be printed only after the last vector element.
// This is a trivial example, but think about repeated callbacks with complex behavior
for (auto k = 0; k < v.size(); ++k)
{
    std::cout << v[k] << std::endl;
    rv();
}
```
## Lightswitch
`lightswitch`es are triggered when the total count of on / off switched amount to zero
```cpp
async::lightswitch lsw {
    [] {
        std::cout << "Triggered!" << std::endl;
    }
};

lsw.lights_on();
lsw.lights_on();
lsw.lights_off();
lsw.lights_off();
// Triggered!
lsw.lights_off();
lsw.lights_on();
// Triggered!
```
## Waiter
Sometimes you need to add callbacks with a different order that you expect them to execute. A `waiter` may help in some of these use cases. Just set the function and a priority and let the waiter sort it out.
```cpp
async::waiter w;

w.take_order(
    1,
    [] {
        std::cout << "I'm the first with 1!" << std::endl;
    }
);

w.take_order(
    10,
    [] {
        std::cout << "I'm the third with 10!" << std::endl;
    }
);

w.take_order(
    3,
    [] {
        std::cout << "I'm the second with 3!" << std::endl;
    }
);

w.serve();
```
This outputs:
```
I'm the first with 1!
I'm the second with 3!
I'm the third with 10!
```
## One Time Function
As the name suggests, `one_time_function`s enforce that the wrapped function will be called only once.
```cpp
async::one_time_function<void> otf {
    [] {
        std::cout << "Triggered!" << std::endl;
    }
};

// Triggers
otf();
// Fails silently
otf(); 
```
## Collapse Function
This is a function wrapper that solves the problem of calling the same function before an interval of time as passed.
```cpp
async::collapse_function<void> cf {
    std::chrono::milliseconds { 1000 },
    [] {
        std::cout << "Triggered!" << std::endl;
    }
};

// This first call executes the underlying function
cf();
// This second fails, as any other call made before 1000 milliseconds has passed
cf();
```
