#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <vector>
#include "Transform.h"


class Node {
private:
    Node *pere;
    std::vector<Node*> enfants;


public:
    char name[50];
    Transform transform;
    Transform mouvement;
    explicit Node(Node *pere,std::string name);
    explicit Node();

    bool isRoot();
    void addChild(Node *enfant);
    std::vector<Node*> getChilds();



}
;
#endif // GRAPHNODE_H
