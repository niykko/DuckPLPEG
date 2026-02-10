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

    parser["QuotedString"] = [](const SemanticValues &vs) {
        std::cout << "QuotedString Token(0): " << vs.token(0) << std::endl;
        std::cout << "QuotedString VS[0]: " << any_cast<string>(vs[0]) << std::endl;
        return any_cast<string>(vs[0])+" [QsSemanticBonus] ";
    };


    parser["String"] = [](const SemanticValues &vs) {
        string s;
        std::cout << "String Token(0): " << vs.token(0) << std::endl;
        for (const auto &v : vs) {
            s += any_cast<string>(v);
            std::cout << "String v in vs[]:  " << any_cast<string>(v) << std::endl;
        }
        return s + " [StringSemanticBonus] ";
    };

    parser["Char"] = [](const SemanticValues &vs) {
        string s = vs.token_to_string();

        if (vs.choice() == 0) {
            std::cout << "Char VS[0]: " << any_cast<string>(vs[0]) << std::endl;
        }

        std::cout << "Char Token(0): " << vs.token(0) << std::endl;
        return s;
    };


    parser["s"] = [](const SemanticValues &vs) {
        string s = vs.token_to_string();

        std::cout << "s Token(0): " << vs.token(0) << std::endl;
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
