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
}

void COptimizer::vInitialize()
{
	bZeroPens = true;
	dBestFitness = 0;
	iGenerations = 0;
	iStagnation = 0;
	iCurrentPopSize = I_START_POP;
	iSubGrpSize = I_START_POP / I_PARENTS_SUBGRPS;
	iPrevPopSize = I_START_POP;

	dBestFitness = -DBL_MAX;
	v_current_best.clear();
	
	pcPopulation = new CPopulation(this, c_evaluator);
	
	pcPopulation->vInit();
			
}

void COptimizer::vRunIteration()
{
	iGenerations++;
	if (iGenerations % I_POP_INCR_INTERVAL == 0 && (iCurrentPopSize + I_POP_STEP) <= I_POP_SIZE) {
		iCurrentPopSize += I_POP_STEP;
	}

	pcPopulation->vEvalSortAll();
	if (I_SUB_POPS > 1 && iGenerations % I_MIGRATION_GAP == 0) {
		pcPopulation->vExchangeBestGenes();
		pcPopulation->vEvalSortIndividuals();
	}
	if (iGenerations % I_HELPER_POP_MIG == 0) {
		pcPopulation->vGenBestFromHelper();
		pcPopulation->vEvalSortIndividuals();
	}
	if (iGenerations % I_KILL_WAIT == 0) {
		pcPopulation->vExterminateClones();
		pcPopulation->vEvalSortAll();
	}
	
	double dIterFitness = pcPopulation->dGetBestValue();
	if (dIterFitness > dBestFitness) {
		dBestFitness = dIterFitness;
		v_current_best = pcPopulation->vGetBest();
		
		iStagnation = 0;
		bZeroPens = true;
	}
	else {
		iStagnation++;
		if (iStagnation % I_WAIT == 0) {
			bZeroPens = false;
			vGetNewRandParams();
		}
	}
	if ((bZeroPens && iGenerations % I_WAIT == 0) || iStagnation % I_RESET == 0) {
		iStagnation = 0;
		dParentPenalty = 0;
		dCrossPenalty = 0;
		dGenePenalty = 0;
	}

	if ((iStagnation != 0 && iStagnation % I_MIG_WAIT == 0) || (iGenerations % I_MIGRATION_GAP == 0)) {
		pcPopulation->vExchangeBestGenes();
		pcPopulation->vEvalSortIndividuals();
	}
	else if (iGenerations % I_HELPER_POP_MIG == 0) {
		pcPopulation->vGenBestFromHelper();
		pcPopulation->vEvalSortIndividuals();
	}
	else if (iGenerations % I_KILL_WAIT == 0) {
		pcPopulation->vExterminateClones();
		pcPopulation->vEvalSortAll();
	}


	
	pcPopulation->vCrossMutate();

	if (iStagnation != 0 && iStagnation % I_CHAOS_WAIT == 0) {
		pcPopulation->vUnleashChaos();
	}
	
	if (iPrevPopSize != iCurrentPopSize) {
		iSubGrpSize = iCurrentPopSize / I_PARENTS_SUBGRPS;
		iPrevPopSize += I_POP_STEP;
	}
	
}


void COptimizer::vRunUntil(const CExecutor* cExecutor)
{
	cExecutor->vRun(this);
}

void COptimizer::vGetNewRandParams()
{
	long seed = MyMath::lRand(899'999'999) + 100'000'000;
	std::default_random_engine generator(seed);
	std::normal_distribution<double> distribution1(D_PARENT_PEN_MEAN, D_PARENT_PEN_DEV);	
	dParentPenalty = distribution1(generator);
	
	/*if(MyMath::dRand() < 0.1) {
		dParentPenalty *= -1;
	}*/

	std::normal_distribution<double> distribution2(D_CROSS_PEN_MEAN, D_CROSS_PEN_DEV);
	dCrossPenalty = distribution2(generator);

	if (MyMath::dRand() < 0.2) {
		dCrossPenalty *= -1;
	}

	std::normal_distribution<double> distribution3(D_GENE_PEN_MEAN, D_GENE_PEN_DEV);
	dGenePenalty = distribution3(generator);

}


