#include "Transform.h"
#include <iostream>
#include <QVector2D>
#include <QVector3D>

Transform::Transform()
{
    m=QMatrix4x4();
    m.setToIdentity();
}

Transform::Transform(QMatrix4x4 M)
{
    m=M;
}

QVector4D Transform::apply(QVector4D p){
    return m*p;
}

QVector3D Transform::applyToPoint(QVector3D p){
    return m*p;
}
QVector3D Transform::applyToVector(QVector3D v){
    return m*v;
}
QVector3D Transform::applyToVersor(QVector3D v){
    return m*v;
}

void Transform::modify(QMatrix4x4 mat){
    m=m*mat;
}
QMatrix4x4 Transform::apply(QMatrix4x4 mat){
   return m*mat;
}

QMatrix4x4 Transform::getMatrix(){
    return m;
}
