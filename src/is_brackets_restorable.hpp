#include <string>



template<typename Iter>
bool is_brackets_restorable(Iter first, Iter last)
{
    int a = 0, b = 0;

    for (; first != last; ++first) {
        if (*first == '(') { a++; b++; }
        else if (*first == ')') { 
            a--;
            b--;
            if (a < 0) { a = 1; }
            if (b < 0) { return false; }
        }
        else {
            a--;
            b++;
            if (a < 0) { a = 1; }
        }
    }

    return a == 0;
}


template<typename Iter>
std::string is_brackets_restorable_v2(Iter first, Iter last)
{
    int a = 0, b = 0;
    std::deque<std::string> deq;
    deq.push_back("");

    for (; first != last; ++first) {
        if (*first == '(') { a++; b++; }
        else if (*first == ')') { 
            a--;
            b--;
            if (a < 0) { 
                a = 1;
                deq.pop_front();
            }
            if (b < 0) { return false; }
        }
        else {
            a--;
            b++;

            if (a < 0) { 
                a = 1;
                deq.pop_front(); 
            }

            auto back = d.back();

            for (auto& el : deq) { el.push_back(')'); }
            deq.push_back(back + '(');

        }
    }

    if (a == 0) { return deq.front(); }
    return "";
}
