#include "Individual.h"
#include "Evaluator.h"
#include "MyMath.h"
#include "settings.h"

CIndividual::CIndividual(COptimizer* cOpt, CLFLnetEvaluator* cEv)
{
    this->cOpt = cOpt;
    vSolution = new std::vector<int>((size_t)cEv->iGetNumberOfBits(), 0);
    //vSolution->resize();

    bCalculated = false;
    dFitness = 0.;
    int iStart = MyMath::dRand() * I_GAP;
    for (int ii = iStart; ii < vSolution->size(); ii += I_GAP)
    {
        vSolution->at(ii) = lRand(cEv->iGetNumberOfValues(ii));
    }
}

CIndividual::CIndividual(COptimizer* cOp)
{
    cOpt = cOp;
    vSolution = new std::vector<int>();
    bCalculated = false;
    dFitness = 0.;
}


CIndividual::CIndividual(CIndividual* pcToCopy)
{
    cOpt = pcToCopy->cOpt;
    vSolution = new std::vector<int>();
    bCalculated = pcToCopy->bCalculated;
    dFitness = pcToCopy->dFitness;
    std::copy(pcToCopy->vSolution->begin(), pcToCopy->vSolution->end(), std::back_inserter(*vSolution));
}

void CIndividual::vEvaluateFitness(CLFLnetEvaluator* cEvaluator)
{
    if (!bCalculated) {
        dFitness = cEvaluator->dEvaluate(vSolution);
        bCalculated = true;
    }
}

void CIndividual::vCrossover(CIndividual* pcParent1, CIndividual* pcParent2, CIndividual* pcChild1, CIndividual* pcChild2)
{
    int iParts = MyMath::dRand() * I_MAX_CUTS_CROSS + 1;
    int iLastCut = 0;
    int iCut = 0;
    int iPrev = 0;
    int iSolSize = pcParent1->vSolution->size();
    std::vector<int>* vC1 = pcChild1->vSolution;
    std::vector<int>* vC2 = pcChild2->vSolution;
    while (iParts > 0 && iCut < iSolSize) {
        iParts--;
        iCut = iCut + MyMath::dRand() * (iSolSize - iCut);
        std::copy(pcParent1->vSolution->begin() + iPrev, pcParent1->vSolution->begin() + iCut, back_inserter(*vC1));
        std::copy(pcParent2->vSolution->begin() + iPrev, pcParent2->vSolution->begin() + iCut, back_inserter(*vC2));
        std::vector<int>* vTemp = vC1;
        vC1 = vC2;
        vC2 = vTemp;
        iPrev = iCut;
    }
    std::copy(pcParent1->vSolution->begin() + iPrev, pcParent1->vSolution->end(), back_inserter(*vC1));
    std::copy(pcParent2->vSolution->begin() + iPrev, pcParent2->vSolution->end(), back_inserter(*vC2));
}

void CIndividual::vMutate(CLFLnetEvaluator* cEv)
{
    for (int i = 0; i < vSolution->size(); i++) {
        if (MyMath::dRand() < (D_MUTATION_CHANCE + cOpt->dGenePenalty)) {
            bCalculated = false;
            int newVal = MyMath::dRand() * cEv->iGetNumberOfValues(i);
            vSolution->at(i) = newVal;
        }
    }
}

void CIndividual::vAddChaos(CLFLnetEvaluator* pcEv)
{
    bCalculated = false;
    int iNumGenesToMod = MyMath::dRand() * I_CHAOS_GENES;
    int iBound = pcEv->iGetNumberOfBits();
    for (int i = 0; i < iNumGenesToMod; i++) {
        int iPos = dRand() * iBound;
        vSolution->at(iPos) = pcEv->iGetNumberOfValues(iPos) * dRand();
    }

}

