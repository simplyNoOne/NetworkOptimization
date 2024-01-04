#pragma once
#include <vector>
#include "Optimizer.h"



class CGene
{

	std::vector<int>* vSolution;
	bool bCalculated;
	double dFitness;

public:
	CGene(CLFLnetEvaluator* cEv);
	CGene();
	//Gene(Gene&& cOriginal)noexcept;
	CGene(CGene* pcToCopy);
	inline ~CGene() {
		if (vSolution != nullptr)
		{
			delete vSolution;
		}
	}
	void vEvaluateFitness(CLFLnetEvaluator* cEvaluator);
	static void vCrossover(CGene* pcParent1, CGene* pcParent2, CGene* pcChild1, CGene* pcChild2);
	void vMutate(CLFLnetEvaluator* cEv);
	double dGetFitness() { return dFitness; }
	std::vector<int> vGet() { return *vSolution; }

};

