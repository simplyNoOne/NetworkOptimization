#pragma once

//----------------DEBUG
const static char* S_CASE = "128b03";
const int I_TIME = 1800;

//----------------GENERAL
const double D_MUTATION_CHANCE = 0.001;
const double D_PARENT_MUTATION = 0.0079;
const double D_CROSSOVER_CHANCE = 0.71;
const int I_GAP = 3; 

//----------------SIZING
const int I_POP_SIZE = 700;
const int I_START_POP = 60;
const int I_POP_STEP = 8; 
const int I_POP_INCR_INTERVAL = 70;

//----------------SUBPOPS
const int I_SUB_POPS = 10;
const int I_MIGRATION_GAP = 90;
const int I_BEST_TO_MIGRATE = 26;
const int I_HELPERS = 5;
const int I_HELPER_POP_MIG = 65;
const double D_PAR_POP_SHIFT = 0.0011 / (I_SUB_POPS + I_HELPERS);
const double D_CROSS_POP_SHIFT = 0.07 / (I_SUB_POPS + I_HELPERS);

//----------------CROSSOVER
const int I_PARENTS_SUBGRPS = 4;
const double I_BIAS = 1.18;
const int I_OPTIONS = I_PARENTS_SUBGRPS * (1 + I_PARENTS_SUBGRPS) / 2.0;
const int I_MAX_CUTS_CROSS = 6;

//------------VARIETY
const int I_WAIT = 13;
const int I_MIG_WAIT = 3 * I_WAIT;
const int I_CHAOS_WAIT = 7 * I_WAIT;
const int I_KILL_WAIT = 8 * I_WAIT;
const int I_KILL_STAG_WAIT = 4 * I_WAIT;
const int I_CLONE_TH = 5;
const int I_CHAOS_COUNT = 105;
const int I_CHAOS_GENES = 150;
const int I_ALT_PARENT_CHOOSING = 10;

//------------PENALTIES DISTRIBUTION
const double D_GENE_PEN_MEAN = 0.00008;
const double D_GENE_PEN_DEV = 0.00005;
const double D_PARENT_PEN_MEAN = 0.0002;
const double D_PARENT_PEN_DEV = 0.0004;
const double D_CROSS_PEN_MEAN = 0.03;
const double D_CROSS_PEN_DEV = 0.08;

/*
VENI, VIDI, VICI:
	104b00 - optimum 0,000631114

	104b02 - moze optimum 0,000830022
	104b03 - moze optimum 0,000310511

	104b05  - 0,000203791
	104b06 - 0.000244828
	104b07 - 0.000165262

	104b08 - 9.67165e-05
	104b09 - 6.09422e-05

	104d01 - 3.12925e-05


	114b00 - 1
	114b01 - 1
	114b02 - 1

	114b04 - 1

	114b06 - 1
	
	11407 - moze optimum 0,00077101

	128b02 - 1
*/
