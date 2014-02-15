#include "GA.h"
#include <QDebug>

GA::GA(){
    popSize = 40; /// Population size
    numPolys = 120; /// Number of polygons
    vertPerPoly = 6; /// Number of vertices per polygon
    differenceFunc = 1; /// 1-> squared difference or Euclidean difference between pixels
    mutationChance = 0.024; /// between 0 & 1
    mutationAmout = 0.1; /// between 0 & 1
    uniformCrossOver = 1; /// @NOTUSED
    breedingCutOffParents = 0.25;
    killParents = 1; /// @NOTUSED

    /// Initialising random number generator
    srand(time(0));
}
void GA::start(QPixmap *dpix, QPixmap *opix){
    /// Get the right image pixmap to draw on and the left image pixmap to compare to
    drawing = dpix;
    original = opix;

    myPop = new Population(popSize, getDNALength(), getDNAcrossedLength(), getPolyPts(),getNumPolys(), opix, dpix);
    genCount = 0;
    /// Population has now been initialised

    /// Set a time which keeps calling the function update
    /// update stops timer, executes itself, then starts timer again.
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(gaUpdate()));
    timer.setInterval(10); /// every 10msec means update() is executed 100 times every second
    timer.start();
    //gaUpdate();
}

void GA::gaUpdate(){
    timer.stop();

    myPop->nextGeneration(mutationChance, mutationAmout, breedingCutOffParents, genCount+1);

    Candidate fittest = myPop->getFittest();
    if((genCount+1) % 10 == 0){
        qDebug()<<"Generation:"<<genCount+1;
        qDebug()<<"Fittest:"<<fittest.getFitness();
    }
    genCount++;

    /// DRAW FITTEST SET OF POLYGONS ON THE ACTUAL DRAWING!
    fittest.drawAllPolys(drawing);

    /// Update label my sending Pixmap via signal
    emit new_drawing_created(drawing);

    timer.start();
}
