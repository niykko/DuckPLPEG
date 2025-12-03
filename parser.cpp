#include "peglib.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace peg;
using namespace std;

int main(void)
{
    // (2) Make a parser

    std::ifstream file("gram.peg");
    if (!file.is_open())
    {
        std::cerr << "Failed to open grammar file!\n";
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string grammar = buffer.str();

    parser parser(grammar);
    assert(static_cast<bool>(parser) == true);

    // (3) Setup actions
    parser["Additive"] = [](const SemanticValues &vs)
    {
        switch (vs.choice())
        {
        case 0: // "Multiplicative '+' Additive"
            return any_cast<int>(vs[0]) + any_cast<int>(vs[1]);
        default: // "Multiplicative"
            return any_cast<int>(vs[0]);
        }
    };

    parser["Multiplicative"] = [](const SemanticValues &vs)
    {
        switch (vs.choice())
        {
        case 0: // "Primary '*' Multiplicative"
            return any_cast<int>(vs[0]) * any_cast<int>(vs[1]);
        default: // "Primary"
            return any_cast<int>(vs[0]);
        }
    };

    parser["Number"] = [](const SemanticValues &vs)
    {
        return vs.token_to_number<int>();
    };

    // (4) Parse
    parser.enable_packrat_parsing(); // Enable packrat parsing.

    int val;
    parser.parse(" (1 + 2) * 3 ", val);

    assert(val == 9);
}