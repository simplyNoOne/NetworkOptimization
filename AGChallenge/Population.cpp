#include "Population.h"
#include <algorithm>
#include <future>
#include <execution>
#include <omp.h>
#include "Individual.h"
#include "Evaluator.h"




CSubPopulation::~CSubPopulation()
{
	for (auto g : *vpcGenes)
		delete g;
	delete vpcGenes;
	for (auto e : vpcEvaluators)
		delete e;
	delete cEv;
}

void CSubPopulation::vEvalSortGenes()
{
#pragma omp parallel for
	for (int i = 0; i < I_POP_SIZE; ++i) {
		(*vpcGenes)[i]->vEvaluateFitness(vpcEvaluators[i]);
	}

	std::sort(vpcGenes->begin(), vpcGenes->end(), [](CIndividual* a, CIndividual* b) { return a->dGetFitness() > b->dGetFitness(); });
}

void CSubPopulation::vInit()
{
	vpcGenes = new std::vector<CIndividual*>(I_POP_SIZE);
	for (int i = 0; i < I_POP_SIZE; i++) {
		vpcGenes->at(i) = new CIndividual(cEv);
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
	return vpcGenes->at(0)->vGet();
}

double CSubPopulation::dGetBestValue()
{
	return vpcGenes->at(0)->dGetFitness();
}

int CSubPopulation::iGetParentsId() {
	int iGrpId = I_PARENTS_SUBGRPS - 1;
	int iShift = I_OPTIONS * MyMath::dRand();
	for (int i = 0, j = 2; i < iShift; i += j, j++) {
		iGrpId--;
	}
	int iGrpPos = (I_SUB_GRP_SIZE * MyMath::dRand()) / I_BIAS;
	return iGrpId * I_SUB_GRP_SIZE + iGrpPos;
}

void CSubPopulation::vCrossMutate()
{
	
	int iNewPopSize = 0;
	
	std::vector<CIndividual*>* pvpcNewPop = new std::vector<CIndividual*>();
	while (iNewPopSize < I_POP_SIZE) {
		
		
		CIndividual* pcP1 = vpcGenes->at(iGetParentsId());
		
		CIndividual* pcP2 = vpcGenes->at(iGetParentsId());
		if (MyMath::dRand() < D_CROSSOVER_CHANCE + D_PENALTY) {
			CIndividual* pcC1 = new CIndividual();
			CIndividual* pcC2 = new CIndividual();
			CIndividual::vCrossover(pcP1, pcP2, pcC1, pcC2);
			
			pcC1->vMutate(cEv);
			pcC2->vMutate(cEv);
			
			pvpcNewPop->push_back((pcC1));
			pvpcNewPop->push_back((pcC2));
		}
		else {
			if (MyMath::dRand() < (D_PARENT_MUTATION + D_PENALTY)) {
				pcP1->vMutate(cEv);
				pcP2->vMutate(cEv);
			}
			pvpcNewPop->push_back(new CIndividual(pcP1));
			pvpcNewPop->push_back(new CIndividual(pcP2));
		}
		iNewPopSize += 2;
	}
	for (int i = 0; i < I_POP_SIZE; i++) {
		delete vpcGenes->at(i);
	}
	delete vpcGenes;
	vpcGenes = pvpcNewPop;
}

std::vector<CIndividual*>* CSubPopulation::pvpcGetTopGenes(int iNum)
{
	std::vector<CIndividual*>* vToRet = new std::vector<CIndividual*>();
	for (int i = 0; i < iNum; i++) {
		vToRet->push_back(new CIndividual(vpcGenes->at(i)));
	}
	return vToRet;
}

void CSubPopulation::vMigrateInto(std::vector<CIndividual*>* vGenesToMigrate)
{
	int iLocToMigrate = MyMath::dRand() * (I_POP_SIZE / 2 - I_BEST_TO_MIGRATE - 1);
#pragma omp parallel for
	for (int i = 0; i < I_BEST_TO_MIGRATE; i++) {
		delete vpcGenes->at(I_POP_SIZE / 2 + iLocToMigrate + i);
		vpcGenes->at(I_POP_SIZE / 2 + iLocToMigrate + i) = new CIndividual(vGenesToMigrate->at(i));
	}
	delete vGenesToMigrate;
}


CPopulation::~CPopulation()
{
	for (auto s : vpcSubPopulations) {
		delete s;
	}
	for (auto vg : vpcBestGenes) {
		for (auto g : *vg)
			delete g;
		delete vg;
	}
}

void CPopulation::vInit()
{
	int index = 0;
	for (auto pSub : vpcSubPopulations) {
		pSub->vInit();
		vpcBestGenes.push_back(new vector<CIndividual*>());
		for (int j = 0; j < I_BEST_TO_MIGRATE; j++) {
			vpcBestGenes.at(index)->push_back(nullptr);
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

void CPopulation::vEvalSortGenes()
{
	for (int i = 0; i < I_SUB_POPS; i++) {
		vpcSubPopulations[i]->vEvalSortGenes();
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

	for (int i = 0; i < I_SUB_POPS; i++) {
#pragma omp parallel for
		for (int j = 0; j < I_BEST_TO_MIGRATE; j++) {
			delete vpcBestGenes[i]->at(j);
		}
		delete vpcBestGenes[i];
		vpcBestGenes[i] = vpcSubPopulations[i]->pvpcGetTopGenes(I_BEST_TO_MIGRATE);
	}


	for (int i = 0; i < I_SUB_POPS; i++) {
		vector<CIndividual*>* pvpcGenesToMigrate = new vector<CIndividual*>(I_BEST_TO_MIGRATE);
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
