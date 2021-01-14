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

#include "geometryengine.h"
#include "terraingen.h"

#include <iostream>
#include <QVector2D>
#include <QImage>
#include <QVector3D>
#include <time.h>
#include <QOpenGLTexture>
#include "BasicIO.h"

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0]
GeometryEngine::GeometryEngine():size_x(1024),size_y(512)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    /*arrayBuf.create();
    indexBuf.create();*/

    // Initializes cube geometry and transfers it to VBOs
    //initCubeGeometry();
    //initPlanGeometry();
    qInfo("Init");
    //LoadObject("sphere.obj");




    initTerrain();
    newWorm();
}

GeometryEngine::~GeometryEngine()
{
    for(int i=0;i<arrayBufVector.size();i++){
    arrayBufVector[i].destroy();
    indexBufVector[i].destroy();
    }
}
//! [0]

QOpenGLTexture *textureTerrain;
bool init=false;
int GeometryEngine::initTerrain()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.

    TerrainGen generator;
    qInfo("debut terrain");
    srand(time(NULL));
    matriceTerrain=generator.generateTerrain(size_y,size_x,rand());

    qInfo("good");

    QVector<VertexData> vertices;
    QVector<GLushort> indices;


    vertices.push_back({QVector3D(-1.5,1.0,0.0),QVector2D(0.0f,1.0f)});
    vertices.push_back({QVector3D(1.5,1.0,0.0),QVector2D(1.0f,1.0f)});
    vertices.push_back({QVector3D(1.5,-1.0,0.0),QVector2D(1.0f,0.0f)});
    vertices.push_back({QVector3D(-1.5,-1.0,0.0),QVector2D(0.0f,0.0f)});

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

//! [1]
    // Transfer vertex data to VBO 0
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf(QOpenGLBuffer::IndexBuffer);
    arrayBuf.create();
    indexBuf.create();

    qInfo("test start");

    arrayBuf.bind();
    arrayBuf.allocate(vertices.data(), vertices.size() * sizeof(VertexData));

    qInfo("test allocate vertex");
    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices.data(), indices.size()* sizeof(GLushort));

    qInfo("test allocate vertex");
    arrayBufVector.push_back(arrayBuf);
    indexBufVector.push_back(indexBuf);
    //! [1]
    qInfo("%d",arrayBuf.size());


    QImage imagen(size_x,size_y, QImage::Format_RGBA8888);
   for(int i=0;i<size_x;++i){
       for(int j=0;j<size_y;++j){
           //qInfo("%d %d %d",i,j,matriceTerrain[i][j]);
           imagen.setPixel(i,j, qRgba(matriceTerrain[i][j]*25,matriceTerrain[i][j]*255,matriceTerrain[i][j]*25,matriceTerrain[i][j]*255));
       }
   }
imagen.save("image.png");
   textureTerrain = new QOpenGLTexture(imagen.mirrored());
   textureTerrain->setMinificationFilter(QOpenGLTexture::Nearest);
   textureTerrain->setMagnificationFilter(QOpenGLTexture::Linear);
   textureTerrain->setWrapMode(QOpenGLTexture::Repeat);
    qInfo("fin gen");
    init=true;
    return arrayBufVector.size()-1;
}

void GeometryEngine::updateTerrain(){
    QImage imagen(size_x,size_y, QImage::Format_RGBA8888);
   for(int i=0;i<size_x;++i){
       for(int j=0;j<size_y;++j){
           //qInfo("%d %d %d",i,j,matriceTerrain[i][j]);
           imagen.setPixel(i,j, qRgba(matriceTerrain[i][j]*25,matriceTerrain[i][j]*255,matriceTerrain[i][j]*25,matriceTerrain[i][j]*255));
       }
   }
imagen.save("image.png");
   textureTerrain = new QOpenGLTexture(imagen.mirrored());
   textureTerrain->setMinificationFilter(QOpenGLTexture::Nearest);
   textureTerrain->setMagnificationFilter(QOpenGLTexture::Linear);
   textureTerrain->setWrapMode(QOpenGLTexture::Repeat);
}



int GeometryEngine::LoadObject(const std::string filename){

    std::vector<QVector3D> vertices;
    std::vector<std::vector<GLushort>> indices;

    OBJIO::open("sphere.obj",vertices,indices,false,false,false);

    const int size_indices = indices.size() * 3;

    GLushort* t_indices = new GLushort[size_indices];

    for(size_t i = 0; i < indices.size(); i++){
        for(size_t j = 0; j < indices[i].size(); j++){
            t_indices[i * 3 + j] = indices[i][j];
        }
    }

//! [1]
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf(QOpenGLBuffer::IndexBuffer);
    arrayBuf.create();
    indexBuf.create();


    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices.data(), vertices.size() * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(t_indices, size_indices * sizeof(GLushort));

    arrayBufVector.push_back(arrayBuf);
    indexBufVector.push_back(indexBuf);
    qInfo("fin init");
    return arrayBufVector.size()-1;
}


int GeometryEngine::newWorm(){

    std::vector<VertexData> vertices;
    std::vector<GLushort> indices;

    vertices.push_back({QVector3D(-0.05,0.05,0.0),QVector2D(1.0f,1.0f)});
    vertices.push_back({QVector3D(0.05,0.05,0.0),QVector2D(0.0f,1.0f)});
    vertices.push_back({QVector3D(0.05,-0.05,0.0),QVector2D(0.0f,0.0f)});
    vertices.push_back({QVector3D(-0.05,-0.05,0.0),QVector2D(1.0f,0.0f)});

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);


//! [1]
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf(QOpenGLBuffer::IndexBuffer);
    arrayBuf.create();
    indexBuf.create();


    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices.data(), vertices.size() * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices.data(), indices.size() * sizeof(GLushort));

    arrayBufVector.push_back(arrayBuf);
    indexBufVector.push_back(indexBuf);
    qInfo("fin init");
    return arrayBufVector.size()-1;
}


//! [2]


void GeometryEngine::drawWorm(QOpenGLShaderProgram *program,int numIndex)
{
    // Tell OpenGL which VBOs to use
    arrayBufVector[numIndex].bind();
    indexBufVector[numIndex].bind();

    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, indexBufVector[numIndex].size(), GL_UNSIGNED_SHORT, 0);

}

void GeometryEngine::drawPlanGeometry(QOpenGLShaderProgram *program,int numIndex)
{
    // Tell OpenGL which VBOs to use
    arrayBufVector[numIndex].bind();
    indexBufVector[numIndex].bind();
    quintptr offset = 0;

    if(init){
       textureTerrain->bind(1);
       program->setUniformValue("textureTerrain", 1);


        // Tell OpenGL programmable pipeline how to locate vertex position data
        int vertexLocation = program->attributeLocation("a_position");
        program->enableAttributeArray(vertexLocation);
        program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

        // Offset for texture coordinate
        offset += sizeof(QVector3D);

        // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
        int texcoordLocation = program->attributeLocation("a_texcoord");
        program->enableAttributeArray(texcoordLocation);
        program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

        // Draw cube geometry using indices from VBO 1
        glDrawElements(GL_TRIANGLES, indexBufVector[numIndex].size(), GL_UNSIGNED_SHORT, 0);

        arrayBufVector[numIndex].release();
        indexBufVector[numIndex].release();

    }

}

void GeometryEngine::drawGeometry(QOpenGLShaderProgram *program,int numIndex)
{
    // Tell OpenGL which VBOs to use
    arrayBufVector[numIndex].bind();
    indexBufVector[numIndex].bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));



    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, indexBufVector[numIndex].size(), GL_UNSIGNED_SHORT, 0);
}
