//
// Created by Lenovo on 24.1.2026 г..
//

#include <iostream>
#include <stack>

bool evaluate(const std::string& input)
{
    std::stack<char> operators;
    std::stack<bool> operands;

    for (const char el : input)
    {
        if (el == '0' || el == '1')
            operands.push(el - '0');
        else if (el == '~' || el == '&' || el == '|')
            operators.push(el);
        else if (el == ')')
        {
            char currentOperator = operators.top();
            operators.pop();

            if (currentOperator == '~')
            {
                const bool currentOperand = operands.top();
                operands.pop();

                operands.push(!currentOperand);
            }
            else
            {
                const bool currentOperand1 = operands.top();
                operands.pop();
                const bool currentOperand2 = operands.top();
                operands.pop();

                bool newOperand = (
                    currentOperator == '&' ? currentOperand1 && currentOperand2 : currentOperand1 || currentOperand2
                );

                operands.push(newOperand);
            }
        }
    }

    return operators.empty() && operands.size() == 1 && operands.top();
}

int main()
{
    std::string input;
    std::getline(std::cin, input);

    std::cout << std::boolalpha << evaluate(input) << std::endl;

    return 0;
}
