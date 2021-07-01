#include "ParseSparql.h"




void ParseSparql::parseBaseDecl(SparqlParser::BaseDeclContext *ctx) {
    groupGraph.BASE = ctx->IRI_REF()->getText();
}


void ParseSparql::parsePrefixDecl(SparqlParser::PrefixDeclContext *ctx) {
    string pName = ctx->PNAME_NS()->getText();
    pName.erase(pName.size() - 1);
    groupGraph.prefix.emplace(pName, ctx->IRI_REF()->getText());
}

/**
 * query
    : prologue ( selectQuery | askQuery ) EOF          //| constructQuery | describeQuery 删掉。不需要
    ;
 * @param tree
 */
void ParseSparql::parseQuery(SparqlParser::QueryContext *tree) {
    //parse prologue
    if (tree->prologue()->baseDecl()){
        parseBaseDecl(tree->prologue()->baseDecl());
    }
    for (int i = 0; i < tree->prologue()->prefixDecl().size(); ++i) {
        parsePrefixDecl(tree->prologue()->prefixDecl(i));
    }
    //parse
    if (tree->selectQuery()) {
        parseSelectQuery(tree->selectQuery());
    }
    //|  constructQuery | describeQuery| askQuery TODO

}


/**
 *
selectQuery
    : 'SELECT' ( 'DISTINCT' | 'REDUCED' )? ( var+ | '*' ) datasetClause* whereClause solutionModifier
    'DISTINCT' | 'REDUCED' 消除重复

 * @param ctx
 */
void ParseSparql::parseSelectQuery(SparqlParser::SelectQueryContext *ctx) {
    //( var+ | '*' ) 需要哪些变量 或者全部
    if (ctx->var().size() != 0) {
        string tmp;
        for (int i = 0; i < ctx->var().size(); ++i) {
            tmp = ctx->var(i)->getText();
            groupGraph.reqVar.emplace_back(tmp);   //保持输出顺序
            groupGraph.varSet.emplace(tmp);    //查看ifSelected 是否需要的输出
        }
    }
    //指定数据库 TODO
    if (ctx->datasetClause().size() != 0) {

    }
    //必有
    ParseInfo tmp;
    this->groupGraph.pInfos.emplace_back(tmp);
    parseGroupGraphPattern(ctx->whereClause()->groupGraphPattern(), this->groupGraph.pInfos[0]);

    if (ctx->solutionModifier()) { //排序 limit等
        parseSolutionModifier(ctx->solutionModifier());
    }

//fpga
//    OC_Data data= sendReqs(SEND_REQ_MODE, groupGraph.oc_reqs);
//测试
    OC_Data data;
    vector<Triple> localData;
    readData(this->dataPath, localData);
    for (int i = 0; i < groupGraph.oc_reqs.triples.size(); ++i) {
        data.datas.emplace_back(Turtle());
        data.datas[i].triples=localData;
    }
//测试over
    int offset = 0;
    for (int i = 0; i < groupGraph.pInfos.size(); ++i) {
        parseData(groupGraph.pInfos[i], data,offset);
        offset += groupGraph.pInfos[i].reqs.size();
    }
    resultOut(this->groupGraph);

/*    cout << "parse end"<< endl;
    for (auto iter = pInfo.varMap.begin(); iter != pInfo.varMap.end(); ++iter) {
        cout << iter->first<<" : "<<iter->second.str<<"--"<<iter->second.selected<< endl;
    }
    cout << pInfo.oriReqs.size()<< endl;*/


}

/**
 * groupGraphPattern
    : '{' triplesBlock? ( ( graphPatternNotTriples | filter ) '.'? triplesBlock? )* '}'
 * @param ctx
 */
void ParseSparql::parseGroupGraphPattern(SparqlParser::GroupGraphPatternContext *ctx,ParseInfo &pInfo) {
    int tag = 0;
    if (!ctx->graphPatternNotTriples().empty()) {
        for (int i = 0; i < ctx->graphPatternNotTriples().size(); ++i) {
            parseGraphPatternNotTriples(ctx->graphPatternNotTriples(i));
            tag = 1;
        }
    }
    if (tag==1){
        return;
    }
    //作用于整个组
    if (!ctx->filter().empty()) {
        for (int i = 0; i < ctx->filter().size(); ++i) {
            parseFilter(ctx->filter(i));
        }
    }
    if (!ctx->triplesBlock().empty()) {
        for (int i = 0; i < ctx->triplesBlock().size(); ++i) {
            parseTriplesBlock(ctx->triplesBlock(i),pInfo);
        }
    }
    parseGroupGraph(pInfo, groupGraph.oc_reqs);
}

