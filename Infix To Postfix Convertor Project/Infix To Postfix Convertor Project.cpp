#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include <cctype>

using namespace std;

class InfixToPostfix {
private:
    string postfixExpression;

    // Function to determine operator precedence
    int precedence(char op) {
        if (op == '+' || op == '-')
            return 1;
        if (op == '*' || op == '/')
            return 2;
        return 0;
    }

    // Function to check if a character is an operator
    bool isOperator(char ch) {
        return ch == '+' || ch == '-' || ch == '*' || ch == '/';
    }

public:
    // Function to convert infix to postfix
    void convert(const string& infix) {
        postfixExpression = ""; // Clear the postfix string
        stack<char> operators;

        for (char ch : infix) {
            if (isalpha(static_cast<unsigned char>(ch))) { // If character is an operand
                postfixExpression += ch;
            }
            else if (ch == '(') { // If character is '('
                operators.push(ch);
            }
            else if (ch == ')') { // If character is ')'
                while (!operators.empty() && operators.top() != '(') {
                    postfixExpression += operators.top();
                    operators.pop();
                }
                if (!operators.empty() && operators.top() == '(') {
                    operators.pop(); // Pop the '('
                }
            }
            else if (isOperator(ch)) { // If character is an operator
                while (!operators.empty() && precedence(ch) <= precedence(operators.top())) {
                    postfixExpression += operators.top();
                    operators.pop();
                }
                operators.push(ch);
            }
            else if (isspace(static_cast<unsigned char>(ch))) { // Ignore spaces
                continue;
            }
            else { // Skip invalid characters
                cerr << "Warning: Ignoring invalid character '" << ch << "' in expression." << endl;
            }
        }

        // Pop remaining operators from the stack
        while (!operators.empty()) {
            postfixExpression += operators.top();
            operators.pop();
        }
    }

    // Accessor to get the postfix expression
    string getPostfixExpression() const {
        return postfixExpression;
    }
};

int main() {
    ifstream inputFile("InfixData.txt");
    if (!inputFile) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }

    InfixToPostfix converter;
    string infix;

    while (getline(inputFile, infix)) {
        // Trim leading and trailing whitespace
        infix.erase(0, infix.find_first_not_of(" \t\r\n"));
        infix.erase(infix.find_last_not_of(" \t\r\n") + 1);

        // Remove the semicolon at the end of the expression if present
        if (!infix.empty() && infix.back() == ';') {
            infix.pop_back();
        }

        // Debugging output to verify input
        cout << "Processed input line: " << infix << endl;

        // Perform the conversion
        converter.convert(infix);

        // Display the result
        cout << "Infix Expression: " << infix << ";" << endl;
        cout << "Postfix Expression: " << converter.getPostfixExpression() << endl;
        cout << endl;
    }

    inputFile.close();
    return 0;
}