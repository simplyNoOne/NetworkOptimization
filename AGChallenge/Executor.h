#pragma once
#include "Optimizer.h"
#include "Timer.h"

using namespace TimeCounters;

class CExecutor {
protected:
	int iBound;
public: 
	CExecutor(int iBound) {this->iBound = iBound;}
	virtual void vRun(COptimizer* cOptimizer) const = 0;
};

class CStopAfterTime : public CExecutor {
public:
	CStopAfterTime(int iBound) : CExecutor(iBound) {}
	inline virtual void vRun(COptimizer* cOptimizer)const override{
		CTimeCounter cCounter;
		double dPassed = 0.0;
		cCounter.vSetStartNow();
		cCounter.bGetTimePassed(&dPassed);
		int iNumIters = 0;
		while (dPassed <= iBound) {
			iNumIters++;
			cOptimizer->vRunIteration();
			cCounter.bGetTimePassed(&dPassed);
			//std::cout << "GEN " << iNumIters << "\t| TIME " << dPassed << "\t| " << cOptimizer->dGetBestFitness() << "\tSIZE "<< cOptimizer->iCurrentPopSize << "\tPENS " << cOptimizer->dParentPenalty <<", " << cOptimizer->dGenePenalty << ", " << cOptimizer->dCrossPenalty << endl;
		}
	}
};

class CStopAfterIters : public CExecutor {
public:
	CStopAfterIters(int iBound) : CExecutor(iBound) {}
	inline virtual void vRun(COptimizer* cOptimizer)const  override {
		int iNumIters = 0;
		while (iNumIters < iBound) {
			cOptimizer->vRunIteration();
			iNumIters++;
		}
	}
};

