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
	iCurrentPopSize = I_START_POP;
	iSubGrpSize = I_START_POP / I_PARENTS_SUBGRPS;
	iPrevPopSize = I_START_POP;
}

void COptimizer::vInitialize()
{
	dBestFitness = -DBL_MAX;
	v_current_best.clear();
	
	pcPopulation = new CPopulation(this, c_evaluator);
	
	pcPopulation->vInit();
			
}

void COptimizer::vRunIteration()
{
	iGenerations++;
	if (iGenerations % I_POP_INCR == 0 && (iCurrentPopSize + I_POP_STEP) <= I_POP_SIZE) {
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
	
	double dIterFitness = pcPopulation->dGetBestValue();
	if (dIterFitness > dBestFitness) {
		dBestFitness = dIterFitness;
		v_current_best = pcPopulation->vGetBest();
		dParentPenalty = 0;
		dCrossPenalty = 0;
		dGenePenalty = 0;
		iStagnation = 0;
	}
	else{
		iStagnation++;
		if (iStagnation % I_WAIT == 0) {
			/*if(dGenePenalty < D_MAX_GENE_PEN)
				dGenePenalty += D_GENE_PUSH;
			if(dParentPenalty < D_MAX_PARENT_PEN)
				dParentPenalty += D_PARENT_PUSH;
			if (dCrossPenalty < D_MAX_CROSS_PEN)
				dCrossPenalty += D_CROSS_PUSH;*/
			vGetNewRandParams();
		}
	}
	
	//cout << dBestFitness << endl;
	//bsack here
	pcPopulation->vCrossMutate();

	if (iStagnation != 0 && iStagnation % I_WAIT == 0) {
		pcPopulation->vEvalSortIndividuals();
		if (iStagnation % I_MIG_WAIT == 0 ) {
			pcPopulation->vExchangeBestGenes();
		}
		/*if (iStagnation % I_HELP_WAIT == 0) {
			pcPopulation->vGenBestFromHelper();
		}*/
		if (iStagnation % ( I_CHAOS_WAIT) == 0) {
			pcPopulation->vUnleashChaos();
		}

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
	
	if(MyMath::dRand() > 0.5) {
		dParentPenalty *= -1;
	}

	std::normal_distribution<double> distribution2(D_CROSS_PEN_MEAN, D_CROSS_PEN_DEV);
	dCrossPenalty = distribution2(generator);

	if (MyMath::dRand() > 0.5) {
		dCrossPenalty *= -1;
	}

	std::normal_distribution<double> distribution3(D_GENE_PEN_MEAN, D_GENE_PEN_DEV);
	dGenePenalty = distribution3(generator);

	
}


