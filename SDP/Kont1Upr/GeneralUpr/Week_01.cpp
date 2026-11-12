//
// Created by Lenovo on 15.11.2025 г..
//

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <queue>

// P2
const std::size_t N_VALUES = 1024;

unsigned int getUniqueValueCount(const int n)
{
    std::vector<bool> seen(N_VALUES, 0);
    int temp;

    for (std::size_t i = 0; i < n; i++)
    {
        std::cin >> temp;
        seen[temp] = 1;
    }

    unsigned int count = 0;
    for (const bool val : seen)
    {
        count += val;
    }

    return count;
}

// P3
bool is_opening(char c)
{
    return c == '(' || c == '{' || c == '[';
}

bool is_closing(char c)
{
    return c == ')' || c == '}' || c == ']';
}

bool is_right(char opened, char closed)
{
    return opened == '(' && closed == ')'
        || opened == '{' && closed == '}'
        || opened == '[' && closed == ']';
}

bool is_valid(const std::string& expr)
{
    std::stack<char> st;

    for (const char& c : expr)
    {
        if (is_opening(c))
            st.push(c);

        if (is_closing(c))
        {
            if (st.empty()) return false;

            if (is_right(st.top(), c))
            {
                st.pop();
            }
            else return false;
        }
    }

    return st.empty();
}

// P4
template <typename Type>
void print_stack(const std::stack<Type>& st)
{
    std::stack<Type> copy = st;

    while (!copy.empty())
    {
        std::cout << copy.top() << ' ';
        copy.pop();
    }

    std::cout << std::endl;
}

template <typename Type>
void reverse_stack(std::stack<Type>& st)
{
    std::stack<Type> copy;

    while (!st.empty())
    {
        copy.push(st.top());
        st.pop();
    }

    st = copy;
}

// P5
bool evaluate(const std::string& input)
{
    std::stack<char> operators;
    std::stack<bool> operands;

    for (const char el : input)
    {
        if (el == '0' || el == '1')
            operands.push(el);
        else if (el == '~' || el == '&' || el == '|')
            operators.push(el);
        else if (el == ')')
        {
            char currentOperator = operators.top();
            operators.pop();

            if (currentOperator == '~')
            {
                bool currentOperand = operands.top();
                operands.pop();

                operands.push(!currentOperand);
            }
            else
            {
                bool currentOperand1 = operands.top();
                operands.pop();
                bool currentOperand2 = operands.top();
                operands.pop();

                bool newOperand = (currentOperator == '&'
                                       ? currentOperand1 && currentOperand2
                                       : currentOperand1 || currentOperand2);

                operands.push(newOperand);
            }
        }
    }

    return operators.empty() && operands.size() == 1 && operands.top();
}

// P6
void printFirstKBinary(const int n)
{
    std::queue<std::string> q;
    q.push("1");

    int temp = 1;

    do
    {
        std::string curr = q.front();
        q.pop();

        std::cout << curr << std::endl;

        q.push(curr + '0');
        q.push(curr + '1');

        temp++;
    }
    while (temp <= n);
}

// P7
const int N_MOVES = 8;
const int dX[] = {1, 1, -1, -1, 2, -2, 2, -2},
          dY[] = {2, -2, 2, -2, 1, 1, -1, 1};

struct Position
{
    int x, y;
};

struct Move
{
    Position pos;
    unsigned int k;
};

enum State
{
    UNVISITED,
    IN_QUEUE,
    DONE
};

bool is_valid(unsigned int size, Position pos)
{
    return 0 <= pos.x && pos.x < size
        && 0 <= pos.y && pos.y < size;
}

int getMinMoves(const std::size_t size, const Position start, const Position end)
{
    std::queue<Move> moves;
    std::vector<std::vector<State>> visited(size, std::vector<State>(size, UNVISITED));
    moves.push({start, 0});

    while (!moves.empty())
    {
        Move current = moves.front();
        moves.pop();

        visited[current.pos.x][current.pos.y] = DONE;

        for (int it = 0; it < N_MOVES; ++it)
        {
            Position next_pos = {
                current.pos.x + dX[it],
                current.pos.y + dY[it]
            };

            if (is_valid(size, next_pos) && visited[next_pos.x][next_pos.y] == UNVISITED)
            {
                if (next_pos.x == end.x && next_pos.y == end.y)
                    return current.k + 1;

                moves.push({next_pos, current.k + 1});
                visited[next_pos.x][next_pos.y] = IN_QUEUE;
            }
        }
    }

    return -1;
}

int main()
{
    // // P2
    // unsigned int n;
    // std::cin >> n;
    //
    // std::cout << getUniqueValueCount(n) << std::endl;

    // // P3
    // std::string expr;
    // std::getline(std::cin, expr);
    //
    // std::cout << std::boolalpha << is_valid(expr) << std::endl;

    // // P4
    // unsigned int n;
    // std::cin >> n;
    //
    // std::stack<int> st;
    // int temp;
    // for (std::size_t it = 0; it < n; ++it)
    // {
    //     std::cin >> temp;
    //     st.push(temp);
    // }
    //
    // print_stack(st);
    // reverse_stack(st);
    // print_stack(st);

    // P5
    // std::string input;
    // std::getline(std::cin, input);
    //
    // std::cout << std::boolalpha << evaluate(input) << std::endl;

    // P6
    // unsigned int n;
    // std::cin >> n;
    //
    // printFirstKBinary(n);

    // P7
    unsigned int n;
    std::cin >> n;
    Position start, end;
    std::cin >> start.x >> start.y;
    std::cin >> end.x >> end.y;

    std::cout << getMinMoves(n, start, end) << std::endl;

    return 0;
}
