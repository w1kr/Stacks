#pragma once

#include <algorithm>
#include <cctype>
#include <cmath>
#include <functional>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>


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
    token_t type = token_t::op;

    std::size_t argc = 0;
};


template<typename Iter>
std::vector<token> split_for_tokens(Iter first, Iter last)
{
    std::vector<token> result;

    while (first != last)
    {
        const unsigned char ch = static_cast<unsigned char>(*first);
        if (std::isspace(ch))
        {
            ++first;
            continue;
        }

        auto begin = first;

        if (std::isdigit(ch) || (*first == '.' && first != last))
        {
            bool has_digit = false;
            while (first != last && std::isdigit(static_cast<unsigned char>(*first)))
            {
                has_digit = true;
                ++first;
            }

            if (first != last && *first == '.')
            {
                ++first;
                while (first != last && std::isdigit(static_cast<unsigned char>(*first)))
                {
                    has_digit = true;
                    ++first;
                }
            }

            if (!has_digit)
            {
                result.push_back({".", token_t::op});
                continue;
            }

            if (first != last && (*first == 'e' || *first == 'E'))
            {
                auto exponent_begin = first;
                ++first;
                if (first != last && (*first == '+' || *first == '-'))
                    ++first;

                auto digits_begin = first;
                while (first != last && std::isdigit(static_cast<unsigned char>(*first)))
                    ++first;

                if (digits_begin == first)
                    first = exponent_begin;
            }
        }
        else if (std::isalpha(ch) || *first == '_')
        {
            while (first != last)
            {
                const unsigned char c = static_cast<unsigned char>(*first);
                if (!std::isalnum(c) && *first != '_')
                    break;
                ++first;
            }
        }
        else
        {
            ++first;

            if (first != last)
            {
                const std::string two(begin, first + 1);
                if (two == ">=" || two == "<=" || two == "==" || two == "!=" || two == "&&" || two == "||")
                    ++first;
            }
        }

        result.push_back({std::string(begin, first), token_t::op});
    }

    return result;
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

inline token_t token_type(const token& t, const token* next = nullptr)
{
    if (t.name.empty())
        throw std::runtime_error("Err: empty token");

    const unsigned char c = static_cast<unsigned char>(t.name[0]);
    if (std::isdigit(c) || t.name[0] == '.')
        return token_t::constant;

    if (t.name == "(")
        return token_t::op_br;

    if (t.name == ")")
        return token_t::cl_br;

    if (std::isalpha(c) || t.name[0] == '_')
    {
        return (next != nullptr && next->name == "(") ? token_t::fun : token_t::var;
    }

    return token_t::op;
}


inline void define_token_types(std::vector<token>& src)
{
    for (std::size_t i = 0; i < src.size(); ++i)
    {
        const token* next = (i + 1 < src.size()) ? &src[i + 1] : nullptr;
        src[i].type = token_type(src[i], next);
    }
}


inline std::vector<token> tokenize_expression(const std::string& expression)
{
    auto tokens = split_for_tokens(expression.begin(), expression.end());
    define_token_types(tokens);

    return tokens;
}

// 3. Создание таблицы приритетов
//      std::string -> число

inline std::unordered_map<std::string, int> priority_table()
{
    return {
        {"||", 1},
        {"&&", 2},
        {"<", 3}, {">", 3}, {"<=", 3}, {">=", 3}, {"==", 3}, {"!=", 3},
        {"+", 4}, {"-", 4},
        {"*", 5}, {"/", 5}, {"%", 5},
        {"u+", 6}, {"u-", 6}, {"not", 6},
        {"^", 7},
        {"fact", 8},
        {"?:", 0}
    };
}

// 4. Преобразование в постфиксную запись
//      std::vector<token> -> std::vector<token>

inline std::vector<token> to_postfix(
    const std::vector<token>& src,
    const std::unordered_map<std::string, int>& priorities)
{
    struct paren_frame
    {
        bool is_function = false;
        std::size_t commas = 0;
    };

    std::vector<token> result;
    std::vector<token> operators;
    std::vector<paren_frame> parentheses;
    bool expect_operand = true;

    auto get_priority = [&](const token& op) -> int
    {
        const auto it = priorities.find(op.name);
        if (it == priorities.end())
            throw std::runtime_error("Err: unknown operator: " + op.name);

        return it->second;
    };

    auto pop_operator = [&]()
    {
        if (operators.empty() || operators.back().type != token_t::op)
            throw std::runtime_error("Err: invalid operator stack");

        result.push_back(operators.back());
        operators.pop_back();
    };

    for (const token& original : src)
    {
        token t = original;

        if (t.type == token_t::constant || t.type == token_t::var)
        {
            if (!expect_operand)
                throw std::runtime_error("Err: missing operator before " + t.name);

            result.push_back(t);
            expect_operand = false;
        }
        else if (t.type == token_t::fun)
        {
            if (!expect_operand)
                throw std::runtime_error("Err: missing operator before function " + t.name);

            operators.push_back(t);
        }
        else if (t.type == token_t::op_br)
        {
            if (!expect_operand)
                throw std::runtime_error("Err: missing operator before '('");

            const bool is_function = !operators.empty() && operators.back().type == token_t::fun;
            operators.push_back(t);
            parentheses.push_back({is_function, 0});
        }
        else if (t.type == token_t::cl_br)
        {
            if (parentheses.empty())
                throw std::runtime_error("Err: mismatched parentheses");

            const paren_frame frame = parentheses.back();
            if (expect_operand && (!frame.is_function || frame.commas != 0))
                throw std::runtime_error("Err: empty argument or incomplete expression");

            while (!operators.empty() && operators.back().type != token_t::op_br)
                pop_operator();

            if (operators.empty())
                throw std::runtime_error("Err: mismatched parentheses");

            operators.pop_back(); // '('
            parentheses.pop_back();

            if (frame.is_function)
            {
                if (operators.empty() || operators.back().type != token_t::fun)
                    throw std::runtime_error("Err: invalid function call");

                token function = operators.back();
                operators.pop_back();
                function.argc = (expect_operand ? 0 : frame.commas + 1);
                result.push_back(function);
            }
            expect_operand = false;
        }
        else if (t.type == token_t::op)
        {
            if (t.name == ",")
            {
                if (expect_operand || parentheses.empty() || !parentheses.back().is_function)
                    throw std::runtime_error("Err: invalid comma");

                while (!operators.empty() && operators.back().type != token_t::op_br)
                    pop_operator();

                if (operators.empty())
                    throw std::runtime_error("Err: invalid comma");

                ++parentheses.back().commas;
                expect_operand = true;
                continue;
            }

            if (t.name == "?")
            {
                if (expect_operand)
                    throw std::runtime_error("Err: missing condition before '?'");

                while (!operators.empty() && operators.back().type == token_t::op &&
                        operators.back().name != "?" &&
                       get_priority(operators.back()) > 0)
                    pop_operator();

                operators.push_back(t);
                expect_operand = true;
                continue;
            }

            if (t.name == ":")
            {
                if (expect_operand)
                    throw std::runtime_error("Err: missing expression before ':'");

                while (!operators.empty() && !(operators.back().type == token_t::op && 
                        operators.back().name == "?"))
                {
                    if (operators.back().type == token_t::op_br)
                        throw std::runtime_error("Err: ':' without matching '?'");
                    pop_operator();
                }

                if (operators.empty())
                    throw std::runtime_error("Err: ':' without matching '?'");

                operators.pop_back(); // '?'
                operators.push_back({"?:", token_t::op});
                expect_operand = true;
                continue;
            }

            if (expect_operand)
            {
                if (t.name == "+") t.name = "u+";
                else if (t.name == "-") t.name = "u-";
                else if (t.name == "!") t.name = "not";
                else
                    throw std::runtime_error("Err: unexpected operator: " + t.name);

                get_priority(t);
                operators.push_back(t);
                continue;
            }
            else if (t.name == "!")
            {
                t.name = "fact";
            }

            const int current_priority = get_priority(t);
            const bool right_associative = (t.name == "^" || t.name == "u+" || t.name == "u-" || t.name == "not" || t.name == "?:");

            while (!operators.empty() && operators.back().type == token_t::op &&
                    operators.back().name != "?")
            {
                const int top_priority = get_priority(operators.back());
                if (top_priority > current_priority ||
                    (top_priority == current_priority && !right_associative))
                    pop_operator();
                else
                    break;
            }

            operators.push_back(t);
            expect_operand = (t.name != "fact");
        }
        else
        {
            throw std::runtime_error("Err: unexpected token: " + t.name);
        }
    }

    if (!src.empty() && expect_operand)
        throw std::runtime_error("Err: expression ends with an operator or incomplete argument");
    if (!parentheses.empty())
        throw std::runtime_error("Err: mismatched parentheses");

    while (!operators.empty())
    {
        if (operators.back().type == token_t::op_br)
            throw std::runtime_error("Err: mismatched parentheses");

        if (operators.back().type == token_t::fun)
            throw std::runtime_error("Err: function call without parentheses");
            
        if (operators.back().type == token_t::op &&
            (operators.back().name == "?"))
            throw std::runtime_error("Err: ternary operator is missing ':'");

        result.push_back(operators.back());
        operators.pop_back();
    }

    return result;
}


inline std::vector<token> compile_expression(const std::string& expression)
{
    return to_postfix(tokenize_expression(expression), priority_table());
}

// 5. Вычисление постфиксной записи
//      std::vector<token>, 
//      словарь значений переменных, 
//      коды для вычисления функций и операций 
//          (задается через map(unordered map) который по имени функции и операции индексирует)

using operator_t = std::function<double(double, double)>;
using unary_operator_t = std::function<double(double)>;
using function_t = std::function<double(double)>;
using multi_function_t = std::function<double(const std::vector<double>&)>;

inline std::unordered_map<std::string, operator_t> operator_table()
{
    return {
        {"+", [](double a, double b) { return a + b; }},
        {"-", [](double a, double b) { return a - b; }},
        {"*", [](double a, double b) { return a * b; }},
        {"/", [](double a, double b) {
            if (b == 0.0) throw std::runtime_error("Err: division by zero");
            return a / b;
        }},
        {"%", [](double a, double b) {
            if (b == 0.0) throw std::runtime_error("Err: division by zero");
            return std::fmod(a, b);
        }},
        {"^", [](double a, double b) { return std::pow(a, b); }},
        {"<", [](double a, double b) { return double(a < b); }},
        {">", [](double a, double b) { return double(a > b); }},
        {"<=", [](double a, double b) { return double(a <= b); }},
        {">=", [](double a, double b) { return double(a >= b); }},
        {"==", [](double a, double b) { return double(a == b); }},
        {"!=", [](double a, double b) { return double(a != b); }},
        {"&&", [](double a, double b) { return double(a != 0.0 && b != 0.0); }},
        {"||", [](double a, double b) { return double(a != 0.0 || b != 0.0); }}
    };
}


inline std::unordered_map<std::string, unary_operator_t> unary_operator_table()
{
    return {
        {"u+", [](double x) { return x; }},
        {"u-", [](double x) { return -x; }},
        {"not", [](double x) { return double(x == 0.0); }},
        {"fact", [](double x) {
            if (x < 0.0 || std::floor(x) != x)
                throw std::runtime_error("Err: factorial is defined for non-negative integers");
            double value = 1.0;
            for (unsigned int i = 2; i <= static_cast<unsigned int>(x); ++i)
                value *= i;
            return value;
        }}
    };
}


inline std::unordered_map<std::string, function_t> make_functions()
{
    return {
        {"sin", [](double x) { return std::sin(x); }},
        {"cos", [](double x) { return std::cos(x); }},
        {"sqrt", [](double x) { return std::sqrt(x); }},
        {"abs", [](double x) { return std::abs(x); }},
        {"log", [](double x) { return std::log(x); }},
        {"exp", [](double x) { return std::exp(x); }},
        {"f", [](double x) { return x * x; }}
    };
}


inline void require_argc(const std::vector<double>& args, std::size_t count, const std::string& function_name)
{
    if (args.size() != count)
        throw std::runtime_error("Err: function '" + function_name + "' expects " + std::to_string(count) + " argument(s)");
}


inline std::unordered_map<std::string, multi_function_t> make_function_table()
{
    auto unary = [](auto function, const std::string& name) -> multi_function_t {
        return [function, name](const std::vector<double>& args) {
            require_argc(args, 1, name);
            return function(args[0]);
        };
    };

    return {
        {"sin", unary([](double x) { return std::sin(x); }, "sin")},
        {"cos", unary([](double x) { return std::cos(x); }, "cos")},
        {"sqrt", unary([](double x) { return std::sqrt(x); }, "sqrt")},
        {"abs", unary([](double x) { return std::abs(x); }, "abs")},
        {"log", unary([](double x) { return std::log(x); }, "log")},
        {"exp", unary([](double x) { return std::exp(x); }, "exp")},
        {"f", unary([](double x) { return x * x; }, "f")},
        {"pow", [](const std::vector<double>& a) {
            require_argc(a, 2, "pow");
            return std::pow(a[0], a[1]);
        }},
        {"min", [](const std::vector<double>& a) {
            if (a.empty()) throw std::runtime_error("Err: function 'min' expects at least one argument");
            return *std::min_element(a.begin(), a.end());
        }},
        {"max", [](const std::vector<double>& a) {
            if (a.empty()) throw std::runtime_error("Err: function 'max' expects at least one argument");
            return *std::max_element(a.begin(), a.end());
        }}
    };
}


inline double pop_value(std::vector<double>& stack)
{
    if (stack.empty())
        throw std::runtime_error("Err: not enough operands");
    const double value = stack.back();
    stack.pop_back();
    return value;
}


template<typename FunctionMap>
double evaluate_posfix(
    const std::vector<token>& src,
    const std::unordered_map<std::string, double>& variables,
    const std::unordered_map<std::string, operator_t>& operators,
    const FunctionMap& functions)
{
    using mapped_function_t = typename FunctionMap::mapped_type;
    std::vector<double> stack;
    const auto unary_operators = unary_operator_table();

    for (const token& t : src)
    {
        if (t.type == token_t::constant)
        {
            std::size_t parsed = 0;
            const double value = std::stod(t.name, &parsed);
            if (parsed != t.name.size())
                throw std::runtime_error("Err: invalid number: " + t.name);
            stack.push_back(value);
        }
        else if (t.type == token_t::var)
        {
            const auto it = variables.find(t.name);
            if (it == variables.end())
                throw std::runtime_error("Err: unknown variable: " + t.name);
            stack.push_back(it->second);
        }
        else if (t.type == token_t::op)
        {
            if (t.name == "?:")
            {
                if (stack.size() < 3)
                    throw std::runtime_error("Err: not enough operands for ternary operator");

                const double false_value = pop_value(stack);
                const double true_value = pop_value(stack);
                const double condition = pop_value(stack);
                stack.push_back(condition != 0.0 ? true_value : false_value);
            }
            else if (const auto unary_it = unary_operators.find(t.name); unary_it != unary_operators.end())
            {
                const double value = pop_value(stack);
                stack.push_back(unary_it->second(value));
            }
            else
            {
                const auto it = operators.find(t.name);
                if (it == operators.end())
                    throw std::runtime_error("Err: unknown operator: " + t.name);

                const double b = pop_value(stack);
                const double a = pop_value(stack);
                stack.push_back(it->second(a, b));
            }
        }
        else if (t.type == token_t::fun)
        {
            const auto it = functions.find(t.name);
            if (it == functions.end())
                throw std::runtime_error("Err: unknown function: " + t.name);
            if (stack.size() < t.argc)
                throw std::runtime_error("Err: not enough arguments for function: " + t.name);

            std::vector<double> args(t.argc);
            for (std::size_t i = t.argc; i > 0; --i)
                args[i - 1] = pop_value(stack);

            if constexpr (std::is_invocable_r_v<double, mapped_function_t, const std::vector<double>&>)
            {
                stack.push_back(it->second(args));
            }
            else if constexpr (std::is_invocable_r_v<double, mapped_function_t, double>)
            {
                require_argc(args, 1, t.name);
                stack.push_back(it->second(args[0]));
            }
            else
            {
                static_assert(std::is_invocable_v<mapped_function_t, double>, "Unsupported function signature");
            }
        }
        else
        {
            throw std::runtime_error("Err: unexpected token in postfix expression: " + t.name);
        }
    }

    if (stack.size() != 1)
        throw std::runtime_error("Err: invalid expression (result stack must contain one value)");
    return stack.back();
}

// 2. Написать класс-функтор для вычисления фунций одной переменной
//  class custom_func f("x*sin(x/2)"); std::cout << f(2) - передается строчка с одной переменной 

class custom_func
{
private:
    std::vector<token> postfix_;

public:
    explicit custom_func(const std::string& expression) : postfix_(compile_expression(expression))
    {
        for (const token& t : tokenize_expression(expression))
        {
            if (t.type == token_t::var && t.name != "x")
                throw std::runtime_error("Err: custom_func supports only variable 'x'");
        }
    }

    double operator()(double x) const
    {
        static const auto operators = operator_table();
        static const auto functions = make_function_table();
        return evaluate_posfix(postfix_, {{"x", x}}, operators, functions);
    }

    const std::vector<token>& get_postfix() const noexcept
    {
        return postfix_;
    }
};


inline double calculate_expression(
    const std::string& expression,
    const std::unordered_map<std::string, double>& variables = {})
{
    const auto postfix = compile_expression(expression);
    static const auto operators = operator_table();
    static const auto functions = make_function_table();

    return evaluate_posfix(postfix, variables, operators, functions);
}

// (необязательно) 3. Создание дерева вычислений (по принципу левый сын правый сосед)

