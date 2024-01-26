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
	vector<CLFLnetEvaluator*>* pvpcEvaluators;
	CLFLnetEvaluator* cEv;
	COptimizer* cOpt;
	int iId;
	CIndividual* pcDaBest;

public:
	inline CSubPopulation(COptimizer* cOpt, CLFLnetEvaluator& ev, vector<CLFLnetEvaluator*>* pvpcEvals, int id) {
		cEv = new CLFLnetEvaluator();
		cEv->bConfigure(ev.sGetNetName());
		this->cOpt = cOpt;
		iId = id;
		pvpcEvaluators = pvpcEvals;
		pcDaBest = nullptr;
	}
	~CSubPopulation();

	std::vector<int> vGetBest();
	double dGetBestValue();
	std::vector<CIndividual*>* pvpcGetTopGenes(int iNum);

	void vInit();
	void vEvalSortIndividuals();
	void vCrossMutate();
	void vMigrateInto(std::vector<CIndividual*>* vGenesToMigrate, int iNum);
	void vDoChaos(int iIndivPos, CLFLnetEvaluator* pcEvToUse);

private:
	int iGetParentsId1();
	int iGetParentsId2();
	
};

class CPopulation 
{
public: 
	inline CPopulation(COptimizer* cOpt, CLFLnetEvaluator& ev) {
		pcEv = &ev;
		pcOpt = cOpt;
		pvpcEvaluators = new vector<CLFLnetEvaluator*>();
		for (int i = 0; i < I_SUB_POPS + I_HELPERS; i++) {
			vpcSubPopulations.push_back(new CSubPopulation(cOpt, ev, pvpcEvaluators, i));
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
	void vUnleashChaos();
	vector<int> vGetRandomRange();

	

private:
	//CSubPopulation* pcHelperPop;
	vector<CLFLnetEvaluator*>* pvpcEvaluators;
	std::vector<CSubPopulation*> vpcSubPopulations;
	vector<vector<CIndividual*>*> vpcBestGenes;
	CLFLnetEvaluator* pcEv;
	COptimizer* pcOpt;

	vector<int> vCurrentBest;
	double dBestFitness;

	int iIterCount;
	bool bKeepRunning;
	bool bAsyncStopped;
};
