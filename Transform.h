#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QMatrix4x4>
class Transform
{
    public:
        explicit Transform(QMatrix4x4 m);
        explicit Transform();
        void modify(QMatrix4x4 mat);
        QVector4D apply(QVector4D p);
        QMatrix4x4 apply(QMatrix4x4 mat);
        QVector3D applyToPoint(QVector3D p);
        QVector3D applyToVector(QVector3D v);
        QVector3D applyToVersor(QVector3D v);
        QMatrix4x4 getMatrix();
        QMatrix4x4 m;
     private:


};

#endif // TRANSFORM_H
