#include "Population.h"
#include <algorithm>
#include <future>
#include <execution>
#include <omp.h>
#include "Individual.h"
#include "Evaluator.h"




CSubPopulation::~CSubPopulation()
{
	/*for (auto g : *vpcIndividuals)
		delete g;*/
	delete vpcIndividuals;
	for (auto e : vpcEvaluators)
		delete e;
	delete cEv;
}

void CSubPopulation::vEvalSortIndividuals()
{
#pragma omp parallel for
	for (int i = 0; i < cOpt->iPrevPopSize; ++i) {
		(*vpcIndividuals)[i]->vEvaluateFitness(vpcEvaluators[i]);
	}

	std::sort(vpcIndividuals->begin(), vpcIndividuals->end(), [](CSmartPtr<CIndividual> a, CSmartPtr<CIndividual> b) { return a->dGetFitness() > b->dGetFitness(); });
}

void CSubPopulation::vInit()
{
	vpcIndividuals = new std::vector<CSmartPtr<CIndividual>>(I_START_POP);
	for (int i = 0; i < I_START_POP; i++) {
		//vpcIndividuals->at(i) = CSmartPtr(new CIndividual(cOpt, cEv));
		CSmartPtr<CIndividual> newIndividual(new CIndividual(cOpt, cEv));
		vpcIndividuals->at(i) = newIndividual;
		vpcEvaluators.push_back(new CLFLnetEvaluator());
	}
	for (int i = I_START_POP; i < I_POP_SIZE; i++) {
		vpcEvaluators.push_back(new CLFLnetEvaluator());
	}

	auto sName = cEv->sGetNetName();

#pragma omp parallel for
	for (int i = 0; i < I_POP_SIZE; ++i) {
		vpcEvaluators[i]->bConfigure(sName);
	}		
}


std::vector<int> CSubPopulation::vGetBest()
{
	return vpcIndividuals->at(0)->vGet();
}

double CSubPopulation::dGetBestValue()
{
	return vpcIndividuals->at(0)->dGetFitness();
}

int CSubPopulation::iGetParentsId() {
	int iGrpId = I_PARENTS_SUBGRPS - 1;
	int iShift = I_OPTIONS * MyMath::dRand();
	for (int i = 0, j = 2; i < iShift; i += j, j++) {
		iGrpId--;
	}
	int iGrpPos = (cOpt->iSubGrpSize * MyMath::dRand()) / I_BIAS;
	return iGrpId * cOpt->iSubGrpSize + iGrpPos;
}

void CSubPopulation::vCrossMutate()
{
	
	int iNewPopSize = 0;
	
	std::vector<CSmartPtr<CIndividual>>* pvpcNewPop = new std::vector<CSmartPtr<CIndividual>>();
	while (iNewPopSize < cOpt->iCurrentPopSize) {
		
		
		CSmartPtr<CIndividual> pcP1 = vpcIndividuals->at(iGetParentsId());
		
		CSmartPtr<CIndividual> pcP2 = vpcIndividuals->at(iGetParentsId());
		if (MyMath::dRand() < D_CROSSOVER_CHANCE + cOpt->dParentPenalty) {
			CSmartPtr<CIndividual> pcC1 = new CIndividual(cOpt);
			CSmartPtr<CIndividual> pcC2 = new CIndividual(cOpt);
			CIndividual::vCrossover(pcP1, pcP2, pcC1, pcC2);
			
			pcC1->vMutate(cEv);
			pcC2->vMutate(cEv);

			pvpcNewPop->push_back(pcC1);
			pvpcNewPop->push_back(pcC2);
		}
		else {
			if (MyMath::dRand() < (D_PARENT_MUTATION + cOpt->dParentPenalty)) {
				pcP1->vMutate(cEv);
				pcP2->vMutate(cEv);
			}
			pvpcNewPop->push_back(pcP1);
			pvpcNewPop->push_back(pcP2);
		}
		iNewPopSize += 2;
	}
	/*for (int i = 0; i < cOpt->iPrevPopSize; i++) {
		delete vpcIndividuals->at(i);
	}*/
	delete vpcIndividuals;
	vpcIndividuals = pvpcNewPop;
}

