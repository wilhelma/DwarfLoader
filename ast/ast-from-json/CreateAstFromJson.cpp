//
// Created by Faris Cakaric on 30.05.17.
//

#include "CreateAstFromJson.h"

Program CreateAstFromJson::generateAst(Json json) {
    std::vector<std::unique_ptr<DefinitionExpression>> definitionExpressions;
    Json::array statements = json.array_items();

    for (int i = 0; i < statements.size(); i++) {
        definitionExpressions.push_back(std::unique_ptr<DefinitionExpression>(generateDefinitionExpressionFromJson(statements[i])));
    }
    return Program{definitionExpressions};
}

DefinitionExpression* CreateAstFromJson::generateDefinitionExpressionFromJson(Json json) {
    std::unique_ptr<Component> left(generateComponentFromJson(json["left"]));
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
    }  else if(rightSide["type"] == "Component") {
        right.reset(generateComponentFromJson(rightSide));
    } else if(rightSide["type"] == "SetExpression") {
        right.reset(generateSetTermFromJson(rightSide));
    }

    return new DefinitionExpression(left, right);
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
    }  else if(leftSide["type"] == "Component") {
        left = std::unique_ptr<Component>(generateComponentFromJson(leftSide));
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
    }  else if(rightSide["type"] == "Component") {
        right = std::unique_ptr<Component>(generateComponentFromJson(rightSide));
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
    }  else if(rightSide["type"] == "Component") {
        right = std::unique_ptr<Component>(generateComponentFromJson(rightSide));
    } else if(rightSide["type"] == "SetExpression") {
        right = std::unique_ptr<SetExpression>(generateSetTermFromJson(rightSide));
    }

    return new NotExpression{right};
}

AtomExpression* CreateAstFromJson::generateAtomExpressionFromJson(Json json) {
    return new AtomExpression{json["rule"].string_value(), json["regex"].string_value()};
}

SetExpression* CreateAstFromJson::generateSetTermFromJson(Json json) {
    std::vector<std::unique_ptr<Component>> elements;
    Json::array jsonElements = json["elements"].array_items();

    for(int i = 0; i < jsonElements.size(); i++)
        elements.push_back(std::unique_ptr<Component>(generateComponentFromJson(jsonElements[i])));

    return new SetExpression{elements};
}

Component* CreateAstFromJson::generateComponentFromJson(Json json) {
    return new Component{json["name"].string_value()};
}