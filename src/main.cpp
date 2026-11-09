/* #include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <limits>
#include <random>
#include <numbers> */

// #include "build_convex_hull.hpp"


// --- Вычисление символьных выражений ---
// 1. Разделение строки на токены

#include <string>
#include <vector>
#include <iostream>




// текущий символ начала нового токена = а
// [a; b] - новый токен
// следующий начинается с b + 1
// !!!(может быть, a == b)


// 2. Определение типов токенов
// Просматриваем массив токенов 
// Если первый символ очередного токена:
// - цифра, то это константа
// - открыващая скобка, то это op_br
// - закрывающая скобка, то это cl_br
// - буква, то смотрим есть ли следующий токен и не равен ли он открывающей скобке
//      - если это скобка, тогда func (fun) 
//      - иначе var
// - все остальное op

enum class token_t { constant, var, op_br, cl_br, fun, op };

struct token
{
	std::string name;
	token_t type;
};


template<typename Iter>
std::vector<token> split_for_tokens(Iter first, Iter last)
{
    std::vector<token> res;

    while (first != last)
    {
        if (std::isspace(*first))
        {
            ++first;
            continue;
        }

        auto it = std::__find_if(first, last, [](auto c)
        {
            return std::isalnum(c) && c != '.';
        });

        if (first == it)
        {
            ++it;
        }

        res.push_back( {std::string(first, it), token_t::op} );
        first = it;
    }

    return res;
}

// isdigit(), isalph(), isalnum(), isspace()

/* token_t token_type(std::vector<token> src)
{
    
} */


// 3. Создание таблицы приритетов
//      std::string -> число

// 4. Преобразование в постфиксную запись
//      std::vector<token> -> std::vector<token>

// 5. Вычисление постфиксной записи
//      std::vector<token>, 
//      словарь значений переменных, 
//      коды для вычисления функций и операций 
//          (задается через map(unordered map) который по имени функции и операции индексирует)


const std::string data = "1 + 22 * f(123)";


int main(void)
{
    std::vector<token> tokens = split_for_tokens(begin(data), end(data));

    for (auto t : tokens)
    {
        std::cout << t.name << "\n";
    }

    return 0;

    /* bool f = true;

    for (std::size_t layers = 1; layers <= 100; ++layers)
    {
        auto test = make_nested_triangles(layers);
        std::size_t result = build_multi_convex_hull(test);

        if (result != layers)
        {
            std::cout << "test failed: " << result << " != " << layers << "\n";
            f = false;
            break;
        }
    }

    if (f)
    {    
        std::cout << "all triangle's tests passed\n";
    }


    f = true;

    for (std::size_t layers = 1; layers <= 100; ++layers)
    {
        auto test = make_nested_rings(layers);
        std::size_t result = build_multi_convex_hull(test);

        if (result != layers)
        {
            std::cout << "test failed: " << result << " != " << layers << "\n";
            f = false;
        }
    }

    if (f)
    {    
        std::cout << "all ring's tests passed\n";
    } */

    return 0;
}
