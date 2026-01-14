#include "../../peglib.h"
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

    parser["X"] = [](const SemanticValues &vs)
    {
        return std::string("success");
    };

    string val;
    if (!parser.parse(input, val))
    {
        throw runtime_error("Failed to parse input: " + input);
    }

    return val;
}

int main()
{
    string expr = ">z";
    string result = evaluateExpression(expr);
    cout << "Result: " << result << "\n";
}
