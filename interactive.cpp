#include "peglib.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace peg;
using namespace std;

// Function to load grammar from file
string loadGrammar(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Failed to open grammar file: " + filename);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Function that creates a parser and evaluates an expression
int evaluateExpression(const string &input, const string &grammarFile = "gram.peg") {
    string grammar = loadGrammar(grammarFile);
    parser parser(grammar);

    if (!parser) {
        throw runtime_error("Failed to compile grammar");
    }

    // Setup actions
    parser["Additive"] = [](const SemanticValues &vs) {
        switch (vs.choice()) {
        case 0: return any_cast<int>(vs[0]) + any_cast<int>(vs[1]);
        default: return any_cast<int>(vs[0]);
        }
    };
    parser["Multiplicative"] = [](const SemanticValues &vs) {
        switch (vs.choice()) {
        case 0: return any_cast<int>(vs[0]) * any_cast<int>(vs[1]);
        default: return any_cast<int>(vs[0]);
        }
    };
    parser["Number"] = [](const SemanticValues &vs) {
        return vs.token_to_number<int>();
    };

    parser.enable_packrat_parsing();

    int val;
    if (!parser.parse(input, val)) {
        throw runtime_error("Failed to parse input: " + input);
    }

    return val;
}

int main() {
    try {
        string expr;
        cout << "Enter arithmetic expressions to evaluate (or 'quit' to exit):\n";
        while (true) {
            cout << "> ";
            getline(cin, expr);
            if (expr == "quit") break;
            try {
                int result = evaluateExpression(expr);
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
