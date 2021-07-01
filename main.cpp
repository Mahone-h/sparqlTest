#include "transfer/system_fun.h"
#include <iostream>
#include "antlr4-runtime.h"
#include "sparql/SparqlLexer.h"
#include "sparql/SparqlParser.h"
#include "parser/ParseSparql.h"


using namespace antlr4;
using namespace std;
using namespace antlr4::tree;

int main() {
    /**
     * transfer
     */

    setbuf(stdout, NULL);

    ParseSparql sql;
    //不使用prefix
    sql.groupGraph.ifUsePrefix = false;
    sql.dataPath = "/home/mahone/clionProjects/graphPage/test/data3.txt";
    ANTLRInputStream input(readSql("/home/mahone/clionProjects/graphPage/test/test3_9.sql"));
    SparqlLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    cout << input.toString()<< endl;
    tokens.fill();
//    for (auto token : tokens.getTokens()) {
//        std::cout << token->toString() << std::endl;
//    }
    SparqlParser parser(&tokens);
    cout <<"----------------------------" << endl;

    sql.parseQuery(parser.query());

    return 0;


}
