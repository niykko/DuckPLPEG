#include "peglib.h"
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

string evaluateExpression(const string &input, const string &grammarFile = "grambini.peg") {
    string grammar = loadGrammar(grammarFile);
    parser parser(grammar);

    if (!parser) {
        throw runtime_error("Failed to compile grammar");
    }

    parser["QuotedString"] = [](const SemanticValues &vs) {
        return any_cast<string>(vs[0]);
    };


    parser["String"] = [](const SemanticValues &vs) {
        string s;
        for (const auto &v : vs) {
            s += any_cast<string>(v);
        }
        return s;
    };

    parser["Char"] = [](const SemanticValues &vs) {
        string s = vs.token_to_string();
        std::cout << "Token(0): " << vs.token(0) << std::endl;
        return s;
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
        cout << "Enter arithmetic expressions to evaluate (or 'quit' to exit):\n";
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