std::vector< CSmartPtr<CIndividual>>* CSubPopulation::pvpcGetTopGenes(int iNum)
{
	std::vector<CSmartPtr<CIndividual>>* vToRet = new std::vector<CSmartPtr<CIndividual>>();
	for (int i = 0; i < iNum; i++) {
		vToRet->push_back(vpcIndividuals->at(i));
	}
	return vToRet;
}

void CSubPopulation::vMigrateInto(std::vector<CSmartPtr<CIndividual>>* vGenesToMigrate)
{
	int iLocToMigrate = MyMath::dRand() * (cOpt->iPrevPopSize / 2 - I_BEST_TO_MIGRATE - 1);
#pragma omp parallel for
	for (int i = 0; i < I_BEST_TO_MIGRATE; i++) {
		//delete vpcIndividuals->at(cOpt->iPrevPopSize / 2 + iLocToMigrate + i);
		vpcIndividuals->at(cOpt->iPrevPopSize / 2 + iLocToMigrate + i) = vGenesToMigrate->at(i);
	}
	delete vGenesToMigrate;
}


CPopulation::~CPopulation()
{
	for (auto s : vpcSubPopulations) {
		delete s;
	}
	for (auto vg : vpcBestGenes) {
		//for (auto g : *vg)
		//	delete g;
		delete vg;
	}
}

void CPopulation::vInit()
{
	int index = 0;
	for (auto pSub : vpcSubPopulations) {
		pSub->vInit();
		vpcBestGenes.push_back(new vector<CSmartPtr<CIndividual>>());
		for (int j = 0; j < I_BEST_TO_MIGRATE; j++) {
			vpcBestGenes.at(index)->push_back(CSmartPtr<CIndividual>(nullptr));
		}
		index++;
	}
}

std::vector<int> CPopulation::vGetBest()
{
	if (I_SUB_POPS == 1) {
		return  vpcSubPopulations[0]->vGetBest();
	}
	double dBestVal = 0.0;
	int iBestId = 0;
	for (int i = 0; i < vpcSubPopulations.size(); i++) {
		double dVal = vpcSubPopulations.at(i)->dGetBestValue();
		if (dVal > dBestVal) {
			dBestVal = dVal;
			iBestId = i;
		}
	}
	return vpcSubPopulations.at(iBestId)->vGetBest();
}

double CPopulation::dGetBestValue()
{
	if (I_SUB_POPS == 1) {
		return vpcSubPopulations[0]->dGetBestValue();
	}
	double dBestVal = 0.0;
	for (auto pSub : vpcSubPopulations) {
		double dVal = pSub->dGetBestValue();
		if (dVal > dBestVal) {
			dBestVal = dVal;
		}
	}
	return dBestVal;
}

void CPopulation::vEvalSortIndividuals()
{
	for (int i = 0; i < I_SUB_POPS; i++) {
		vpcSubPopulations[i]->vEvalSortIndividuals();
	}

}

void CPopulation::vCrossMutate()
{
#pragma omp parallel for
	for (int i = 0; i < I_SUB_POPS; i++) {
		vpcSubPopulations[i]->vCrossMutate();
	}
}

void CPopulation::vExchangeBestGenes()
{
	cout << "MIGRATIONNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN\n";
	for (int i = 0; i < I_SUB_POPS; i++) {
//#pragma omp parallel for
		/*for (int j = 0; j < I_BEST_TO_MIGRATE; j++) {
			delete vpcBestGenes[i]->at(j);
		}*/
		delete vpcBestGenes[i];
		vpcBestGenes[i] = vpcSubPopulations[i]->pvpcGetTopGenes(I_BEST_TO_MIGRATE);
	}


	for (int i = 0; i < I_SUB_POPS; i++) {
		vector<CSmartPtr<CIndividual>>* pvpcGenesToMigrate = new vector<CSmartPtr<CIndividual>>(I_BEST_TO_MIGRATE);
#pragma omp parallel for
		for (int j = 0; j < I_BEST_TO_MIGRATE; j++) {
			int iIndex = 0;
			do {
				iIndex = MyMath::dRand() * I_SUB_POPS;
			} while (iIndex == i);
			int iIndividual = MyMath::dRand() * I_BEST_TO_MIGRATE;
			pvpcGenesToMigrate->at(j) = (vpcBestGenes[iIndex]->at(iIndividual));	
		}
		vpcSubPopulations[i]->vMigrateInto(pvpcGenesToMigrate);
	
	}
}