void ParseSparql::parseTriplesBlock(SparqlParser::TriplesBlockContext *ctx,ParseInfo &pInfo) {
    if (ctx->triplesSameSubject()) {
        parseTriplesSameSubject(ctx->triplesSameSubject(),pInfo);
    }
    if (ctx->triplesBlock()) {
        parseTriplesBlock(ctx->triplesBlock(),pInfo);
    }
}

void ParseSparql::parseFilter(SparqlParser::FilterContext *ctx) {
    //TODO
    cout << "parseFilter TODO" << endl;
}


void ParseSparql::parseGraphPatternNotTriples(SparqlParser::GraphPatternNotTriplesContext *ctx) {
    if (ctx->optionalGraphPattern()) {
        parseOptionalGraphPattern(ctx->optionalGraphPattern());
    }
    if (ctx->graphGraphPattern()) {
        parseGraphGraphPattern(ctx->graphGraphPattern());
    }
    if (ctx->groupOrUnionGraphPattern()) {
        parseGroupOrUnionGraphPattern(ctx->groupOrUnionGraphPattern());
    }
}

void ParseSparql::parseOptionalGraphPattern(SparqlParser::OptionalGraphPatternContext *ctx) {
    //TODO
    cout << "parseOptionalGraphPattern TODO" << endl;
}

void ParseSparql::parseGraphGraphPattern(SparqlParser::GraphGraphPatternContext *ctx) {
    //TODO
    cout << "parseGraphGraphPattern TODO" << endl;
}

void ParseSparql::parseGroupOrUnionGraphPattern(SparqlParser::GroupOrUnionGraphPatternContext *ctx) {
    ParseInfo tmp;
    groupGraph.pInfos.emplace_back(tmp);
    if (!ctx->groupGraphPattern().empty()) {
        for (int i = 0; i < ctx->groupGraphPattern().size(); ++i) {
            parseGroupGraphPattern(ctx->groupGraphPattern(i), groupGraph.pInfos[i]);
        }
    }

}

void ParseSparql::parseSolutionModifier(SparqlParser::SolutionModifierContext *ctx) {

};


/**
 * triplesSameSubject
    : varOrTerm propertyListNotEmpty
    | triplesNode propertyList  TODO
    ;
 * @param ctx
 */

void ParseSparql::parseTriplesSameSubject(SparqlParser::TriplesSameSubjectContext *ctx,ParseInfo &pInfo) {
    if (ctx->triplesNode()) {
        //visitTriplesNode TODO
        parseTriplesNode(ctx->triplesNode());
    }
    if (ctx->varOrTerm()) {
        reqNode s = parseVarOrTerm(ctx->varOrTerm(),pInfo);
        parsePropertyListNotEmpty(ctx->propertyListNotEmpty(), s,pInfo);
    }
}


/**
 * propertyListNotEmpty
    : verb objectList ( ';' ( verb objectList )? )*
    ;
 * @param ctx
 * @param s
 */

void ParseSparql::parsePropertyListNotEmpty(SparqlParser::PropertyListNotEmptyContext *ctx, reqNode &s,ParseInfo &pInfo) {
    for (int i = 0; i < ctx->verb().size(); ++i) {
        SparqlParser::VarOrIRIrefContext *pContext = ctx->verb(i)->varOrIRIref();
        if (pContext->isEmpty()) {
            cout << "error:: case verb 'a' " << endl;
            return;
        }
        reqNode p = parseVarOrIRIref(pContext,pInfo);
        parseObjectList(ctx->objectList(i), s, p,pInfo);
    }
}


void ParseSparql::parseTriplesNode(SparqlParser::TriplesNodeContext *ctx) {
    cout << "parseTriplesNode TODO" << endl;
}

/**
 * object: graphNode     TODO graphNode: varOrTerm | triplesNode
 * @param ctx
 * @param s
 * @param p
 */
