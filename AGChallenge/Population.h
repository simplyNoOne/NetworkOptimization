#pragma once
#include <vector>
#include "settings.h"
#include <mutex>
#include "Evaluator.h"

class CGene;

class CSubPopulation
{
private:
	vector<CGene*>* vpcGenes;
	vector<CLFLnetEvaluator*> vpcEvaluators;
	CLFLnetEvaluator* cEv;
	int iGenes;

public:
	inline CSubPopulation(CLFLnetEvaluator& ev, int iGenes) {
		cEv = new CLFLnetEvaluator();
		cEv->bConfigure(ev.sGetNetName());
		this->iGenes = iGenes;
	}
	inline ~CSubPopulation() {
		for (auto g : *vpcGenes)
			delete g;
		delete vpcGenes;
		for (auto e : vpcEvaluators)
			delete e;
		delete cEv;

	}
	void vEvalSortGenes();
	void vInit();
	std::vector<int> vGetBest();
	double dGetBestValue();
	void vCrossMutate();
	void vConfEval(CLFLnetEvaluator* cE, ATL::CString sName);
	std::vector<CGene*>* pvpcGetTopGenes(int iNum);
	void vMigrateInto(std::vector<CGene*>* vGenesToMigrate);
	void vRunSubPop(const bool& bKeepRunning, int iSubId, vector<vector<CGene*>>& vpcBestGenes, vector<int>& vCurrentBest, double& dBestFitness, mutex& mMutex);
};

class CPopulation 
{
public: 
	inline CPopulation(CLFLnetEvaluator& ev, int iGenes, int iNumSubs = 1) {
		for (int i = 0; i < iNumSubs; i++) {
			vpcSubPopulations.push_back(new CSubPopulation(ev, iGenes));
		}
	}

	~CPopulation() {
		if (B_ASYNC) {
			if (!bAsyncStopped) {
				vStopAsync();
			}
		}
		for (auto s : vpcSubPopulations) {
			delete s;
		}
		for (auto vg : vpcBestGenes) {
			for( auto g : vg)
				delete g;
		}
		
	}

	inline void vInit() {
		int index = 0;
		for (auto pSub : vpcSubPopulations) {
			pSub->vInit();
			vpcBestGenes.push_back(vector<CGene*>());
			for (int j = 0; j < I_BEST_TO_MIGRATE; j++) {
				vpcBestGenes.at(index).push_back(nullptr);
			}
			index++;
		}
	}

	std::vector<int> vGetBest();
	double dGetBestValue();
	inline void vEvalSortGenes() {
		for (auto pSub : vpcSubPopulations) {
			pSub->vEvalSortGenes();
		}
	}
	
	inline void vCrossMutate() {
		for (auto pSub : vpcSubPopulations) {
			pSub->vCrossMutate();
		}
	}
	void vLaunchAsync();
	void vStopAsync();

private:
	std::vector<CSubPopulation*> vpcSubPopulations;
	vector<vector<CGene*>> vpcBestGenes;
	vector<thread*> vptThreads;

	mutex mMutex;
	vector<int> vCurrentBest;
	double dBestFitness;

	int iIterCount;
	bool bKeepRunning;
	bool bAsyncStopped;
};
