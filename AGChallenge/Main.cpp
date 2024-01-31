#include "Evaluator.h"
#include "Optimizer.h"
#include "Timer.h"

#include <exception>
#include <iostream>
#include <random>
#include "Executor.h"

//
#include <sstream>
using namespace TimeCounters;

using namespace std;

#define dMAX_TIME 25 * 60


void vRunExperiment(CLFLnetEvaluator &cConfiguredEvaluator)
{
	try
	{
		CTimeCounter c_time_counter;

		double d_time_passed;
		
		COptimizer c_optimizer(cConfiguredEvaluator);

		c_time_counter.vSetStartNow();

		c_optimizer.vInitialize();

		c_time_counter.bGetTimePassed(&d_time_passed);

		vector<int>* sol;
		bool found = false;
		while (!found && d_time_passed <= dMAX_TIME)
		{
		
			c_optimizer.vRunIteration();
			sol = c_optimizer.pvGetCurrentBest();
			found = (c_optimizer.dGetBestFitness() == 1);
			std::cout << "GEN " << c_optimizer.iGetGens() << "\t| TIME " << d_time_passed << "\t| " << c_optimizer.dGetBestFitness() << "\t| SIZE " << c_optimizer.iCurrentPopSize << "\tPENS P: " << c_optimizer.dParentPenalty << ", G: " << c_optimizer.dGenePenalty << ", C: " << c_optimizer.dCrossPenalty << endl;


			c_time_counter.bGetTimePassed(&d_time_passed);
		}

		//c_optimizer.vRunUntil(new CStopAfterTime(I_TIME));
		cout << cConfiguredEvaluator.dEvaluate(sol)<< "( "<< d_time_passed <<")\n";
		//cout<<c_optimizer.dGetBestFitness();
	}//try
	catch (exception &c_exception)
	{
		cout << c_exception.what() << endl;
	}//catch (exception &c_exception)
}//void vRunExperiment(const CEvaluator &cConfiguredEvaluator)



void  vRunLFLExperiment(CString  sNetName)
{
	CLFLnetEvaluator c_lfl_eval;
	c_lfl_eval.bConfigure(sNetName);
	vRunExperiment(c_lfl_eval);
	
}//void vRunRastriginExperiment(int iNumberOfBits, int iBitsPerFloat, int iMaskSeed)



void main(int iArgCount, char **ppcArgValues)
{
	random_device c_mask_seed_generator;
	int i_mask_seed = (int)c_mask_seed_generator();


	CString  s_test;

	//vRunLFLExperiment("g120d00");

	//vRunLFLExperiment("114b04");
	//vRunLFLExperiment("114b00");
	vRunLFLExperiment("114b04");
	
	//vRunLFLExperiment("162096");
	//vRunLFLExperiment("g120081");

	/*vRunIsingSpinGlassExperiment(81, 0, i_mask_seed);
	vRunIsingSpinGlassExperiment(81, 0, iSEED_NO_MASK);

	vRunLeadingOnesExperiment(50, i_mask_seed);
	vRunLeadingOnesExperiment(50, iSEED_NO_MASK);

	vRunMaxSatExperiment(25, 0, 4.27f, i_mask_seed);
	vRunMaxSatExperiment(25, 0, 4.27f, iSEED_NO_MASK);

	vRunNearestNeighborNKExperiment(100, 0, 4, i_mask_seed);
	vRunNearestNeighborNKExperiment(100, 0, 4, iSEED_NO_MASK);

	vRunOneMaxExperiment(100, i_mask_seed);
	vRunOneMaxExperiment(100, iSEED_NO_MASK);

	vRunRastriginExperiment(200, 10, i_mask_seed);
	vRunRastriginExperiment(200, 10, iSEED_NO_MASK);*/
}//void main(int iArgCount, char **ppcArgValues)