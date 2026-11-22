#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <cmath>

// --- Вычисление символьных выражений ---
// 1. Разделение строки на токены

// текущий символ начала нового токена = а
// [a; b] - новый токен
// следующий начинается с b + 1
// !!!(может быть, a == b)

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

        auto a = first;

        if (std::isdigit(static_cast<unsigned char>(*first)))
        {
            while (first != last && std::isdigit(static_cast<unsigned char>(*first)))
            {
                ++first;
            }

            if (first != last && *first == '.')
            {
                ++first;

                while (first != last && std::isdigit(static_cast<unsigned char>(*first)))
                {
                    ++first;
                }
            }
        }

        else if (std::isalpha(static_cast<unsigned char>(*first)))
        {
            while (first != last && std::isalnum(static_cast<unsigned char>(*first)))
            {
                ++first;
            }
        }

        else
        {
            ++first;
        }

        res.push_back({std::string(a, first), token_t::op}); // token_t::op is temporary
    }

    return res;
}

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

token_t token_type(const token& t, const token* next = nullptr)
{
    char c = t.name[0];

    if (std::isdigit(static_cast<unsigned char>(c)))
    {
        return token_t::constant;
    }

    else if (c == '(')
    {
        return token_t::op_br;
    }

    else if (c == ')')
    {
        return token_t::cl_br;
    }

    else if (std::isalpha(static_cast<unsigned char>(c)))
    {
        if (next != nullptr && next->name == "(")
        {
            return token_t::fun;
        }
        else
        {
            return token_t::var;
        }
    }
    
    else
    {
        return token_t::op;
    }
}

void define_token_types(std::vector<token>& src)
{
    for (std::size_t i = 0; i < src.size(); ++i)
    {
        const token* next = nullptr;

        if (i + 1 < src.size())
        {
            next = &src[i + 1];
        }

        src[i].type = token_type(src[i], next);
    }
}

// 3. Создание таблицы приритетов
//      std::string -> число

std::unordered_map<std::string, int> priority_table()
{
    return 
    {
        {"+", 1}, 
        {"-", 1},
        {"*", 2},
        {"/", 2},
        {"%", 2},
        {"^", 3}
    };
}

// 4. Преобразование в постфиксную запись
//      std::vector<token> -> std::vector<token>

std::vector<token> to_postfix(
    const std::vector<token>& src,
    const std::unordered_map<std::string, int>& priorities)
{
    std::vector<token> res;
    std::vector<token> stack;

    for (const auto& t : src)
    {
        if (t.type == token_t::constant || t.type == token_t::var)
        {
            res.push_back(t);
        }

        else if (t.type == token_t::fun)
        {
            stack.push_back(t);
        }

        else if (t.type == token_t::op_br)
        {
            stack.push_back(t);
        }

        else if (t.type == token_t::cl_br)
        {
            while (!stack.empty() && stack.back().type != token_t::op_br)
            {
                res.push_back(stack.back());
                stack.pop_back();
            }

            if (stack.empty())
            {
                throw std::runtime_error("Err: missmatched parentheses");
            }

            stack.pop_back();

            if (!stack.empty() && stack.back().type == token_t::fun)
            {
                res.push_back(stack.back());
                stack.pop_back();
            }
        }

        else if (t.type == token_t::op)
        {
            if (t.name == ",")
            {
                while (!stack.empty() && stack.back().type != token_t::op_br)
                {
                    res.push_back(stack.back());
                    stack.pop_back();
                }

                if (stack.empty())
                {
                    throw std::runtime_error("Err: invalid comma");
                }

                continue;
            }

            auto cur = priorities.find(t.name);

            if (cur == priorities.end())
            {
                throw std::runtime_error("Err: unknown operator: " + t.name);
            }

            while (!stack.empty() && stack.back().type == token_t::op)
            {
                auto top = priorities.find(stack.back().name);

                if (top == priorities.end())
                {
                    throw std::runtime_error("Err: unknown operator in stack");
                }

                int p1 = cur->second;
                int p2 = top->second;
                bool right_associative = (t.name == "^");

                if (p2 > p1 || (p2 == p1 && !right_associative))
                {
                    res.push_back(stack.back());
                    stack.pop_back();
                }
                else
                {
                    break;
                }
            }

            stack.push_back(t);
        }
    }

    while (!stack.empty())
    {
        if (stack.back().type == token_t::op_br)
        {
            throw std::runtime_error("Err: mismatched parentheses");
        }

        res.push_back(stack.back());
        stack.pop_back();
    }

    return res;
}

// 5. Вычисление постфиксной записи
//      std::vector<token>, 
//      словарь значений переменных, 
//      коды для вычисления функций и операций 
//          (задается через map(unordered map) который по имени функции и операции индексирует)

using operator_t = std::function<double(double, double)>;

