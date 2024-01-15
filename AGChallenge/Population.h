#pragma once
#include <vector>
#include "settings.h"
#include <mutex>
#include "Evaluator.h"

class CIndividual;

class CSubPopulation
{
private:
	vector<CIndividual*>* vpcGenes;
	vector<CLFLnetEvaluator*> vpcEvaluators;
	CLFLnetEvaluator* cEv;

public:
	inline CSubPopulation(CLFLnetEvaluator& ev, int iGenes) {
		cEv = new CLFLnetEvaluator();
		cEv->bConfigure(ev.sGetNetName());
	}
	~CSubPopulation();

	std::vector<int> vGetBest();
	double dGetBestValue();
	std::vector<CIndividual*>* pvpcGetTopGenes(int iNum);

	void vInit();
	void vEvalSortGenes();
	void vCrossMutate();
	void vMigrateInto(std::vector<CIndividual*>* vGenesToMigrate);

private:
	int iGetParentsId();
	
};

class CPopulation 
{
public: 
	inline CPopulation(CLFLnetEvaluator& ev, int iGenes, int iNumSubs ) {
		for (int i = 0; i < iNumSubs; i++) {
			vpcSubPopulations.push_back(new CSubPopulation(ev, iGenes));
		}
	}

	~CPopulation();

	void vInit();

	std::vector<int> vGetBest();
	double dGetBestValue();
	void vEvalSortGenes();
	
	void vCrossMutate();
	void vExchangeBestGenes();

private:
	std::vector<CSubPopulation*> vpcSubPopulations;
	vector<vector<CIndividual*>*> vpcBestGenes;
	vector<thread*> vptThreads;

	mutex mMutex;
	vector<int> vCurrentBest;
	double dBestFitness;

	int iIterCount;
	bool bKeepRunning;
	bool bAsyncStopped;
};
