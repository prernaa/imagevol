#include "Population.h"
#include <QDebug>
#include <algorithm>
#include "math.h"
Population::Population(int s, int dlen, int dclen, int verts, int numPolys, QPixmap *original, QPixmap *drawing){
    size = s;
    oPix = original;
    dPix = drawing;


    /// Initialising population with candidates
    Candidate::setDNALength(dlen);
    Candidate::setDNACrossedLength(dclen);
    Candidate::setPolyPts(verts);
    Candidate::setNumPolys(numPolys);
    Candidate *newCand;
    for(int i=0; i<size; i++){
        //qDebug()<<"Population"<<i+1;
        newCand = new Candidate(oPix, dPix, 0);
        candidates.push_back(*newCand); // A new candidate is generated and pushed into the vector
        std::vector <float> newCandDna = newCand->getDnaValues();
        qDebug()<<newCand->getGen()<<" | "<<i<<" | "<<newCand->getFitness()<<" | "<<newCandDna[0]<<newCandDna[1200]<<newCandDna[1600];
    }

}
void Population::nextGeneration(int mutateChance, int mutateAmt, float breedingCutoff, int newgenNum){
    //qDebug()<<"Generation:"<<newgenNum;

    if(candidates.size()==1){
        /// Asexual reproduction
        Candidate parent = candidates[0];
        Candidate child(oPix, dPix, parent.getDnaValues(), parent.getDnaValues(), mutateChance, mutateAmt, newgenNum);
        if(child.getFitness()>parent.getFitness()){
            candidates[0] = child;
        }
    }
    else{
        //qDebug()<<"Before Sorting DNA SIZE"<<candidates[0].getDnaValues().size();
        /// Candidates are more than one in number
        /// So, they can do Sexual reproduction
        std::sort(candidates.begin(), candidates.end(), compareSort);

        /// Check if sorted
        //if(newgenNum==20 || newgenNum==30){
        /*for(int i=0; i<candidates.size(); i++){
            Candidate c = candidates[i];
            std::vector<float> c_dna = candidates[i].getDnaValues();
            qDebug()<<c.getGen()<<" | "<<i<<" | "<<c.getFitness()<<" | "<<c_dna[0]<<c_dna[1200]<<c_dna[1600];
        }*/
        //}
        /// Sorting worked!

        std::vector <Candidate> children;
        int numOfBreedingParents = floor(candidates.size()*breedingCutoff);
        int numOfChildren = ceil(1/breedingCutoff);



        for (int i=0;i<numOfBreedingParents;i++) {  /// Each breeding parent has numOfChildren number of children other parents chosen randomly
            std::vector <float> dna = candidates.at(i).getDnaValues();
            for (int j=0;j<numOfChildren;j++) {
                int rndCandidate = getRandomPartner(0,numOfBreedingParents-1,i);
                /// To avoid asexual reproduction, rndCandidate should be any value between 0 & numOfBreedingParents (excluding i)
                //qDebug()<<"Just before breeding"<<dna.size()<<candidates[rndCandidate].getDnaValues().size();
                Candidate newChild (oPix, dPix, dna, candidates[rndCandidate].getDnaValues(), mutateChance, mutateAmt, newgenNum);
                children.push_back(newChild);
            }
        }
        //qDebug()<<"reproduction complete";
//#if 0
        /// lets say parents cut off was 0.25 and population size 40
        /// 40*0.25 = 10 parents will breed and give birth to 1/0.25=4 children each
        /// So, we still have 10*4 = 40 individuals in our population!
        /// Each parent can breed with ANY of the other parents. We randomly choose which parent to breed with. They could've bred earlier too

        /// Now we kill of parents
        candidates = children;

        /// Check population size (only for verification purposes)
        while(candidates.size()>size){
            candidates.pop_back();
        }

//#endif
    } // end of else

    /// Print next generation to check if it worked
    /*for(int i=0; i<candidates.size(); i++){
        /// Will print i | GenNum | Fitness
        qDebug()<<i<<" | "<<candidates.at(i).getGen()<<" | "<<candidates.at(i).getFitness();
    }*/

    /// It Worked!! :D :D :D
}

int Population::getRandomPartner(int min,int max, int excl){
    /// random number between max and min (max & min included)
    int beforeExcld = excl-1;
    int afterExcld = excl+1;
    int flag = 0;
    if(beforeExcld<0) flag=1;
    else if(afterExcld>max) flag=2;
    int randNum; /// FORMULA: int randNum = rand()%(max-min + 1) + min;
    switch(flag){
        case 1: ///number to exclude is the 1st index i.e. min
                randNum = rand()%(max-(excl+1) + 1) + (excl+1); /// excld + 1 is index 1
                break;
        case 2: ///number to exclude is the last index i.e. max
                randNum = rand()%((excl-1)-min + 1) + min;
                break;
        default: /// number to exclude is somewhere in between
                /// generate 1 number before exclude and 1 after exclude
                int randNumBefore = rand()%((excl-1)-min + 1) + min;
                int randNumAfter = rand()%(max-(excl+1) + 1) + (excl+1);
                int randNumBinary = rand()%(2); /// generates either 0 or 1
                if(randNumBinary==0){randNum=randNumBefore;}
                else{randNum=randNumAfter;}
                break;
    }
    return randNum;
}

Candidate Population::getFittest(){
    std::sort(candidates.begin(), candidates.end(), compareSort);
    return candidates.at(0);
}
