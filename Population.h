#include "Candidate.h"
#include <vector>
#include <QPixmap>

class Population{
    int size;
    QPixmap *oPix, *dPix;
private:
    std::vector < Candidate > candidates;
    int getRandomPartner(int min,int max, int excl);
public:
    Population(int s, int dnaLength, int dnaCrossedLength, int numPolyVerts, int numPolys, QPixmap *original, QPixmap *drawing);
    void nextGeneration(int c, int a, float breedingCutOff, int newgenNum);
    static bool compareSort(Candidate &c1, Candidate &c2){ return c1.getFitness()>c2.getFitness();} /// DESC ORDER
    Candidate getFittest();
};
