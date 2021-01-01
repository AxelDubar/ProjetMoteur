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

float MainWidget::cameraSpeed=0.05f;
#include <QMouseEvent>
#include <QTime>

#include <math.h>
MainWidget::MainWidget(QWidget *parent,int fps) :
    QOpenGLWidget(parent),
    geometries(0),
    textureGrass(0),
    textureSnow(0),
    textureRock(0),
    textureHeight(0),
    cameraPos(0.0f,0.0f,7.0f),
    cameraFront(0.0f,0.0f,-1.0f),
    cameraUp(0.0f,1.0f,0.0f),
    cam_orbital(false),
    angle(0),
    angularSpeed(0),
    soleil(&world,"soleil"),
    terre(&soleil,"terre"),
    lune(&terre,"lune")




{

    cam_rotation.rotate(0.5f,QVector3D(0.0f, 0.0f, 1.0f));
    // Use QBasicTimer because its faster than QTimer
    timer.start(1000.0/(float)fps, this);
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete textureHeight;
    delete textureRock;
    delete textureGrass;
    delete textureSnow;
    delete geometries;
    doneCurrent();
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
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
        cameraSpeed+=0.01f;
    }
    else if(e->key()==Qt::Key_Down){
        cameraSpeed-=0.01f;
        if(cameraSpeed<0)
            cameraSpeed=0;
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



    else if(e->key()==Qt::Key_C){
        cam_orbital=!cam_orbital;
        if(!cam_orbital){
            cameraPos=QVector3D(0.0f,0.0f,7.0f);
            cameraFront=QVector3D(0.0f,0.0f,-1.0f);
            cameraUp=QVector3D(0.0f,1.0f,0.0f);
        }
        else{

            cameraPos=QVector3D(0.0f,0.0f,7.0f);
            QMatrix4x4 mat=QMatrix4x4();
            mat.rotate(45.0f,QVector3D(1.0f, 0.0f, 0.0f));
            cameraPos=cameraPos* mat;
            QVector3D up = QVector3D(0.0f, 1.0f, 0.0f);

            QVector3D cameraRight = cameraUp.crossProduct(up, cameraFront).normalized();

            cameraUp=cameraUp.crossProduct(cameraFront,cameraRight);
        }
    }

    update();
}


