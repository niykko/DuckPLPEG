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

    parser.set_logger([](size_t ln, size_t col, const string &msg)
                      { cerr << "Error at line " << ln << ", column " << col << ": " << msg << endl; });


    parser["CANDIDATE"] = [](const SemanticValues &vs)
    {
        return std::string("Legal vote. Voted for: ") + vs.token_to_string();
    };

    parser["CANDIDATE"].predicate = [](const SemanticValues &vs,
                                       const std::any & /*dt*/, std::string &msg)
    {
        if (vs.token_to_string() != "CDU")
        {
            msg = "Wrong candidate!";
            return false;
        }
        return true;
    };
    string val;
    
    if (!parser.parse(input, val))
    {
        throw runtime_error("Please vote for CDU instead of " + input);
    }

    return val;
}

int main()
{
    try
    {
        string expr;
        cout << "Welcome to the digital voting center for the Bundestagswahl 2029. Please vote for 'CDU'. Enter 'CDU' now (or 'quit' to exit):\n";
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
