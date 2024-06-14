#include <iostream>
#include <optional>
#include <variant>
#include "timingwheel.h"

void hello(){}

int main()
{
    TimingWheel wheel;
    wheel.addTaskJob(hello , 1);

    std::variant<int> v_int;
    std::optional<int> optionalVal(100);
    if (optionalVal.has_value())
    {
        std::cout << "has_value" << std::endl;
    }
    int val = optionalVal.value();
    std::cout << val << std::endl;

    auto res = std::views::iota(1) | std::views::transform([](auto n)
                                                           { return n * n; }) |
               std::views::filter([](auto n)
                                  { return n % 2 == 1; }) |
               std::views::take_while([](auto n)
                                      { return n < 100000; });
    auto t = res.begin();
    std::cout << *t << std::endl;
    return 0;
}