void ParseSparql::parseObjectList(SparqlParser::ObjectListContext *ctx, reqNode &s, reqNode &p,ParseInfo &pInfo) {
    for (int i = 0; i < ctx->object().size(); ++i) {
        SparqlParser::ObjectContext *oContext = ctx->object(i);
        if (oContext->graphNode()->triplesNode()) {
            parseTriplesNode(oContext->graphNode()->triplesNode());
            return;
        }
        pInfo.oriReqs.emplace_back(Req(s, p, parseVarOrTerm(oContext->graphNode()->varOrTerm(),pInfo)));
    }
}


reqNode ParseSparql::parseVarOrIRIref(SparqlParser::VarOrIRIrefContext *ctx,ParseInfo &pInfo) {
    //parseQuery verb
    reqNode p;
    if (ctx->var()) {
        string tmp = ctx->var()->getText();
        p = make_pair(tmp, pInfo.VAR);
        if (pInfo.varMap.find(tmp) == pInfo.varMap.end()) {
            if (groupGraph.varSet.find(tmp) != groupGraph.varSet.end()) {
                pInfo.varMap.emplace(tmp, Var(tmp, true));
            } else{
                pInfo.varMap.emplace(tmp, Var(tmp));
            }
            pInfo.reqVar.emplace_back(&pInfo.varMap.find(tmp)->second);
        }
    } else {
        p = make_pair(parseIriRef(ctx->iriRef()), pInfo.IRI);
    }
    return p;
}


reqNode ParseSparql::parseVarOrTerm(SparqlParser::VarOrTermContext *ctx,ParseInfo &pInfo) {
    reqNode s;
    if (ctx->var()) {
        string tmp = ctx->var()->getText();
        s = make_pair(tmp, pInfo.VAR);
        if (pInfo.varMap.find(tmp) == pInfo.varMap.end()) {
            if (groupGraph.varSet.find(tmp) != groupGraph.varSet.end()) {
                pInfo.varMap.emplace(tmp, Var(tmp, true));
            } else{
                pInfo.varMap.emplace(tmp, Var(tmp));
            }
            pInfo.reqVar.emplace_back(&pInfo.varMap.find(tmp)->second);
        }
    } else {
        s = parseGraphTerm(ctx->graphTerm(),pInfo);
    }
    return s;
}



/**
 * parseQuery graphTerm
    : iriRef    | rdfLiteral    | numericLiteral    | booleanLiteral    | blankNode    | NIL
    blankNode TODO
 * @param ctx
 * @return
 */
reqNode ParseSparql::parseGraphTerm(SparqlParser::GraphTermContext *ctx,ParseInfo &pInfo) {
    string temp;
    if (ctx->rdfLiteral()) {
        SparqlParser::RdfLiteralContext *pRdfLiteralContext = ctx->rdfLiteral();
        if (pRdfLiteralContext->iriRef()) {
            temp += pRdfLiteralContext->getText() + "^^" + parseIriRef(pRdfLiteralContext->iriRef());
        } else{
            temp += pRdfLiteralContext->getText();
        }
        return reqNode(temp, pInfo.IRI);
    }
    if (ctx->iriRef()) {
        return reqNode(parseIriRef(ctx->iriRef()), pInfo.IRI);
    }
    return reqNode(ctx->getText(), pInfo.IRI);
}

/**
 * parseQuery iriRef
    : IRI_REF    | prefixedName
 * */
string ParseSparql::parseIriRef(SparqlParser::IriRefContext *ctx) {
    string temp;
    if (groupGraph.ifUsePrefix) {
        if (ctx->prefixedName()) {
            string pName = ctx->prefixedName()->getText();
            int offset = pName.find(":");
            string prefix = pName.substr(0, offset);
            string context = pName.substr(offset + 1, pName.length());
            if (prefix.empty() && !groupGraph.BASE.empty()) {
                temp += groupGraph.BASE.substr(0, groupGraph.BASE.length() - 1) + context + ">";
            } else {
                string prefixContext;
                prefixContext = groupGraph.prefix.find(prefix)->second;
                temp += prefixContext.substr(0, prefixContext.length() - 1) + context + ">";
            }
            return temp;
        }
        if (!groupGraph.BASE.empty()) {
            temp += groupGraph.BASE.substr(0, groupGraph.BASE.length() - 1);
            temp += ctx->getText().substr(1);
        } else {
            temp = ctx->getText();
        }
    } else{
        temp = ctx->getText();
        if (temp.find("<")==temp.npos){
            return temp;
        }
    }



    return temp.substr(1,temp.size()-2);
}








