
#include "terraingen.h"
#include <QQueue>
#include <QImage>

double TerrainGen::turbulence(double x,double y, double f,int cols,PerlinNoise p){
    double t=-0.5;
    for(;f<=cols/12.0;f*=2 )
    t+=abs((p.noise(x,y,0.7)*2-1)/f);
    return t;
}


QVector<QVector<int>> TerrainGen::conversion(QVector<QVector<float>> matFloat){
    QVector<QVector<int>> matInt;
    for(int i=0;i<matFloat.size();i++){
        QVector<int> lineInt;
        for(int j=0;j<matFloat[i].size();j++){
            lineInt.push_back((int)matFloat[i][j]);
        }   
        matInt.push_back(lineInt);
    }
    return matInt;
}

QVector<QVector<int>> TerrainGen::dilatation(QVector<QVector<int>> matIn,int nH, int nW){
    QVector<QVector<int>> matriceOut=matIn;
	for (int j=0; j < nW; j++)//i=0 et i=nH-1
	{
	  if(matIn[0][j]==1){//i=0
        if(j<nW-1){
			matriceOut[0][j+1]=1;//i=0
			matriceOut[1][j+1]=1;
	    }
	    if(j>0){
	   		matriceOut[1][j-1]=1;
	   		matriceOut[0][j-1]=1;
	    }
	   	matriceOut[1][j]=1;//i=0
	   }

	   if(matIn[nH-1][j]==1){//i=nh-1

        if(j<nW-1){
	   		matriceOut[nH-1][j+1]=1;
			matriceOut[nH-2][j+1]=1;
		}
		if(j>0){
			matriceOut[nH-1][j-1]=1;
			matriceOut[nH-2][j-1]=1;
		}
		matriceOut[nH-2][j]=1;
	   }
	}


	for (int i=0; i < nH; i++)//j=0 et j=nW-1
	{
        
	  if(matIn[i][0]==1){//j=0
        if(i<nH-1){
	   		matriceOut[i][0]=1;
	   		matriceOut[i][1]=1;
	    }
	    if(i>0){
	   		matriceOut[(i-1)][0]=1;
	   		matriceOut[(i-1)][1]=1;
	    }
	   	matriceOut[i][1]=1;
	   }
       
       if(matIn[(i)][nW-1]==1){//j=nW-1
       

        if(i<nH-1){
            matriceOut[(i+1)][nW-1]=1;
            matriceOut[(i+1)][nW-2]=1;
               
	    }
	    if(i>0){
            matriceOut[(i-1)][nW-1]=1;
            matriceOut[(i-1)][nW-2]=1;
	    }
	   	matriceOut[i][nW-2]=1;
	   }
       
    }
   for (int i=1; i < nH-1; i++)
     for (int j=1; j < nW-1; j++)
     {
       if(matIn[i][j]==1){
       	matriceOut[i][j+1]=1;
       	matriceOut[i][j-1]=1;
       	matriceOut[(i-1)][j]=1;
       	matriceOut[(i-1)][j+1]=1;
       	matriceOut[(i-1)][j-1]=1;
       	matriceOut[(i+1)][j]=1;
       	matriceOut[(i+1)][j+1]=1;
       	matriceOut[(i+1)][j-1]=1;
       }
     }
    return matriceOut;
}


QVector<QVector<int>> TerrainGen::erosion(QVector<QVector<int>> matIn,int nH, int nW){
    QVector<QVector<int>> matriceOut=matIn;

	for (int j=0; j < nW; j++)//i=0 et i=nH-1
	{
	  if(matIn[0][j]==0){//i=0
        if(j<nW-1){
			matriceOut[0][j+1]=0;//i=0
            matriceOut[1][j+1]=0;
	    }
	    if(j>0){
	   		matriceOut[1][j-1]=0;
	   		matriceOut[0][j-1]=0;
	    }
	   	matriceOut[1][j]=0;//i=0
	   }

	   if(matIn[nH-1][j]==0){//i=nh-1

        if(j<nW-1){
	   		matriceOut[nH-1][j+1]=0;
			matriceOut[nH-2][j+1]=0;
		}
		if(j>0){
			matriceOut[nH-1][j-1]=0;
			matriceOut[nH-2][j-1]=0;
		}
		matriceOut[nH-2][j]=0;
	   }
	}


	for (int i=0; i < nH; i++)//j=0 et j=nW-1
	{
	  if(matIn[i][0]==0){//j=0
	  	if(i<nH){
	   		matriceOut[i][0]=0;
	   		matriceOut[i][1]=0;
	    }
	    if(i>0){
	   		matriceOut[(i-1)][0]=0;
	   		matriceOut[(i-1)][1]=0;
	    }
	   	matriceOut[i][1]=0;
       }

       if(matIn[(i)][nW-1]==0){//j=nW-1

        if(i<nH-1){
            matriceOut[(i+1)][nW-2]=0;
            matriceOut[(i+1)][nW-1]=0;
               
	    }
	    if(i>0){
            matriceOut[(i-1)][nW-2]=0;
	   		matriceOut[(i-1)][nW-1]=0;
	    }
	   	matriceOut[i][nW-2]=0;
       }
	}

   for (int i=1; i < nH-1; i++)
     for (int j=1; j < nW-1; j++)
     {
       if(matIn[i][j]==0){
       	matriceOut[i][j+1]=0;
       	matriceOut[i][j-1]=0;
       	matriceOut[(i-1)][j]=0;
       	matriceOut[(i-1)][j+1]=0;
       	matriceOut[(i-1)][j-1]=0;
       	matriceOut[(i+1)][j]=0;
       	matriceOut[(i+1)][j+1]=0;
       	matriceOut[(i+1)][j-1]=0;
       }
     }
    return matriceOut;
}

