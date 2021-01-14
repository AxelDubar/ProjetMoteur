#ifndef PHYSICOBJECT_H
#define PHYSICOBJECT_H

#include "QVector"
#include "QVector2D"

class PhysicObject
{
public:
    QVector2D velocity;
    QVector2D acceleration;
    QVector2D position;
    QVector2D worldSpacePosition;
    float radius =2.0f;
    float friction=0.5f;
    bool stable=false;
    bool isdead=false;
    float angle;
    int nbBounce =-1;


    void setPosition(float x,float y);
    void setPosition(QVector2D newPos);
    void setPositionFromWorldSpace(float x,float y);
    QVector2D getPosition();
    PhysicObject();
    PhysicObject(float x,float y){
        setPosition(x,y);
    }

    void updatePhysics(QVector<QVector<int>> matriceTerrain);

};

#endif // PHYSICOBJECT_H
