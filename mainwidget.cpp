/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwidget.h"
#include "GraphNode.h"
#include "worm.h"
#include "debris.h"

float MainWidget::cameraSpeed=0.05f;
#include <QMouseEvent>
#include <QTime>
#include <math.h>
MainWidget::MainWidget(QWidget *parent,int fps) :
    QOpenGLWidget(parent),
    geometries(0),
    textureWorm(0),
    textureGrass(0),
    cameraPos(0.0f,0.0f,2.8f),
    cameraFront(0.0f,0.0f,-1.0f),
    cameraUp(0.0f,1.0f,0.0f),
    cam_orbital(false),
    angle(0),
    angularSpeed(0)




{
    wormsList.push_back(Worm());
    wormsList[0].setPosition(300,250);
    cam_rotation.rotate(0.5f,QVector3D(0.0f, 0.0f, 1.0f));
    // Use QBasicTimer because its faster than QTimer
    timer.start(1000.0/(float)fps, this);
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete textureWorm;
    delete geometries;
    doneCurrent();
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
    qInfo("%f %f",mousePressPosition.x(),mousePressPosition.y());
    explosion(mousePressPosition.x(),mousePressPosition.y(),15);
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}
//! [0]

//! [1]

void MainWidget::keyPressEvent(QKeyEvent *e){
    if(e->key()==Qt::Key_Up){
        //wormsList[0].setPosition(wormsList[0].getPosition().x(),wormsList[0].getPosition().y()-1);
    }
    else if(e->key()==Qt::Key_Down){
        //wormsList[0].setPosition(wormsList[0].getPosition().x(),wormsList[0].getPosition().y()+1);
    }
    else if(e->key()==Qt::Key_Left){
        //wormsList[0].setPosition(wormsList[0].getPosition().x()-1,wormsList[0].getPosition().y());
    }
    else if(e->key()==Qt::Key_Right){
        //wormsList[0].setPosition();
    }


    else if(e->key()==Qt::Key_Z){
        cameraPos+= cameraUp * cameraSpeed;
    }
    else if(e->key()==Qt::Key_S){
        cameraPos-= cameraUp * cameraSpeed;
    }
    else if(e->key()==Qt::Key_Q){
        cameraPos-= cameraPos.crossProduct(cameraFront,cameraUp).normalized()*
                cameraSpeed;
    }
    else if(e->key()==Qt::Key_D){
        cameraPos+= cameraPos.crossProduct(cameraFront,cameraUp).normalized()*
                cameraSpeed;
    }

    else if(e->key()==Qt::Key_T){
        cameraFront+= QVector3D().crossProduct(cameraFront,cameraUp).normalized()*
                cameraSpeed;
    }
    else if(e->key()==Qt::Key_R){
        cameraFront-= QVector3D().crossProduct(cameraFront,cameraUp).normalized()*
                cameraSpeed;
    }

    else if(e->key()==Qt::Key_F){
        cameraFront+=cameraUp.crossProduct(cameraFront.crossProduct(cameraFront,cameraUp).normalized(),cameraFront).normalized()*
                cameraSpeed;

        cameraUp+= cameraUp.crossProduct(cameraFront.crossProduct(cameraFront,cameraUp).normalized(),cameraUp).normalized()*
                cameraSpeed;
    }
    else if(e->key()==Qt::Key_G){
        cameraFront-=cameraUp.crossProduct(cameraFront.crossProduct(cameraFront,cameraUp).normalized(),cameraFront).normalized()*
                cameraSpeed;

        cameraUp-= cameraUp.crossProduct(cameraFront.crossProduct(cameraFront,cameraUp).normalized(),cameraUp).normalized()*
                cameraSpeed;
    }



    else if(e->key()==Qt::Key_A){
        cameraPos+= cameraFront*cameraSpeed;
    }
    else if(e->key()==Qt::Key_E){
        cameraPos-= cameraFront*cameraSpeed;
    }

    update();
}


