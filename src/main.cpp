#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <cmath>


int main()
{
/*     std::unordered_map<std::string, double> variables = {
        {"x", 5},
        {"y", 10},
        {"a", 20},
        {"b", 30}
    };

    auto priority = priority_table();
    auto operators = operator_table();
    auto functions = make_functions();

    struct test_case
    {
        std::string expression;
        double expected;
    };

    std::vector<test_case> tests = {
        {"1 + 22 * f(123)", 332839},
        {"(1 + 2) * 3", 9},
        {"10 - 2 - 3", 5},
        {"20 / 5 * 2", 8},
        {"17 % 5", 2},
        {"2 ^ 3 ^ 2", 512},
        {"x + y * 2", 25},
        {"f(3) + f(4)", 25},
        {"f(f(2)) + 1", 17},
        {"((2 + 3) * (4 - 1)) / 5", 3},
        {"1.5 + 2.5 * 2", 6.5},
        {"sqrt(25) + cos(0)", 6},
        {"sin(0) + cos(0)", 1},
        {"abs(0 - 5)", 5},
        {"log(1) + exp(0)", 1}
    };

    int passed = 0;
    int failed = 0;

    std::cout << "=== EXPRESSION TESTS ===\n\n";

    for (std::size_t i = 0; i < tests.size(); ++i)
    {
        try
        {
            auto tokens = split_for_tokens(
                begin(tests[i].expression),
                end(tests[i].expression)
            );

            define_token_types(tokens);

            auto postfix = to_postfix(tokens, priority);

            double result = evaluate_posfix(
                postfix,
                variables,
                operators,
                functions
            );

            bool success = std::abs(
                result - tests[i].expected
            ) < 1e-9;

            if (success)
            {
                ++passed;
                std::cout << "[PASS] ";
            }
            else
            {
                ++failed;
                std::cout << "[FAIL] ";
            }

            std::cout << tests[i].expression
                      << " = " << result
                      << " (expected: " << tests[i].expected << ")\n";
        }
        catch (const std::exception& e)
        {
            ++failed;

            std::cout << "[ERROR] "
                      << tests[i].expression
                      << " -> " << e.what() << '\n';
        }
    }

    std::cout << "\n=== TEST RESULTS ===\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "Total:  " << tests.size() << '\n';

    std::vector<std::string> error_tests = {
        "1 / 0",
        "z + 1",
        "g(2)",
        "(1 + 2",
        "1 +",
        "1 2"
    };

    std::cout << "\n=== ERROR TESTS ===\n\n";

    for (const auto& expression : error_tests)
    {
        try
        {
            auto tokens = split_for_tokens(
                begin(expression),
                end(expression)
            );

            define_token_types(tokens);

            auto postfix = to_postfix(tokens, priority);

            double result = evaluate_posfix(
                postfix,
                variables,
                operators,
                functions
            );

            std::cout << "[FAIL] "
                    << expression
                    << " -> No exception, result: "
                    << result << '\n';
        }
        catch (const std::exception& e)
        {
            std::cout << "[PASS] "
                    << expression
                    << " -> " << e.what() << '\n';
        }
    }

    return failed == 0 ? 0 : 1; */

    return 0;
}
