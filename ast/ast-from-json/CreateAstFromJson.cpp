//
// Created by Faris Cakaric on 30.05.17.
//

#include "CreateAstFromJson.h"
#include "../nodes/AssignmentExpression.h"

Program CreateAstFromJson::generateAst(Json json) {
    std::vector<std::unique_ptr<Expression>> definitionExpressions;
    Json::array statements = json.array_items();

    for (size_t i = 0; i < statements.size(); i++) {
        definitionExpressions.push_back(std::unique_ptr<Expression>(generateDefinitionExpressionFromJson(statements[i])));
    }
    return Program{definitionExpressions};
}

Expression* CreateAstFromJson::generateDefinitionExpressionFromJson(Json json) {
    std::unique_ptr<Artifact> left(generateArtifactFromJson(json["left"]));
    Json rightSide = json["right"];
    std::unique_ptr<Expression> right;
    if(rightSide["type"] == "OrExpression") {
        right.reset(generateBinaryExpressionFromJson<OrExpression>(rightSide));
    } else if(rightSide["type"] == "AndExpression") {
        right.reset(generateBinaryExpressionFromJson<AndExpression>(rightSide));
    } else if(rightSide["type"] == "NotExpression") {
        right.reset(generateNotExpressionFromJson(rightSide));
    } else if(rightSide["type"] == "AtomExpression") {
        right.reset(generateAtomExpressionFromJson(rightSide));
    }  else if(rightSide["type"] == "Artifact") {
        right.reset(generateArtifactFromJson(rightSide));
    } else if(rightSide["type"] == "SetExpression") {
        right.reset(generateSetTermFromJson(rightSide));
    }
    if(json["operator"] == "=")
        return new DefinitionExpression(left, right);
    else
        return new AssignmentExpression(left, right);
}

template <typename T>
T* CreateAstFromJson::generateBinaryExpressionFromJson(Json json) {
    Json leftSide = json["left"];
    std::unique_ptr<Expression> left;
    if(leftSide["type"] == "OrExpression") {
        left = std::unique_ptr<OrExpression>(generateBinaryExpressionFromJson<OrExpression>(leftSide));
    } else if(leftSide["type"] == "AndExpression") {
        left = std::unique_ptr<AndExpression>(generateBinaryExpressionFromJson<AndExpression>(leftSide));
    } else if(leftSide["type"] == "NotExpression") {
        left = std::unique_ptr<NotExpression>(generateNotExpressionFromJson(leftSide));
    } else if(leftSide["type"] == "AtomExpression") {
        left = std::unique_ptr<AtomExpression>(generateAtomExpressionFromJson(leftSide));
    }  else if(leftSide["type"] == "Artifact") {
        left = std::unique_ptr<Artifact>(generateArtifactFromJson(leftSide));
    } else if(leftSide["type"] == "SetExpression") {
        left = std::unique_ptr<SetExpression>(generateSetTermFromJson(leftSide));
    }

    Json rightSide = json["right"];
    std::unique_ptr<Expression> right;
    if(rightSide["type"] == "OrExpression") {
        right = std::unique_ptr<OrExpression>(generateBinaryExpressionFromJson<OrExpression>(rightSide));
    } else if(rightSide["type"] == "AndExpression") {
        right = std::unique_ptr<AndExpression>(generateBinaryExpressionFromJson<AndExpression>(rightSide));
    } else if(rightSide["type"] == "NotExpression") {
        right = std::unique_ptr<NotExpression>(generateNotExpressionFromJson(rightSide));
    } else if(rightSide["type"] == "AtomExpression") {
        right = std::unique_ptr<AtomExpression>(generateAtomExpressionFromJson(rightSide));
    }  else if(rightSide["type"] == "Artifact") {
        right = std::unique_ptr<Artifact>(generateArtifactFromJson(rightSide));
    } else if(rightSide["type"] == "SetExpression") {
        right = std::unique_ptr<SetExpression>(generateSetTermFromJson(rightSide));
    }

    return new T(left, right);
}

NotExpression* CreateAstFromJson::generateNotExpressionFromJson(Json json) {
    Json rightSide = json["operand"];
    std::unique_ptr<Expression> right;
    if(rightSide["type"] == "OrExpression") {
        right = std::unique_ptr<OrExpression>(generateBinaryExpressionFromJson<OrExpression>(rightSide));
    } else if(rightSide["type"] == "AndExpression") {
        right = std::unique_ptr<AndExpression>(generateBinaryExpressionFromJson<AndExpression>(rightSide));
    } else if(rightSide["type"] == "NotExpression") {
        right = std::unique_ptr<NotExpression>(generateNotExpressionFromJson(rightSide));
    } else if(rightSide["type"] == "AtomExpression") {
        right = std::unique_ptr<AtomExpression>(generateAtomExpressionFromJson(rightSide));
    }  else if(rightSide["type"] == "Artifact") {
        right = std::unique_ptr<Artifact>(generateArtifactFromJson(rightSide));
    } else if(rightSide["type"] == "SetExpression") {
        right = std::unique_ptr<SetExpression>(generateSetTermFromJson(rightSide));
    }

    return new NotExpression{right};
}

AtomExpression* CreateAstFromJson::generateAtomExpressionFromJson(Json json) {
    return new AtomExpression{json["rule"].string_value(), json["regex"].string_value()};
}

SetExpression* CreateAstFromJson::generateSetTermFromJson(Json json) {
    std::vector<std::unique_ptr<Artifact>> elements;
    Json::array jsonElements = json["elements"].array_items();

    for(size_t i = 0; i < jsonElements.size(); i++)
        elements.push_back(std::unique_ptr<Artifact>(generateArtifactFromJson(jsonElements[i])));

    return new SetExpression{elements};
}

Artifact* CreateAstFromJson::generateArtifactFromJson(Json json) {
    return new Artifact{json["name"].string_value()};
}