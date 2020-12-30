#include "Transform.h"
#include "GraphNode.h"
#include <iostream>
#include <QVector2D>
#include <QVector3D>

Node::Node()
{
    pere=NULL;
}

Node::Node(Node *pere,std::string name)
{
   sprintf(this->name,"%s",name.c_str());
   //this->name=name;
   this->pere=pere;
   this->pere->addChild(this);
}

bool Node::isRoot(){
    return pere==NULL;
}

void Node::addChild(Node* enfant){
    enfants.push_back(enfant);
}

std::vector<Node*> Node::getChilds(){
    return enfants;
}