void MainWidget::timerEvent(QTimerEvent *)
{

    for(int i=0;i<wormsList.size();i++){

        wormsList[i].updatePhysics(geometries->matriceTerrain);
    }

    for(int i=0;i<debrisList.size();i++){
        if(debrisList[i].isdead){
            debrisList.remove(i);
            i--;
            continue;
        }
        debrisList[i].updatePhysics(geometries->matriceTerrain);
    }

    update();
}
//! [1]

void MainWidget::explosion(float x,float y,float radius){

    QVector2D explosion_pos=QVector2D(x,y);
    bool terrain_changed=false;
    for(int i=0;i<geometries->matriceTerrain.size();i++){
        for(int j=0;j<geometries->matriceTerrain[i].size();j++){
            if(geometries->matriceTerrain[i][j]==1){
                QVector2D delta=QVector2D(i,j)-explosion_pos;
                float distance=sqrt(delta.x()*delta.x()+delta.y()*delta.y());
                if(distance<radius){
                        geometries->matriceTerrain[i][j]=0;
                        terrain_changed=true;
                }
            }
        }
    }

    if(terrain_changed){
        geometries->updateTerrain();
    }

    for(int i=0;i<wormsList.size();i++){
        QVector2D delta=wormsList[i].position-explosion_pos;
        float distance=sqrt(delta.x()*delta.x()+delta.y()*delta.y());

        if(distance<radius){
            wormsList[i].velocity = (delta/distance) *radius/6.0f;
            wormsList[i].stable=false;
        }

    }


    for(int i=0;i<15;i++){
        debrisList.push_back(Debris());
        debrisList[debrisList.size()-1].setPosition(explosion_pos);
    }
}


void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    //glEnable(GL_CULL_FACE);
//! [2]

    geometries = new GeometryEngine;




}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    // Load cube.png image
    textureWorm = new QOpenGLTexture(QImage(":/worm.png").mirrored());
    textureWorm->setMinificationFilter(QOpenGLTexture::Nearest);
    textureWorm->setMagnificationFilter(QOpenGLTexture::Linear);
    textureWorm->setWrapMode(QOpenGLTexture::Repeat);


    textureGrass = new QOpenGLTexture(QImage(":/grass.png").mirrored());
    textureGrass ->setMinificationFilter(QOpenGLTexture::Nearest);
    textureGrass ->setMagnificationFilter(QOpenGLTexture::Linear);
    textureGrass ->setWrapMode(QOpenGLTexture::Repeat);




}
//! [4]

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.1f, zFar = 14.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]



bool even=true;;
void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 view;
    view.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


    textureWorm->bind(1);
    QMatrix4x4 model;
    for(int i=0;i<wormsList.size();i++){
        model=QMatrix4x4();
        model.translate(QVector3D(wormsList[i].getPosition().x(),wormsList[i].getPosition().y(),0.0));
        model.scale(0.5f);

        // Set modelview-projection matrix
        program.setUniformValue("camera_pos", cameraPos);
        program.setUniformValue("view", view);
        program.setUniformValue("model", model);
        program.setUniformValue("projection", projection);

        // Use texture unit 0 which contains cube.png
        program.setUniformValue("textureTerrain", 1);
        geometries->drawWorm(&program,1);
    }
textureGrass->bind(1);
    for(int i=0;i<debrisList.size();i++){
        model=QMatrix4x4();
        model.translate(QVector3D(debrisList[i].getPosition().x(),debrisList[i].getPosition().y(),0.0));
        model.rotate(atan2(debrisList[i].velocity.y(),debrisList[i].velocity.x())*180/3.14,QVector3D(0.0,0.0,1.0));
        model.scale(0.1f);



        // Set modelview-projection matrix
        program.setUniformValue("camera_pos", cameraPos);
        program.setUniformValue("view", view);
        program.setUniformValue("model", model);
        program.setUniformValue("projection", projection);

        // Use texture unit 0 which contains cube.png
        program.setUniformValue("textureTerrain", 1);
        geometries->drawWorm(&program,1);
    }




    // Set modelview-projection matrix
    program.setUniformValue("camera_pos", cameraPos);
    program.setUniformValue("view", view);
    program.setUniformValue("model", QMatrix4x4());
    program.setUniformValue("projection", projection);


//! [6]

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("textureWorm", 0);

    geometries->drawPlanGeometry(&program,0);

}
