#pragma once
#include "Optimizer.h"
#include "Timer.h"
#include <iostream>

using namespace TimeCounters;

class CStopper {
protected:
	int iBound;
public: 
	CStopper(int iBound) {this->iBound = iBound;}
	virtual void vRun(COptimizer* cOptimizer) const = 0;
};

class CStopAfterTime : public CStopper {
public:
	CStopAfterTime(int iBound) : CStopper(iBound) {}
	inline virtual void vRun(COptimizer* cOptimizer)const override{
		CTimeCounter cCounter;
		double* dPassed = new double (0.0);
		cCounter.vSetStartNow();
		cCounter.bGetTimePassed(dPassed);
		int iNumIters = 0;
		while (double(*dPassed) <= iBound) {
			iNumIters++;
			cOptimizer->vRunIteration();
			cCounter.bGetTimePassed(dPassed);
			std::cout << "GEN " << iNumIters << "\t| TIME " << *dPassed << "\t| " << cOptimizer->dGetBestFitness() << endl;
		}
		delete dPassed;
	}
	inline void vWait(COptimizer* cOptimizer) const{
		CTimeCounter cCounter;
		double* dPassed = new double(0.0);
		cCounter.vSetStartNow();
		cCounter.bGetTimePassed(dPassed);
		int iNumIters = 0;
		while (int(*dPassed) <= iBound) {
			iNumIters++;
			cOptimizer->vRunIteration();
			if (iNumIters % 10000 == 0)
				std::cout << "TIME " << *dPassed << "\t| " << cOptimizer->dGetBestFitness() << endl;

			cCounter.bGetTimePassed(dPassed);
		}
		delete dPassed;
	}
};

class CStopAfterIters : public CStopper {
public:
	CStopAfterIters(int iBound) : CStopper(iBound) {}
	inline virtual void vRun(COptimizer* cOptimizer)const  override {
		int iNumIters = 0;
		while (iNumIters < iBound) {
			cOptimizer->vRunIteration();
			iNumIters++;
		}
	}
};

class CStopAfterEvals : public CStopper {
public:
	CStopAfterEvals(int iBound) : CStopper(iBound) {}
	inline virtual void vRun(COptimizer* cOptimizer)const override {
		int iNumEvals = 0;
		while (iNumEvals < iBound / I_POP_SIZE) {
			cOptimizer->vRunIteration();
			iNumEvals++;
		}
	}
};