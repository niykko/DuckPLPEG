#include "peglib.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace peg;
using namespace std;

string loadGrammar(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        throw runtime_error("Failed to open grammar file: " + filename);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

string evaluateExpression(const string &input, const string &grammarFile = "gram.peg")
{
    string grammar = loadGrammar(grammarFile);
    parser parser(grammar);

    if (!parser)
    {
        throw runtime_error("Failed to compile grammar");
    }

    parser.set_logger([](size_t ln, size_t col, const string &msg)
                      { cerr << "Error at line " << ln << ", column " << col << ": " << msg << endl; });

    parser["ROOT"] = [](const SemanticValues &vs)
    {
        std::cout << "ROOT Token(0): " << vs.token(0) << std::endl;
        std::cout << "ROOT sv(): " << vs.sv() << std::endl;
        return vs.token_to_string();
    };

    parser["CONTENT"] = [](const SemanticValues &vs)
    {
        std::cout << "CONTENT Token(0): " << vs.token(0) << std::endl;
        std::cout << "CONTENT sv(): " << vs.sv() << std::endl;
        return vs.token_to_string();
    };

    parser["ELEMENT"] = [](const SemanticValues &vs)
    {
        std::cout << "ELEMENT Token(0): " << vs.token(0) << std::endl;
        std::cout << "ELEMENT sv: " << vs.sv() << std::endl;
        std::cout << "ELEMENT vs[0]: " << any_cast<string>(vs[0]) << std::endl;
        return vs.token_to_string();
    };

    parser["STAG"] = [](const SemanticValues &vs)
    {
        std::cout << "STAG Token(0): " << vs.token(0) << std::endl;
        std::cout << "STAG sv: " << vs.sv() << std::endl;
        return vs.token_to_string();
    };

    parser["ETAG"] = [](const SemanticValues &vs)
    {
        std::cout << "ETAG Token(0): " << vs.token(0) << std::endl;
        std::cout << "ETAG sv: " << vs.sv() << std::endl;
        return vs.token_to_string();
    };

    parser["TAG_NAME"] = [](const SemanticValues &vs)
    {
        std::cout << "TAG_NAME Token(0): " << vs.token(0) << std::endl;
        std::cout << "TAG_NAME sv: " << vs.sv() << std::endl;
        return vs.token_to_string();
    };

        parser["TEXT"] = [](const SemanticValues &vs)
    {
        std::cout << "TEXT Token(0): " << vs.token(0) << std::endl;
        std::cout << "TEXT sv: " << vs.sv() << std::endl;
        return vs.token_to_string();
    };

    string val;

    if (!parser.parse(input, val))
    {
        throw runtime_error("Error, cannot parse " + input);
    }

    return val;
}

int main()
{
    try
    {
        string expr;
        cout << "Give input (or 'quit' to exit):\n";
        while (true)
        {
            cout << "> ";
            getline(cin, expr);
            if (expr == "quit")
                break;
            try
            {
                string result = evaluateExpression(expr);
                cout << "Result: " << result << "\n";
            }
            catch (const exception &e)
            {
                cout << "Error: " << e.what() << "\n";
            }
        }
    }
    catch (const exception &e)
    {
        cerr << e.what() << "\n";
        return 1;
    }
}
