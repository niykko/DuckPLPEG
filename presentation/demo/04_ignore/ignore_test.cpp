#include "../peglib.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace peg;
using namespace std;

string loadGrammar(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Failed to open grammar file: " + filename);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

string evaluateExpression(const string &input, const string &grammarFile = "gram.peg") {
    string grammar = loadGrammar(grammarFile);
    parser parser(grammar);

    if (!parser) {
        throw runtime_error("Failed to compile grammar");
    }



    parser["THREE_WORD_SENTENCE"] = [](const SemanticValues &vs) {
        std::cout << "THREE_WORD_SENTENCE #SemanticValues: " << vs.size() << std::endl;
        return std::string("*successful parse*");
    };

    string val;
    if (!parser.parse(input, val)) {
        throw runtime_error("Failed to parse input: " + input);
    }

    return val;
}

int main() {
    try {
        string expr;
        cout << "Enter expressions to evaluate (or 'quit' to exit):\n";
        while (true) {
            cout << "> ";
            getline(cin, expr);
            if (expr == "quit") break;
            try {
                string result = evaluateExpression(expr);
                cout << "Result: " << result << "\n";
            } catch (const exception &e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
    } catch (const exception &e) {
        cerr << e.what() << "\n";
        return 1;
    }
}
