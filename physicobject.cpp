#include "physicobject.h"

PhysicObject::PhysicObject()
{

    velocity=QVector2D(0,0);
}

void PhysicObject::setPosition(float x,float y){
    position.setX(x);position.setY(y);

    float x_norm=x/1024.0;
    x_norm*=3;
    x_norm-=1.5;
    float y_norm=y/512.0;
    y_norm*=-2;
    y_norm+=1;
    worldSpacePosition=QVector2D(x_norm,y_norm);
}

void PhysicObject::setPositionFromWorldSpace(float x,float y){
    worldSpacePosition=QVector2D(x,y);


    float x_pos=x,y_pos=y;
    x_pos+=1.5;
    x_pos/=3;
    x_pos=x_pos*1024;
    y_pos-=1;
    y_pos/=-2;
    y_pos=y*512;

    position.setX(x_pos);position.setY(y_pos);

}

void PhysicObject::setPosition(QVector2D newPos){
    setPosition(newPos.x(),newPos.y());
}

void PhysicObject::updatePhysics(QVector<QVector<int>> matriceTerrain){

    //gravity
    acceleration.setY(0.1f);

    velocity+=acceleration;
    QVector2D potentialPosition=position+velocity;

    acceleration=QVector2D(0,0);
    stable=false;
    bool has_collison=false;

    //Collision
    angle =atan2f(velocity.y(),velocity.x());
    QVector2D response;

    for(float r= angle - 3.14159f/2.0f; r< angle + 3.14159f/2.0f; r+=3.14159f/8.0f){
        QVector2D testPosition = QVector2D(radius * cosf(r)+ potentialPosition.x(), radius * sinf(r)+ potentialPosition.y());

        //Verify if not out of bound
        if(testPosition.x()>=matriceTerrain.size()){
            testPosition.setX(matriceTerrain.size()-1);
        }
        if(testPosition.x()<0){
            testPosition.setX(0);
        }
        if(testPosition.y()>=matriceTerrain[0].size()){
            testPosition.setY(matriceTerrain[0].size()-1);
            isdead=true;
        }
        if(testPosition.y()<0){
            testPosition.setY(0);
        }

        if(matriceTerrain[(int)testPosition.x()][(int)testPosition.y()]!=0){
            response+=potentialPosition-testPosition;
            has_collison=true;
        }
    }

    float velocityNorm=sqrt(velocity.x()*velocity.x()+velocity.y()*velocity.y());
    float responseNorm=sqrt(response.x()*response.x()+response.y()*response.y());
    if(has_collison){
        stable=true;

        float dotProdResp= velocity.x() * (response.x()/responseNorm) + velocity.y() * (response.y()/responseNorm);
        velocity=friction*QVector2D( (-2.0f*dotProdResp*(response.x()/responseNorm))+velocity.x() , (-2.0f*dotProdResp*(response.y()/responseNorm))+velocity.y() );

        if(nbBounce >0 ){
            nbBounce--;
            isdead = nbBounce==0;
        }

    }
    else{
        setPosition(potentialPosition);
    }

    if(velocityNorm<0.1f) stable=true;

}




QVector2D PhysicObject::getPosition(){
    return worldSpacePosition;
}
