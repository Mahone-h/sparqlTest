#ifndef TESTANTLR_PARSESPARQL_H
#define TESTANTLR_PARSESPARQL_H


#include <iostream>
#include <unordered_map>
#include <vector>
#include "BasicQuery.h"
#include "../sparql/SparqlParser.h"
#include "../transfer/system_fun.h"
using namespace std;

class ParseSparql {
public:
    string dataPath; //测试用
    GroupGraph groupGraph;

    void parseQuery(SparqlParser::QueryContext *tree);

    void parseSelectQuery(SparqlParser::SelectQueryContext *ctx);

    void parseGroupGraphPattern(SparqlParser::GroupGraphPatternContext *ctx,ParseInfo &pInfo);

    void parseGraphPatternNotTriples(SparqlParser::GraphPatternNotTriplesContext *ctx);

    void parseOptionalGraphPattern(SparqlParser::OptionalGraphPatternContext *ctx);

    void parseGraphGraphPattern(SparqlParser::GraphGraphPatternContext *ctx);

    void parseGroupOrUnionGraphPattern(SparqlParser::GroupOrUnionGraphPatternContext *ctx);

    void parseFilter(SparqlParser::FilterContext *ctx);

    void parseTriplesBlock(SparqlParser::TriplesBlockContext *ctx,ParseInfo &pInfo);

    void parseSolutionModifier(SparqlParser::SolutionModifierContext *ctx);

    void parseBaseDecl(SparqlParser::BaseDeclContext *ctx);

    void parsePrefixDecl(SparqlParser::PrefixDeclContext *ctx);

    void parseTriplesSameSubject(SparqlParser::TriplesSameSubjectContext *ctx,ParseInfo &pInfo);

    reqNode parseVarOrIRIref(SparqlParser::VarOrIRIrefContext *ctx,ParseInfo &pInfo);

    reqNode parseVarOrTerm(SparqlParser::VarOrTermContext *ctx,ParseInfo &pInfo);

    void parseTriplesNode(SparqlParser::TriplesNodeContext *ctx);

    reqNode parseGraphTerm(SparqlParser::GraphTermContext *ctx,ParseInfo &pInfo);

    void parsePropertyListNotEmpty(SparqlParser::PropertyListNotEmptyContext *ctx, reqNode &s,ParseInfo &pInfo);

    void parseObjectList(SparqlParser::ObjectListContext *ctx, reqNode &s, reqNode &p,ParseInfo &pInfo);

    string parseIriRef(SparqlParser::IriRefContext *ctx);



};







#endif //TESTANTLR_PARSESPARQL_H
