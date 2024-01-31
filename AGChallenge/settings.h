#pragma once

//----------------GENERAL
const double D_MUTATION_CHANCE = 0.001;
const double D_PARENT_MUTATION = 0.04;		
const double D_CROSSOVER_CHANCE = 0.72;		
const int I_GAP = 3; 

//----------------SIZING
const int I_POP_SIZE = 400;
const int I_START_POP = 60;
const int I_POP_STEP = 4;
const int I_POP_INCR_INTERVAL = 69;

//----------------SUBPOPS
const int I_SUB_POPS = 6;		
const int I_MIGRATION_GAP = 80;		
const int I_BEST_TO_MIGRATE = 26;
const int I_HELPERS = 3;		
const int I_HELPER_POP_MIG = 63;		
const double D_PAR_POP_SHIFT = 0.0011 / (I_SUB_POPS + I_HELPERS);
const double D_CROSS_POP_SHIFT = 0.07 / (I_SUB_POPS + I_HELPERS);

//----------------CROSSOVER
const int I_PARENTS_SUBGRPS = 5;
const double I_BIAS = 1.21;
const int I_OPTIONS = I_PARENTS_SUBGRPS * (1 + I_PARENTS_SUBGRPS) / 2.0;
const int I_MAX_CUTS_CROSS = 6;
const double D_MET3_TH = 0.02;
const double D_MET2_TH = 0.8;
const int I_FANCIFY = 4;

//------------VARIETY
const int I_WAIT = 18;		
const int I_MIG_WAIT = 2 * I_WAIT;		
const int I_CHAOS_WAIT = 5 * I_WAIT;
const int I_RESET = 8 * I_WAIT;
const int I_KILL_WAIT = 110;
const int I_CLONE_TH = 3;
const int I_CHAOS_COUNT = 115;		
const int I_CHAOS_GENES = 100;


//------------PENALTIES DISTRIBUTION
const double D_GENE_PEN_MEAN = 0.00008;
const double D_GENE_PEN_DEV = 0.00005;
const double D_PARENT_PEN_MEAN = 0.02;  
const double D_PARENT_PEN_DEV = 0.03;		
const double D_CROSS_PEN_MEAN = 0.03;
const double D_CROSS_PEN_DEV = 0.08;


