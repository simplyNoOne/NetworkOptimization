#pragma once

//----------------DEBUG
const static char* S_CASE = "104b01";
const int I_TIME = 1800;

//----------------GENERAL
const double D_MUTATION_CHANCE = 0.001;
const double D_PARENT_MUTATION = 0.0082;
const double D_CROSSOVER_CHANCE = 0.73;
const int I_GAP = 2; 

//----------------SIZING
const int I_POP_SIZE = 700;
const int I_START_POP = 60;
const int I_POP_STEP = 6; // was 5 here
const int I_POP_INCR = 90;

//----------------SUBPOPS
const int I_SUB_POPS = 2;
const int I_MIGRATION_GAP = 50;
const int I_BEST_TO_MIGRATE = 26;
const int I_HELPERS = 2;
const int I_HELPER_POP_MIG = 115;

//----------------CROSSOVER
const int I_PARENTS_SUBGRPS = 5;
const double I_BIAS = 1.3;
const int I_OPTIONS = I_PARENTS_SUBGRPS * (1 + I_PARENTS_SUBGRPS) / 2.0;
const int I_MAX_CUTS_CROSS = 6;

//------------PENALTIES
const double D_PARENT_PUSH = 0.0004;
const double D_GENE_PUSH = 0.00001;
const double D_CROSS_PUSH = 0.0005;
const double D_MAX_GENE_PEN = 0.0002;
const double D_MAX_PARENT_PEN = 0.0012;
const double D_MAX_CROSS_PEN = 1 - D_CROSSOVER_CHANCE - 0.12;

//------------VARIETY
const int I_WAIT = 19;
const int I_MIG_WAIT = 3 * I_WAIT;
const int I_HELP_WAIT = 5 * I_WAIT;
const int I_CHAOS_COUNT = 40;


//------------DISTRIBUTION STATS
const double D_GENE_PEN_MEAN = 0.0001;
const double D_GENE_PEN_DEV = 0.00007;
const double D_PARENT_PEN_MEAN = 0.001;
const double D_PARENT_PEN_DEV = 0.0005;
const double D_CROSS_PEN_MEAN = 0.07;
const double D_CROSS_PEN_DEV = 0.1;
