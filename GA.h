#include "Candidate.h"
#include "Population.h"
#include <QPixmap>
#include <QTimer>
#include <QObject>

class GA : public QObject
{
    Q_OBJECT
private:
    int popSize; /// Population size
    int numPolys; /// Number of polygons
    int vertPerPoly; /// Number of vertices per polygon
    int differenceFunc; ///@NOTUSED 1-> squared difference or Euclidean difference between pixels
    float mutationChance; /// between 0 & 1
    float mutationAmout; /// between 0 & 1
    int uniformCrossOver; ///@NOTUSED
    float breedingCutOffParents;
    int killParents; ///@NOTUSED
public:
    GA();
    ~GA(){}
    int getDNALength(){ /// We store x & y for every vertex and rgba values for each polygon
        return numPolys*(4+2*vertPerPoly);
    }
    int getDNAcrossedLength(){
        return (4+2*vertPerPoly);
    }
    int getPolyPts(){
        return vertPerPoly;
    }

private:
    Population *myPop;
    int genCount; /// generations counter
    QPixmap *drawing, *original;
    QTimer timer;

public:
    void start(QPixmap *dpix, QPixmap *opix);
    int getNumPolys(){return numPolys;}
    int getVertPerPoly(){return vertPerPoly;}

public slots:
    void gaUpdate();
signals:
    void new_drawing_created(QPixmap *newPix);
};
