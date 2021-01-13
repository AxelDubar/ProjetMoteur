#ifndef TERRAINGEN_H
#define TERRAINGEN_H

#include <QVector>
#include "PerlinNoise.h"

class TerrainGen {
public:
    QVector<QVector<int>> generateTerrain(int cols,int rows,unsigned int seed);

private:
    double turbulence(double x,double y, double f,int cols,PerlinNoise p);
    QVector<QVector<int>> conversion(QVector<QVector<float>> matFloat);
    QVector<QVector<int>> dilatation(QVector<QVector<int>> matIn,int nH, int nW);
    QVector<QVector<int>> erosion(QVector<QVector<int>> matIn,int nH, int nW);
    void remplissage4(QVector<QVector<float>> &matrice,int i,int j,int nH,int nW);

};

#endif // TERRAINGEN_H
