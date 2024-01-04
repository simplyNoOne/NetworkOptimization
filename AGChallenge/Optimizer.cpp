#include "Optimizer.h"

#include <cfloat>
#include <iostream>
#include <windows.h>
#include "Stoper.h"
#include "Gene.h"

using namespace std;

COptimizer::COptimizer(CLFLnetEvaluator &cEvaluator)
	: c_evaluator(cEvaluator)
{
	random_device c_seed_generator;

	dBestFitness = 0;
}//COptimizer::COptimizer(CEvaluator &cEvaluator)

void COptimizer::vInitialize()
{
	dBestFitness = -DBL_MAX;
	v_current_best.clear();
	if (B_ASYNC) {
		pcPopulation = new CPopulation(c_evaluator, I_POP_SIZE, I_SUB_POPS);
	}
	else {
		pcPopulation = new CPopulation(c_evaluator, I_POP_SIZE);
	}
	pcPopulation->vInit();
	if (B_ASYNC) {
		//vRunAsync(nullptr);
	}
	
		
}//void COptimizer::vInitialize()

void COptimizer::vRunIteration()
{
	if (!B_ASYNC) {
		pcPopulation->vEvalSortGenes();
		double dIterFitness = pcPopulation->dGetBestValue();
		if (dIterFitness > dBestFitness) {
			dBestFitness = dIterFitness;
			v_current_best = pcPopulation->vGetBest();
		}
		//cout << dBestFitness << endl;
		pcPopulation->vCrossMutate();
	}
	else {
		dBestFitness = pcPopulation->dGetBestValue();
		v_current_best = pcPopulation->vGetBest();
		//cout << dBestFitness << endl;
	}
	
}

void COptimizer::vRunAsync(const CStopAfterTime* pcStopper)
{
	pcPopulation->vLaunchAsync();
	if (pcStopper) {

		cout << "sdlfkj";
		pcStopper->vWait(this);
		cout << "AAAAAAAAAAAAAA";
		pcPopulation->vStopAsync();
		v_current_best = pcPopulation->vGetBest();
		dBestFitness = pcPopulation->dGetBestValue();
	}
}

void COptimizer::vRunUntil(const CStopper* cStopper)
{
	cStopper->vRun(this);
}
//void COptimizer::vRunIteration()


