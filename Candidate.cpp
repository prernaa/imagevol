#include "Candidate.h"
#include "stdlib.h"
#include "math.h"
#include <QDebug>
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QPointF>
#include <QRgb>

int Candidate::dnaLength=0;
int Candidate::dnaCrossedLength=0;
int Candidate::polyPts=0;
int Candidate::numPolys=0;
Candidate::Candidate(QPixmap *o, QPixmap *d, int gen){
    original = o;
    drawing = d;
    generateRandomDNA(gen);


}
Candidate::Candidate(QPixmap *o, QPixmap *d, std::vector < float > parent1Dna, std::vector < float > parent2Dna, float mutateChance, float mutateAmount, int gen){
    /// constructor for breeding or crossover

    /// Saving pointers to pixmaps
    original=o;
    drawing=d;

    //qDebug()<<dnaLength<<parent1Dna.size()<<parent2Dna.size();
    if(!(dnaLength == parent1Dna.size() && dnaLength == parent1Dna.size())){generateRandomDNA(gen);}

    //qDebug()<<"Breeding Call";
    for (int i=0;i<dnaLength;i+=dnaCrossedLength) { /// For each dna crossed length, first randomly pick which parent's part to take
        std::vector < float > part;
        if (((float)rand())/RAND_MAX < 0.5) { /// we get a number which is < 0.5 or >=0.5. Probability for either case is 1/2
            part = parent1Dna;
        } else {
            part = parent2Dna;
        }
        //qDebug()<<"Parent for chromosome"<<i<<"selected";

        for (int j=0;j<dnaCrossedLength;j++) { /// Once, you have the dna crossed length part you want to use, iterate through it to mutate it.
             float val;
            val = part[i+j];
            if (((float)rand())/RAND_MAX  < mutateChance) {
                /// when we say mutate chance is 0.25 then values below 0.25 are few(25% of values between 0 & 1 are below 0.25), so those below 0.25 are mutated
                val += ((float)rand())/RAND_MAX * mutateAmount * 2 - mutateAmount;
                /// mutateAmout*(2*RAND -1) will be added to val. 2*RAND-1 can also give negative values. Mutate amt is also between 0 & 1. So, we are multiplying then to get a value between 0 & 1, but can be +ve or -ve
                /// By adding that value to val, val may become <0 or >1. hence the foll:
                if (val < 0) val = 0;
                if (val > 1) val = 1;
            }
            dnaValues.empty();
            dnaValues.push_back(val);
        }
    }
    //qDebug()<<"New Candidate Born!";
    /// New candidate is born
    /// Now, we need to calculate its fitness
    calcFitness();
    //qDebug()<<"Fitness Calculated!!";
    fromGeneration=gen;
}

void Candidate::generateRandomDNA(int gen){
/// DNA length = numPolys*(4+numPolyPts*2)
/// PolyLength = 4+numPolyPts*2
/// PolyLength and dnaCrossedLength length is the same
     for (int i=0;i<dnaLength;i+=(4+2*polyPts)) {
         // rgba
         float R = ((float)rand())/RAND_MAX;
         float G = ((float)rand())/RAND_MAX;
         float B = ((float)rand())/RAND_MAX;
         float A = ((float)rand())/RAND_MAX * ((float)rand())/RAND_MAX;
         dnaValues.push_back(R);
         dnaValues.push_back(G);
         dnaValues.push_back(B);
         if(0.2>A){A=0.2; dnaValues.push_back(A);}
         else{dnaValues.push_back(A); }
         //qDebug()<<"Color"<<R*255<<G*255<<B*255<<A*255;

         float px = ((float)rand())/RAND_MAX;
         float py = ((float)rand())/RAND_MAX;

         for (int j=0;j<polyPts;j++) {
             float rx = ((float)rand())/RAND_MAX;
             float ry = ((float)rand())/RAND_MAX;
             dnaValues.push_back(px + (rx-0.5));
             dnaValues.push_back(py + (ry-0.5));
         }
     }
     //qDebug()<<"Each candidate has a DNA of length"<<dnaLength;

     /// Once the candidate is initialised with random values, lets calculate its fitness
     calcFitness();
     fromGeneration = gen;
}
void Candidate::calcFitness(){
    /// Create test pixmap
    //qDebug()<<"Drawing!"<<drawing->isNull();
    QPixmap tst = drawing->copy();
    test = &tst;
    //qDebug()<<"Just before drawing polygons!";
    drawAllPolys(test);
    //qDebug()<<"Polygons drawn";
    //test->save("Checkingtest.png");

    /// Calculate the fitness score based on the squared distance between test pixmap and original
    fitness = getSimilarityFitness(test, original);

    /// Now, we have the fitness for this candidate
    //qDebug()<<"Fitness"<<fitness;

}
double Candidate::getSimilarityFitness(QPixmap *t,QPixmap *o ){
    QImage origimg = o->toImage();
    QImage testimg = t->toImage();
    int oH = origimg.size().height();
    int oW = origimg.size().width();
    double diff = 0.0;
    for(int i=0; i<oH; i++){
        for(int j=0; j<oW; j++ ){
            QColor color1 = QColor(origimg.pixel(j,i));
            QColor color2 = QColor(testimg.pixel(j,i));
            diff+=sqrt(pow((color1.red()-color2.red()),2) + pow((color1.blue()-color2.blue()),2) + pow((color1.green()-color2.green()),2));
        }
    }
    float divby = oH * oW * 256* 3;
    //qDebug()<<diff<<divby;
    return ( 1 - (diff/divby) ); /// smaller is the diff, larger is the fitness
}

void Candidate::drawAllPolys(QPixmap *t){
    //qDebug()<<"draw all polys";

    int x =0;
    for (int i=0;i<dnaLength;i+=numPolys) { /// For each polygon in DNA
        // rgba
        float R = dnaValues[x++]*255;
        float G = dnaValues[x++]*255;
        float B = dnaValues[x++]*255;
        float A = dnaValues[x++]*255;
        //qDebug()<<R<<G<<B<<A;

        std::vector < QPointF > pts;

        for (int j=0;j<polyPts;j++) {
            float xpt = dnaValues[x++]*t->width();
            float ypt = dnaValues[x++]*t->height();
            pts.push_back(QPointF(xpt,ypt));
            //qDebug()<<"Point"<<xpt<<ypt;
        }
        QPointF *pointerToPts = &pts[0];
        QPainter pixPaint(t);
        pixPaint.setPen(QColor(0,0,0,0)); /// Transparent outline
        QBrush brush(QColor(R,G,B,A));
        //qDebug()<<R<<G<<B<<A;
        pixPaint.setBrush(brush);
        pixPaint.drawPolygon(pointerToPts, polyPts);
    }
    //test->save("try.jpg");
}

