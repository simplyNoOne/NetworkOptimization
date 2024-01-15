#include "Optimizer.h"

#include <cfloat>
#include <iostream>
#include <windows.h>
#include "Executor.h"
#include "Individual.h"

using namespace std;

COptimizer::COptimizer(CLFLnetEvaluator &cEvaluator)
	: c_evaluator(cEvaluator)
{
	random_device c_seed_generator;

	dBestFitness = 0;
	iGenerations = 0;
	iStagnation = 0;
}

void COptimizer::vInitialize()
{
	dBestFitness = -DBL_MAX;
	v_current_best.clear();
	
	pcPopulation = new CPopulation(c_evaluator, I_POP_SIZE, I_SUB_POPS);
	
	pcPopulation->vInit();
			
}

void COptimizer::vRunIteration()
{
	iGenerations++;
	pcPopulation->vEvalSortGenes();
	if (I_SUB_POPS > 1 && iGenerations % I_MIGRATION_GAP == 0) {
		pcPopulation->vExchangeBestGenes();
		pcPopulation->vEvalSortGenes();
	}
	
	double dIterFitness = pcPopulation->dGetBestValue();
	if (dIterFitness > dBestFitness) {
		dBestFitness = dIterFitness;
		v_current_best = pcPopulation->vGetBest();
		D_PENALTY = 0;
		iStagnation = 0;
	}
	else{
		iStagnation++;
		if (iStagnation > I_WAIT) {
			iStagnation = 0;
			D_PENALTY += D_PUSH;
		}
	}
	
	//cout << dBestFitness << endl;
	pcPopulation->vCrossMutate();
	
}


void COptimizer::vRunUntil(const CExecutor* cExecutor)
{
	cExecutor->vRun(this);
}