std::unordered_map<std::string, operator_t> operator_table()
{
    return 
    {
        {"+", [](double a, double b) { return a + b; }},
        {"-", [](double a, double b) { return a - b; }},
        {"*", [](double a, double b) { return a * b; }},
        {"/", [](double a, double b) 
        {
            if (b == 0)
            {
                throw std::runtime_error("Err: division by zero");
            }
            
            return a / b;
        }},
        {"%", [](double a, double b)
        {
            if (b == 0)
            {
                throw std::runtime_error("Err: division by zero");
            }

            return std::fmod(a, b);
        }},
        {"^", [](double a, double b) { return std::pow(a, b); }}
    };
}

using function_t = std::function<double(double)>;

std::unordered_map<std::string, function_t> make_functions()
{
    return {
        {"sin", [](double x) { return std::sin(x); }},
        {"cos", [](double x) { return std::cos(x); }},
        {"sqrt", [](double x) { return std::sqrt(x); }},
        {"abs", [](double x) { return std::abs(x); }},
        {"log", [](double x) { return std::log(x); }},
        {"exp", [](double x) { return std::exp(x); }},

        // custom func
        {"f", [](double x) { return x * x; }}
    };
}

// ------------------
// correct for func:
#include <stack>

double pop(std::stack<double>& s)
{
    double res = s.top();
    s.pop();
    return res;
}

std::unordered_map<std::string, std::function<double(std::stack<double>&)>> operations
{
    {"+", [](std::stack<double>& s) { return pop(s) + pop(s); }},
    {"-", [](std::stack<double>& s) { return -pop(s) + pop(s); }},
    {"*", [](std::stack<double>& s) { return pop(s) * pop(s); }},
    {"/", [](std::stack<double>& s) 
        {
            double a = pop(s);
            double b = pop(s);
            
            if (b == 0)
            {
                throw std::runtime_error("Err: division by zero");
            }
            
            return a / b;
        }
    },
    {"%", [](std::stack<double>& s) 
        { 
            double a = pop(s);
            double b = pop(s);
            
            if (b == 0)
            {
                throw std::runtime_error("Err: division by zero");
            }
            
            return std::fmod(b, a);
        }
    },
    {"^", [](std::stack<double>& s) 
        { 
            double a = pop(s);
            double b = pop(s);
            
            return std::pow(b, a);
        }
    }
    /* {"-", [](std::stack<double>& s) 
        { 
            return -pop(s); 
        }
    }, */
    /* {"!", [](std::stack<double>& s)
        {
            return 
        }
    } */
};


// correct func or op ------------------
// while(s.size() && (s.top().type == token_t::fun || s.top().type == token_t::op &&
//       priority[s.top().name] >= priority[t.name]))


// make ternarn operator
// ... ? ... : ...

// example
// a + b > c ? a * b ^ 2 - 3 : sin(c*a)
// a b + c > a b 2 ^ + 3 - c a * sin ?: (?: is one token)

// ? - not push in stack, : - push in stack
// : - have minimum priority for all ops and funcs
// -------------------------------------

// 1. Реализовать поддержку тернарного оператора
// 2. Написать класс-функтор для вычисления фунций одной переменной
//  class custom_func f("x*sin(x/2)"); std::cout << f(2) - передается строчка с одной переменной 

// (необязательно) 3. Создание дерева вычислений (по принципу левый сын правый сосед)

double evaluate_posfix(
    const std::vector<token>& src,
    const std::unordered_map<std::string, double>& variables,
    const std::unordered_map<std::string, operator_t>& operators,
    const std::unordered_map<std::string, function_t>& functions)
{
    std::vector<double> stack;

    for (const auto& t : src)
    {
        if (t.type == token_t::constant)
        {
            stack.push_back(std::stod(t.name));
        }

        else if (t.type == token_t::var)
        {
            auto it = variables.find(t.name);

            if (it == variables.end())
            {
                throw std::runtime_error("Err: unknown variable:" + t.name);
            }

            stack.push_back(it->second);
        }

        else if (t.type == token_t::op)
        {
            if (stack.size() < 2)
            {
                throw std::runtime_error("Err: not enough operands");
            }

            auto it = operators.find(t.name);

            if (it == operators.end())
            {
                throw std::runtime_error("Err: unknown operator:" + t.name);
            }

            double b = stack.back();
            stack.pop_back();
            double a = stack.back();
            stack.pop_back();

            double res = it->second(a, b);
            stack.push_back(res);
        }

        else if (t.type == token_t::fun)
        {
            if (stack.empty())
            {
                throw std::runtime_error("Err: not enough arguments");
            }

            auto it = functions.find(t.name);

            if (it == functions.end())
            {
                throw std::runtime_error("Err: unknown function: " +t.name);
            }

            double x = stack.back();
            stack.pop_back();

            double res = it->second(x);
            stack.push_back(res);
        }

        else
        {
            throw std::runtime_error("Err: unexpected token: " + t.name);
        }
    }

    if (stack.size() != 1)
    {
        throw std::runtime_error("Err: invalid expression");
    }

    return stack.back();
}
