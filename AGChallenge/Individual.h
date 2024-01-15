#pragma once
#include <vector>
#include "Optimizer.h"



class CIndividual
{

	std::vector<int>* vSolution;
	bool bCalculated;
	double dFitness;

public:
	CIndividual(CLFLnetEvaluator* cEv);
	CIndividual();
	//Gene(Gene&& cOriginal)noexcept;
	CIndividual(CIndividual* pcToCopy);
	inline ~CIndividual() {
		if (vSolution != nullptr)
		{
			delete vSolution;
		}
	}
	void vEvaluateFitness(CLFLnetEvaluator* cEvaluator);
	static void vCrossover(CIndividual* pcParent1, CIndividual* pcParent2, CIndividual* pcChild1, CIndividual* pcChild2);
	void vMutate(CLFLnetEvaluator* cEv);
	double dGetFitness() { return dFitness; }
	std::vector<int> vGet() { return *vSolution; }

};

