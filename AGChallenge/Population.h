#pragma once
#include <vector>
#include "settings.h"
#include <mutex>
#include "Evaluator.h"

class CIndividual;
class COptimizer;

class CSubPopulation
{
private:
	vector<CIndividual*>* vpcIndividuals;
	vector<CLFLnetEvaluator*> vpcEvaluators;
	CLFLnetEvaluator* cEv;
	COptimizer* cOpt;

public:
	inline CSubPopulation(COptimizer* cOpt, CLFLnetEvaluator& ev) {
		cEv = new CLFLnetEvaluator();
		cEv->bConfigure(ev.sGetNetName());
		this->cOpt = cOpt;
	}
	~CSubPopulation();

	std::vector<int> vGetBest();
	double dGetBestValue();
	std::vector<CIndividual*>* pvpcGetTopGenes(int iNum);

	void vInit();
	void vEvalSortIndividuals();
	void vCrossMutate();
	void vMigrateInto(std::vector<CIndividual*>* vGenesToMigrate);

private:
	int iGetParentsId();
	
};

class CPopulation 
{
public: 
	inline CPopulation(COptimizer* cOpt, CLFLnetEvaluator& ev) {
		for (int i = 0; i <= I_SUB_POPS; i++) {
			vpcSubPopulations.push_back(new CSubPopulation(cOpt, ev));
		}
	}

	~CPopulation();

	void vInit();

	std::vector<int> vGetBest();
	double dGetBestValue();
	void vEvalSortIndividuals();
	void vEvalSortAll();
	
	void vCrossMutate();
	void vExchangeBestGenes();
	void vGenBestFromHelper();

	

private:
	//CSubPopulation* pcHelperPop;
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
