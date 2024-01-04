#include "Population.h"
#include <algorithm>
#include "Gene.h"
#include "Evaluator.h"

void CSubPopulation::vEvalSortGenes()
{
	for (int i = 0; i < vpcGenes->size(); i++) {
		vpcGenes->at(i)->vEvaluateFitness(cEv);
	}
	/*vector<thread*> vtEvalThreads(vpcGenes->size(), nullptr);
	for (int i = 0; i < vpcGenes->size(); i++) {
		vtEvalThreads[i] = new thread([&, i]() {
			vpcGenes->at(i)->vEvaluateFitness(cEv);
			});
	}
	for (int i = 0; i < vpcGenes->size(); i++) {
		vtEvalThreads[i]->join();
	}
	for (int i = 0; i < vpcGenes->size(); i++) {
		delete vtEvalThreads[i];
	}*/
	std::sort(vpcGenes->begin(), vpcGenes->end(), [](CGene* a, CGene* b) { return a->dGetFitness() > b->dGetFitness(); });
}

void CSubPopulation::vInit()
{
	vpcGenes = new std::vector<CGene*>();
	for (int i = 0; i < I_POP_SIZE; i++) {
		CGene* p = new CGene(cEv);
		vpcGenes->push_back(p);
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

int iGetParentsId() {
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
	
	std::vector<CGene*>* pvpcNewPop = new std::vector<CGene*>();
	while (iNewPopSize < I_POP_SIZE) {
		
		
		CGene* pcP1 = vpcGenes->at(iGetParentsId());
		
		CGene* pcP2 = vpcGenes->at(iGetParentsId());
		if (MyMath::dRand() < dCrossoverChance) {
			CGene* pcC1 = new CGene();
			CGene* pcC2 = new CGene();
			CGene::vCrossover(pcP1, pcP2, pcC1, pcC2);
			
			pcC1->vMutate(cEv);
			pcC2->vMutate(cEv);
			
			pvpcNewPop->push_back((pcC1));
			pvpcNewPop->push_back((pcC2));
		}
		else {
			pvpcNewPop->push_back(new CGene(pcP1));
			pvpcNewPop->push_back(new CGene(pcP2));
		}
		iNewPopSize += 2;
	}
	for (int i = 0; i < I_POP_SIZE; i++) {
		delete vpcGenes->at(i);
	}
	delete vpcGenes;
	vpcGenes = pvpcNewPop;
}

std::vector<CGene*>* CSubPopulation::pvpcGetTopGenes(int iNum)
{
	std::vector<CGene*>* vToRet = new std::vector<CGene*>();
	for (int i = 0; i < iNum; i++) {
		vToRet->push_back(new CGene(vpcGenes->at(i)));
	}
	return vToRet;
}

void CSubPopulation::vMigrateInto(std::vector<CGene*>* vGenesToMigrate)
{
	int iSize = vGenesToMigrate->size();
	int iLocToMigrate = MyMath::dRand() * (I_POP_SIZE - iSize - 1);
	for (int i = 0; i < iSize; i++) {
		delete vpcGenes->at(iLocToMigrate + i);
		vpcGenes->at(iLocToMigrate + i) = vGenesToMigrate->at(i);
	}
	delete vGenesToMigrate;
}

void CSubPopulation::vRunSubPop(const bool& bKeepRunning, int iSubId, vector<vector<CGene*>>& vpcBestGenes, vector<int>& vCurrentBest, double& dBestFitness, mutex& mMutex)
{
	int iGenerations = 0;
	cout << iSubId << endl;
	while (bKeepRunning) {
		iGenerations++;
		if (iGenerations % iMigrationGap == 0) {
			vector<CGene*>* pvpcGenesToMigrate = new vector<CGene*>();
			for (int i = 0; i < 2; i++) {
				int index = 0;
				do {
					index = MyMath::dRand() * I_SUB_POPS;
				} while (index == iSubId);
				for (int j = 0; j < iBestToMigrate; j++) {
					if (vpcBestGenes.at(i).at(j) != nullptr) {
						pvpcGenesToMigrate->push_back(new CGene(vpcBestGenes.at(i).at(j)));
					}
				}
			}
			vMigrateInto(pvpcGenesToMigrate);
		}
		vEvalSortGenes();
		double d_Iter_Fitness = dGetBestValue();
		mMutex.lock();
		if (d_Iter_Fitness > dBestFitness) {
			dBestFitness = d_Iter_Fitness;
			vCurrentBest = vGetBest();
		}
		mMutex.unlock();
		if (iGenerations % iMigrationGap == 0) {
		
			vector<CGene*>* pvpvBestGenes = pvpcGetTopGenes(iBestToMigrate);
			for (int i = 0; i < iBestToMigrate; i++) {
				delete vpcBestGenes.at(iSubId).at(i);
				vpcBestGenes.at(iSubId).at(i) = pvpvBestGenes->at(i);
			}
			delete pvpvBestGenes;
		}
		vCrossMutate();
	}
}

std::vector<int> CPopulation::vGetBest()
{
	if (B_ASYNC) {
		return vCurrentBest;
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
	if (B_ASYNC) {
		return dBestFitness;
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

void CPopulation::vLaunchAsync()
{
	bAsyncStopped = false;
	bKeepRunning = true;
	int index = 0;
	for (auto pSub : vpcSubPopulations) {
		vptThreads.push_back(new thread(&CSubPopulation::vRunSubPop, pSub, ref(bKeepRunning), index, ref(vpcBestGenes), ref(vCurrentBest), ref(dBestFitness), ref(mMutex)));
		index++;
	}
}

void CPopulation::vStopAsync()
{
	bKeepRunning = false;
	bAsyncStopped = true;
	for each (thread * th in vptThreads)
	{
		th->join();
		cout << "hell yeahhh\n";
		delete th;
	}
	
}