void MainWidget::timerEvent(QTimerEvent *)
{
    angle=(angle+1)%360;
    qInfo("%d",angle);
    QMatrix4x4 tmpMat;

    //Node *soleil=new Node(&world,"soleil");
    QMatrix4x4 matrixSoleil;
    soleil.transform.m=QMatrix4x4();
    soleil.transform.modify(matrixSoleil);

    //Node *terre=new Node(soleil,"terre");
    terre.transform.m=QMatrix4x4();
    QMatrix4x4 matrixTerre;

    tmpMat.rotate(23.4f,QVector3D(1.0f,0.0f,0.0f));//tilt
    matrixTerre=tmpMat*matrixTerre;
    tmpMat=QMatrix4x4();

    tmpMat.rotate(angle,QVector3D(0.0f,1.0f,0.0f));
    matrixTerre=tmpMat*matrixTerre;
    tmpMat=QMatrix4x4();

    tmpMat.translate(QVector3D(3.0f,0.0f,0.0f));
    matrixTerre=tmpMat*matrixTerre;
    tmpMat=QMatrix4x4();

    matrixTerre.scale(0.5);//size
    tmpMat.rotate(angle,QVector3D(0.0f,1.0f,0.0f));
    matrixTerre=tmpMat*matrixTerre;
    tmpMat=QMatrix4x4();

    //matrixTerre=otherTerre*matrixTerre;
    terre.transform.modify(matrixTerre);

    //Node *lune=new Node(terre,"lune");
    lune.transform.m=QMatrix4x4();
    QMatrix4x4 matrixLune;
    matrixLune.rotate(6.5f,QVector3D(1.0f,0.0f,0.0f));

    matrixLune.translate(QVector3D(2.0f,0.0f,0.0f));
    matrixLune.scale(0.5);
    matrixLune=tmpMat*matrixLune;
    lune.transform.modify(matrixLune);
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Request an update
        update();
    }
    if(cam_orbital){
        cam_rotation=QMatrix4x4();
        cam_rotation.rotate(cameraSpeed,QVector3D(0.0f, 0.0f, 1.0f));
        cameraPos= cameraPos*cam_rotation;

        cameraFront=(QVector3D(0.0f,0.0f,0.0f)-cameraPos).normalized();
        QVector3D up = QVector3D(0.0f, 0.0f, 1.0f);

        QVector3D cameraRight = cameraUp.crossProduct(up, cameraFront).normalized();

        cameraUp=cameraUp.crossProduct(cameraFront,cameraRight);
        update();
    }
    update();
}
//! [1]

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
    textureHeight = new QOpenGLTexture(QImage(":/heightmap-1024x1024.png").mirrored());
    textureHeight->setMinificationFilter(QOpenGLTexture::Nearest);
    textureHeight->setMagnificationFilter(QOpenGLTexture::Linear);
    textureHeight->setWrapMode(QOpenGLTexture::Repeat);

    textureGrass = new QOpenGLTexture(QImage(":/grass.png").mirrored());
    textureGrass->setMinificationFilter(QOpenGLTexture::Nearest);
    textureGrass->setMagnificationFilter(QOpenGLTexture::Linear);
    textureGrass->setWrapMode(QOpenGLTexture::Repeat);

    textureRock = new QOpenGLTexture(QImage(":/rock.png").mirrored());
    textureRock->setMinificationFilter(QOpenGLTexture::Nearest);
    textureRock->setMagnificationFilter(QOpenGLTexture::Linear);
    textureRock->setWrapMode(QOpenGLTexture::Repeat);

    textureSnow = new QOpenGLTexture(QImage(":/snowrocks.png").mirrored());
    textureSnow->setMinificationFilter(QOpenGLTexture::Nearest);
    textureSnow->setMagnificationFilter(QOpenGLTexture::Linear);
    textureSnow->setWrapMode(QOpenGLTexture::Repeat);
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


void MainWidget::paintSceneElementsReccur(Node currentNode,Transform currentTransform){
    program.setUniformValue("model", currentTransform.getMatrix());
    //if(currentNode.name=="lune")
    geometries->drawGeometry(&program,0);
    qInfo("%s ",currentNode.name);
    for(int i=0;i<currentNode.getChilds().size();i++){


        paintSceneElementsReccur(*currentNode.getChilds().at(i),Transform(currentTransform.apply(currentNode.getChilds().at(i)->transform.getMatrix())));

    }

}

void MainWidget::paintSceneElements(Node currentNode){
    Transform currentTransform=currentNode.transform;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    qInfo("%s ",currentNode.name);
    for(int i=0;i<currentNode.getChilds().size();i++){

        paintSceneElementsReccur(*currentNode.getChilds().at(i),Transform(currentTransform.apply(currentNode.getChilds().at(i)->transform.getMatrix())));
    }
    program.setUniformValue("model", currentNode.transform.getMatrix());
}


void MainWidget::paintGL()
{
    // Clear color and depth buffer
    qInfo("test");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    textureSnow->bind(0);
    textureGrass->bind(1);
    textureRock->bind(2);
    textureHeight->bind(3);

//! [6]


    // Calculate model view transformation
    glPolygonMode(GL_FRONT,GL_LINE);
    QMatrix4x4 model;

    //QImage img=QImage(":/heightmap-1024x1024.png");
    //qInfo("%lf",img.pixel(5,5));
    model.rotate(rotation);
    //model.scale(0.3f);



    QMatrix4x4 view;
    view.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // Set modelview-projection matrix
    program.setUniformValue("camera_pos", cameraPos);
    program.setUniformValue("view", view);
    program.setUniformValue("projection", projection);


//! [6]

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("textureSnow", 0);
    program.setUniformValue("textureGrass", 1);
    program.setUniformValue("textureRock", 2);
    program.setUniformValue("textureHeight", 3);

    paintSceneElements(world);

    // Draw cube geometry
    geometries->drawGeometry(&program,0);
    geometries->drawPlanGeometry(&program,1);

}
