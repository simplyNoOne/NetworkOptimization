#pragma once

#include "Evaluator.h"
#include "Population.h"
#include <random>
#include <vector>
#include <mutex>
#include <thread>
#include "settings.h"

class CGene;
class CExecutor;
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
	
	void vRunUntil(const CExecutor* cExecutor);
	void vGetNewRandParams();


	inline double dGetBestFitness() { return dBestFitness; }
	inline int iGetGens() { return iGenerations; }
	inline int iGetStagnation() { return iStagnation; }

	inline ~COptimizer() {
		delete pcPopulation;
	}

	
public:
	int iCurrentPopSize;
	double dParentPenalty;
	double dGenePenalty;
	double dCrossPenalty;
	int iSubGrpSize;
	int iPrevPopSize;

private:
	double dBestFitness;
	int iGenerations;
	int iStagnation;

	CPopulation* pcPopulation;
	

	//don't touch
	CLFLnetEvaluator& c_evaluator;
	vector<int> v_current_best;
};//class COptimizer