class Point{
public:
    int i,j;
    Point(int i,int j){
        this->i=i;
        this->j=j;
    }
};

void TerrainGen::remplissage4(QVector<QVector<float>> &matrice,int i,int j,int nH,int nW){
    if(i<0 || j<0 || i>=nH || j>=nW)
        return;
    if(matrice[i][j] == 1){
        matrice[i][j] =0.5;
    }
    else{
        return;
    }
    QQueue<Point> Q;
    Q.push_front(Point(i,j));
    qInfo("%d %d %f",i,j,matrice[i][j]);

    while(!Q.isEmpty()){
        Point p=Q.dequeue();
        //qInfo("%d %d ",p.i,p.j);
        if(p.i+1<nH && matrice[p.i+1][p.j]==1){
            matrice[p.i+1][p.j]=0.5;
            Q.push_front(Point(p.i+1,p.j));
        }
        if(p.i-1>0 && matrice[p.i-1][p.j]==1){
            matrice[p.i-1][p.j]=0.5;
            Q.push_front(Point(p.i-1,p.j));
        }
        if(p.j-1>0 && matrice[p.i][p.j-1]==1){
            matrice[p.i][p.j-1]=0.5;
            Q.push_front(Point(p.i,p.j-1));
        }
        if(p.j+1<nW && matrice[p.i][p.j+1]==1){
            matrice[p.i][p.j+1]=0.5;
            Q.push_front(Point(p.i,p.j+1));
        }
    }
}

QVector<QVector<int>> TerrainGen::generateTerrain(int cols,int rows,unsigned int seed){

    qInfo("debut generation");
    QVector<QVector<int>> terrain;
    QVector<QVector<float>> terrainFloat;

    PerlinNoise Pnoise=PerlinNoise(seed);
    double xoff=0.05;
    double yoff=0.05;
    double min=10;
    double max=-10;

    qInfo("debut turbulance");
    for(int i=0;i<rows;i++){
        QVector<float> col;
        for(int j=0;j<cols;j++){
            double x=(double)(i)*xoff;
            double y=(double)(j)*yoff;
            double n=(turbulence(x,y,1.0,cols,Pnoise)+1)/2.0;

            if(n>max)
                max=n;
            if(n<min)
                min=n;

            col.push_back(n);
        }
        terrainFloat.push_back(col);
    }
    qInfo("%f %f",min,max);
    qInfo("fin turbulance");

    qInfo("debut binarisation");
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            terrainFloat[i][j]=(terrainFloat[i][j]+min)/(max+min);
            //qInfo("%d %d %f",i,j,terrainFloat[i][j]);
            if(terrainFloat[i][j]<0.50){
                terrainFloat[i][j]=0;
            }
            else{
                terrainFloat[i][j]=1;
            }
        }
    }

    qInfo("fin binarisation");



    qInfo("debut flood");
    for(int i=0;i<rows;i++){
        qInfo("%d/%d %d flood",i,cols,rows);
        remplissage4(terrainFloat,i,cols-25,rows,cols);

        remplissage4(terrainFloat,i,cols-50,rows,cols);

        remplissage4(terrainFloat,i,cols-75,rows,cols);
        
    }
    qInfo("fin flood");


    qInfo("debut clean");

    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(terrainFloat[i][j]==1)
                terrainFloat[i][j]=0;
            if(terrainFloat[i][j]==0.5)
                terrainFloat[i][j]=1;
        }
    }
    qInfo("fin clean");
    qInfo("debut conversion int");

    terrain=conversion(terrainFloat);

    QImage imagen(rows,cols, QImage::Format_RGB888);
        QRgb value;
        for(int i=0;i<rows;++i){
            for(int j=0;j<cols;++j){
                value=qRgb(terrain[i][j]*125,terrain[i][j]*125,terrain[i][j]*125);
                imagen.setPixel(i,j, value);
            }
        }
        imagen.save("unrafined.png");
    qInfo("fin conversion int");

    qInfo("debut conversion int");


    qInfo("debut dilatation erosion");
    terrain= dilatation(terrain,rows,cols);
    terrain= erosion(terrain,rows,cols);

    terrain= dilatation(terrain,rows,cols);
    terrain= dilatation(terrain,rows,cols);
    terrain= erosion(terrain,rows,cols);
    terrain= erosion(terrain,rows,cols);

    
    terrain= dilatation(terrain,rows,cols);
    terrain= dilatation(terrain,rows,cols);
    terrain= dilatation(terrain,rows,cols);
    terrain= dilatation(terrain,rows,cols);
    terrain= dilatation(terrain,rows,cols);
    terrain= erosion(terrain,rows,cols);
    qInfo("fin dilatation erosion");

    //terrain= erosion(terrain,rows,cols);
    //terrain= erosion(terrain,rows,cols);
    //terrain= erosion(terrain,rows,cols);
  //  terrain= erosion(terrain,rows,cols);
    //terrain= erosion(terrain,rows,cols);
    /*terrain= dilatation(terrain,rows,cols);
    terrain= erosion(terrain,rows,cols);
    terrain= dilatation(terrain,rows,cols);
    terrain= erosion(terrain,rows,cols);*/

    qInfo("fin generation");
    return terrain;
}

