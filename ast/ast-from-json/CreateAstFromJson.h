//
// Created by Faris Cakaric on 30.05.17.
//

#ifndef META_ADL_PARSER_VISITOR_CREATEASTFROMJSON_H
#define META_ADL_PARSER_VISITOR_CREATEASTFROMJSON_H


#include "../nodes/Program.h"
#include "../nodes/DefinitionExpression.h"
#include "../nodes/AndExpression.h"
#include "../nodes/OrExpression.h"
#include "../nodes/NotExpression.h"
#include "../nodes/AtomExpression.h"
#include "../nodes/SetExpression.h"
#include "../nodes/Artifact.h"


#include <json11.hpp>

using json11::Json;

class CreateAstFromJson {

public:
    static Program generateAst(Json json);

private:
    static Expression* generateDefinitionExpressionFromJson(Json json);

    template <typename T>
    static T* generateBinaryExpressionFromJson(Json json);

    static NotExpression* generateNotExpressionFromJson(Json json);

    static AtomExpression* generateAtomExpressionFromJson(Json json);

    static SetExpression* generateSetTermFromJson(Json json);

    static Artifact* generateArtifactFromJson(Json json);

};


#endif //META_ADL_PARSER_VISITOR_CREATEASTFROMJSON_H
