#pragma once
#include <vector>
#include <QPixmap>
class Candidate{
    static int dnaLength;
    static int dnaCrossedLength;
    static int polyPts;
    static int numPolys;
private:
    int fromGeneration; /// starts at 0
    std::vector < float > dnaValues;
    double fitness;
    void generateRandomDNA(int gen);
    QPixmap *original, *drawing;
    QPixmap *test;
    double getSimilarityFitness(QPixmap *t,QPixmap *o );
public:
    static void setDNALength(int l){dnaLength=l;}
    static void setDNACrossedLength(int l){dnaCrossedLength=l;}
    static void setPolyPts(int l){polyPts=l;}
    static void setNumPolys(int l){numPolys=l;}
public:
    /// 2 constructor functions because candidates can be created in 2 ways
    Candidate(QPixmap *o, QPixmap *d, int gen); /// initial candidates for population which are randomly created
    Candidate(QPixmap *o, QPixmap *d, std::vector < float > parent1Dna, std::vector < float > parent2Dna, float mutateChance, float mutateAmount, int gen); /// candidates created by breeding (crossover)
    static int getDNALength(){return dnaLength;}
    static int getDNACrossedLength(){return dnaCrossedLength;}
    int getDnaValuesSize(){return dnaValues.size();}
    double getFitness(){return fitness;}
    int getGen(){return fromGeneration;}
    void calcFitness();
    std::vector <float> getDnaValues(){return dnaValues;}
    void drawAllPolys(QPixmap *t);

};
