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

/*Gene::Gene(Gene&& cOriginal) noexcept
{
    bCalculated = cOriginal.bCalculated;
    dFitness = cOriginal.dFitness;
    vSolution = cOriginal.vSolution;
    cOriginal.vSolution = nullptr;
}
*/
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

void CIndividual::vCrossover(CSmartPtr<CIndividual> pcParent1, CSmartPtr<CIndividual> pcParent2, CSmartPtr<CIndividual> pcChild1, CSmartPtr<CIndividual> pcChild2)
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
    int i = 0;
    int iMuts = 0;
    for (int i = 0; i < vSolution->size(); i++) {
        if (MyMath::dRand() < (D_MUTATION_CHANCE + cOpt->dGenePenalty)) {
            int newVal = MyMath::dRand() * cEv->iGetNumberOfValues(i);
            vSolution->at(i) = newVal;
        }
    }
}

