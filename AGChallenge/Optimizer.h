#pragma once

#include "Evaluator.h"
#include "Population.h"
#include <random>
#include <vector>
#include <mutex>
#include <thread>
#include "settings.h"

class CGene;
class CStopper;
class CStopAfterTime;

using namespace std;

class COptimizer
{
public:
	//don't touch
	COptimizer(CLFLnetEvaluator& cEvaluator);
	vector<int>* pvGetCurrentBest() { return &v_current_best; }

	//modify
	void vInitialize();
	void vRunIteration();
	void vRunAsync(const CStopAfterTime* pcStopper);

	void vRunUntil(const CStopper* cStopper);
	double dGetBestFitness() { return dBestFitness; }

	inline ~COptimizer() {
		delete pcPopulation;
	}

private:
	//throw away
	double dBestFitness;
	

	//	Population* cPopulation;
	CPopulation* pcPopulation;
	


	//don't touch
	CLFLnetEvaluator& c_evaluator;
	vector<int> v_current_best;
};//class COptimizer