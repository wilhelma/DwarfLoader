//
// Created by Faris Cakaric on 30.05.17.
//

#ifndef META_ADL_PARSER_VISITOR_ASTNODE_H
#define META_ADL_PARSER_VISITOR_ASTNODE_H


class Visitor;

class ASTNode {
public:
    virtual void accept(Visitor& v) = 0;
};


#endif //META_ADL_PARSER_VISITOR_ASTNODE_H
