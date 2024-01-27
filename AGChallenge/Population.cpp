#include "Population.h"
#include <algorithm>
#include <future>
#include <execution>
#include <omp.h>
#include "Individual.h"
#include "Evaluator.h"



CSubPopulation::~CSubPopulation()
{
	for (auto g : *vpcIndividuals)
		delete g;
	delete vpcIndividuals;
	delete cEv;
	if (pcDaBest)
		delete pcDaBest;
}

void CSubPopulation::vEvalSortIndividuals()
{
#pragma omp parallel for
		for (int i = 0; i < cOpt->iPrevPopSize; ++i) {
			(*vpcIndividuals)[i]->vEvaluateFitness((*pvpcEvaluators)[i]);
		}

	std::sort(vpcIndividuals->begin(), vpcIndividuals->end(), [](CIndividual* a, CIndividual* b) { return a->dGetFitness() > b->dGetFitness(); });

	if (!pcDaBest) {
		pcDaBest = new CIndividual( vpcIndividuals->at(0));
	}
	else {
		if (vpcIndividuals->at(0)->dGetFitness() > pcDaBest->dGetFitness()) {
			delete pcDaBest;
			pcDaBest = new CIndividual(vpcIndividuals->at(0));
		}
			
	}
}

void CSubPopulation::vInit()
{
	vpcIndividuals = new std::vector<CIndividual*>(I_START_POP);
#pragma omp parallel for
	for (int i = 0; i < I_START_POP; i++) {
		vpcIndividuals->at(i) = new CIndividual(cOpt, pvpcEvaluators->at(i));
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

void CSubPopulation::vGetParentsId3(int* iPos1, int* iPos2) {
	*iPos1 = MyMath::dRand() * cOpt->iPrevPopSize;
	*iPos2 = cOpt->iPrevPopSize - *iPos1 - 1;
}

int CSubPopulation::iGetParentsId2() {
	int iPos1 = MyMath::dRand() * (cOpt->iPrevPopSize);
	int iPos2 = MyMath::dRand() * (cOpt->iPrevPopSize);
	if (vpcIndividuals->at(iPos1)->dGetFitness() > vpcIndividuals->at(iPos2)->dGetFitness()) {
		return iPos1;
	}
	return iPos2;
}

//add another parent choosing method

void CSubPopulation::vOrder99()
{
	double dPrevFit = 0.0;
	double dCurrentFit;
	int iCloneCounter = 0;
	vector<int> vMurderSpots;

	for (int i = 0; i < cOpt->iPrevPopSize; i++) {
		dCurrentFit = vpcIndividuals->at(i)->dGetFitness();
		if (dCurrentFit != dPrevFit) {
			dPrevFit = dCurrentFit;
			iCloneCounter = 0;
		}
		else {
			iCloneCounter++;
			if (iCloneCounter > I_CLONE_TH) {
				vMurderSpots.push_back(i);
			}
		}
	}
	cout << "guys to murder: " << vMurderSpots.size() << endl;
#pragma omp parallel for
	for (int i = 0; i < (vMurderSpots.size() / 2); i += 2) {
		if (i % 4 == 0) {
			vpcIndividuals->at(vMurderSpots[i])->vMutate(pvpcEvaluators->at(i));
			vpcIndividuals->at(vMurderSpots[i + 1])->vMutate(pvpcEvaluators->at(i));
		}
		CIndividual* pcC1 = new CIndividual(cOpt);
		CIndividual* pcC2 = new CIndividual(cOpt);
		CIndividual* pcParent = vpcIndividuals->at(vMurderSpots[i]);
		delete vpcIndividuals->at(vMurderSpots[i + 1]);
		CIndividual::vCrossover(pcDaBest, pcParent, pcC1, pcC2);
		delete vpcIndividuals->at(vMurderSpots[i]);
		pcC1->vMutate(pvpcEvaluators->at(i));
		pcC2->vMutate(pvpcEvaluators->at(i));
		vpcIndividuals->at(vMurderSpots[i]) = pcC1;
		vpcIndividuals->at(vMurderSpots[i + 1]) = pcC2;
	}
	if (vMurderSpots.size() % 2 == 1) {
		vpcIndividuals->at(vMurderSpots[vMurderSpots.size() - 1])->vMutate(cEv);
	}
}

int CSubPopulation::iGetParentsId1() {
	int iGrpId = I_PARENTS_SUBGRPS - 1;
	int iShift = I_OPTIONS * MyMath::dRand();
	for (int i = 0, j = 2; i < iShift; i += j, j++) {
		iGrpId--;
	}
	int iGrpPos = (cOpt->iSubGrpSize * MyMath::dRand()) / I_BIAS;
	return iGrpId * cOpt->iSubGrpSize + iGrpPos;
}

void CSubPopulation::vChooseParents(CIndividual* &pcP1, CIndividual* &pcP2) {
	do {
		
		if (cOpt->iGetGens() % I_FANCIFY == 0) {
			double dMethod = MyMath::dRand();
			if (cOpt->iGetStagnation() > I_WAIT && dMethod < D_MET3_TH) {
				int iPos1, iPos2;
				vGetParentsId3(&iPos1, &iPos2);
				pcP1 = vpcIndividuals->at(iPos1);
				pcP2 = vpcIndividuals->at(iPos2);
			}
			else if (cOpt->iGetStagnation() > 0 || dMethod < D_MET2_TH) {
				pcP1 = vpcIndividuals->at(iGetParentsId2());
				pcP2 = vpcIndividuals->at(iGetParentsId2());
			}
			else {
				pcP1 = vpcIndividuals->at(iGetParentsId1());
				pcP2 = vpcIndividuals->at(iGetParentsId1());
			}
		}
		else {
			pcP1 = vpcIndividuals->at(iGetParentsId1());
			pcP2 = vpcIndividuals->at(iGetParentsId1());
		}

	} while (pcP1->dGetFitness() == pcP2->dGetFitness());
}

void CSubPopulation::vCrossMutate()
{
	int iNewPopSize = 0;
	
	std::vector<CIndividual*>* pvpcNewPop = new std::vector<CIndividual*>();
	while (iNewPopSize < cOpt->iCurrentPopSize) {
		CIndividual* pcP1;
		CIndividual* pcP2;

		vChooseParents(pcP1, pcP2);
		
		
		if (MyMath::dRand() < iId * D_CROSS_POP_SHIFT + D_CROSSOVER_CHANCE + cOpt->dCrossPenalty) {
			CIndividual* pcC1 = new CIndividual(cOpt);
			CIndividual* pcC2 = new CIndividual(cOpt);
		

			CIndividual::vCrossover(pcP1, pcP2, pcC1, pcC2);

			pcC1->vMutate(cEv);
			pcC2->vMutate(cEv);

			pcC1->vEvaluateFitness(cEv);
			pcC2->vEvaluateFitness(cEv);

			if (pcC1->dGetFitness() < pcC2->dGetFitness()) {

				CIndividual* temp = pcC1;
				pcC1 = pcC2;
				pcC2 = temp;
			}

			
			if (pcP1->dGetFitness() > pcC2->dGetFitness()) {
				pvpcNewPop->push_back(new CIndividual(pcP1));
				pvpcNewPop->at(pvpcNewPop->size() - 1)->vMutate(cEv);
				delete pcC2;
			}
			else {
				pvpcNewPop->push_back(pcC2);
			}
			pvpcNewPop->push_back(pcC1);
		}
		else {
			pvpcNewPop->push_back(new CIndividual(pcP1));
			pvpcNewPop->push_back(new CIndividual(pcP2));
			if (MyMath::dRand() < (iId * D_PAR_POP_SHIFT + D_PARENT_MUTATION + cOpt->dParentPenalty)) {
				pvpcNewPop->at(pvpcNewPop->size() - 1)->vMutate(cEv);
				pvpcNewPop->at(pvpcNewPop->size() - 2)->vMutate(cEv);
			}
		}
		iNewPopSize += 2;
	}
	for (int i = 0; i < cOpt->iPrevPopSize; i++) {
		delete vpcIndividuals->at(i);
	}
	delete vpcIndividuals;
	vpcIndividuals = pvpcNewPop;
}

std::vector<CIndividual*>* CSubPopulation::pvpcGetTopGenes(int iNum)
{
	std::vector<CIndividual*>* vToRet = new std::vector<CIndividual*>();
	for (int i = 0; i < iNum; i++) {
		vToRet->push_back(new CIndividual(vpcIndividuals->at(i)));
	}
	return vToRet;
}

void CSubPopulation::vMigrateInto(std::vector<CIndividual*>* vGenesToMigrate, int iNum)
{
	int iLocToMigrate = MyMath::dRand() * (cOpt->iPrevPopSize / 2 - iNum - 1);
#pragma omp parallel for
	for (int i = 0; i < iNum; i++) {
		delete vpcIndividuals->at(cOpt->iPrevPopSize / 2 + iLocToMigrate + i);
		vpcIndividuals->at(cOpt->iPrevPopSize / 2 + iLocToMigrate + i) = new CIndividual(vGenesToMigrate->at(i));
	}
}

void CSubPopulation::vDoChaos(int iIndivPos, CLFLnetEvaluator* pcEvToUse)
{
	vpcIndividuals->at(iIndivPos)->vAddChaos(pcEvToUse);
	vpcIndividuals->at(iIndivPos)->vEvaluateFitness(pcEvToUse);
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
	for (auto e : *pvpcEvaluators)
		delete e;
	delete pvpcEvaluators;

}

void CPopulation::vInit()
{
	for (int i = 0; i < I_POP_SIZE; i++) {
		pvpcEvaluators->push_back(new CLFLnetEvaluator());
	}
	auto sName = pcEv->sGetNetName();

#pragma omp parallel for
	for (int i = 0; i < I_POP_SIZE; ++i) {
		pvpcEvaluators->at(i)->bConfigure(sName);
	}


	for (int index = 0; index < I_SUB_POPS; index++) {
		vpcSubPopulations[index]->vInit();
		vpcBestGenes.push_back(new vector<CIndividual*>());
		for (int j = 0; j < I_BEST_TO_MIGRATE; j++) {
			vpcBestGenes.at(index)->push_back(nullptr);
		}
	}
	for (int index = I_SUB_POPS; index < I_SUB_POPS + I_HELPERS; index++) {
		vpcSubPopulations[index]->vInit();
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

void CPopulation::vEvalSortAll()
{
	for (int i = 0; i < I_SUB_POPS + I_HELPERS; i++) {
		vpcSubPopulations[i]->vEvalSortIndividuals();
	}
	
}

void CPopulation::vCrossMutate()
{
#pragma omp parallel for
	for (int i = 0; i < I_SUB_POPS + I_HELPERS; i++) {
		vpcSubPopulations[i]->vCrossMutate();
	}
}

void CPopulation::vExchangeBestGenes()
{
	cout << "-----MIGRATION-----\n";
#pragma omp parallel for
	for (int i = 0; i < I_SUB_POPS; i++) {
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
		vpcSubPopulations[i]->vMigrateInto(pvpcGenesToMigrate, I_BEST_TO_MIGRATE);
		delete pvpcGenesToMigrate;
	}
}

void CPopulation::vGenBestFromHelper()
{
	cout << "-----BEST FROM HELPER-----\n";
	int iHelper = MyMath::dRand() * I_HELPERS;
	vector<CIndividual*>* pvpcGenesToMigrate = vpcSubPopulations[I_SUB_POPS + iHelper]->pvpcGetTopGenes(I_BEST_TO_MIGRATE/2);
	int iSubPop = MyMath::dRand() * I_SUB_POPS;
	vpcSubPopulations[iSubPop]->vMigrateInto(pvpcGenesToMigrate, I_BEST_TO_MIGRATE / 2);
	
	for (auto pcInd : *pvpcGenesToMigrate) {
		delete pcInd;
	}
	delete pvpcGenesToMigrate;
}

void CPopulation::vUnleashChaos()
{
	cout << "----------CHAOS------------\n";
	vector<int>vPos = vGetRandomRange();
	int iCount = I_CHAOS_COUNT;
	if (iCount > pcOpt->iPrevPopSize) {
		iCount = pcOpt->iPrevPopSize;
	}

#pragma omp parallel for
	for (int i = 0; i < iCount; i++) {
		int iPop = MyMath::dRand()* (I_SUB_POPS + I_HELPERS);
		vpcSubPopulations[iPop]->vDoChaos(vPos[i], pvpcEvaluators->at(i));
	}
}

vector<int> CPopulation::vGetRandomRange()
{
	int iMax = pcOpt->iPrevPopSize;
	vector<int> list(iMax);

	int iCount = I_CHAOS_COUNT;
	if (iCount > iMax) {
		iCount = iMax;
	}

	for (int i = 0; i < iMax; i++) {
		list[i] = i;
	}
	vector<int> res(iCount);
	for (int i = 0; i < iCount; i++) {
		int j = i + MyMath::dRand() * (iMax - i);
		int temp = list[i];
		list[i] = list[j];
		list[j] = temp;

		res[i] = list[i];
	}
	return res;
}

void CPopulation::vExterminateClones()
{
	cout << "-----------ORDER 99\n";
	for (int i = 0; i < I_SUB_POPS + I_HELPERS; i++) {
		vpcSubPopulations[i]->vOrder99();
	}
}
