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
	double dGetBestFitness() { return dBestFitness; }


	inline ~COptimizer() {
		delete pcPopulation;
	}

	void vGetNewRandParams();

	int iCurrentPopSize;
	double dParentPenalty;
	double dGenePenalty;
	double dCrossPenalty;
	int iSubGrpSize;
	int iPrevPopSize;

	int iGetGens() { return iGenerations; }
	int iGetStagnation() { return iStagnation; }
private:
	//throw away
	double dBestFitness;
	int iGenerations;
	int iStagnation;

	//	Population* cPopulation;
	CPopulation* pcPopulation;
	

	//don't touch
	CLFLnetEvaluator& c_evaluator;
	vector<int> v_current_best;
};//class COptimizer