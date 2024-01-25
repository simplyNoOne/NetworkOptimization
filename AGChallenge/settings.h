#pragma once

//----------------DEBUG
const static char* S_CASE = "104b00";
const int I_TIME = 1800;

//----------------GENERAL
const double D_MUTATION_CHANCE = 0.001;
const double D_PARENT_MUTATION = 0.008;
const double D_PAR_POP_SHIFT = 0.0001;
const double D_CROSSOVER_CHANCE = 0.74;
const double D_CROSS_POP_SHIFT = 0.01;
const int I_GAP = 3; 

//----------------SIZING
const int I_POP_SIZE = 700;
const int I_START_POP = 60;
const int I_POP_STEP = 8; 
const int I_POP_INCR = 70;

//----------------SUBPOPS
const int I_SUB_POPS = 3;
const int I_MIGRATION_GAP = 70;
const int I_BEST_TO_MIGRATE = 26;
const int I_HELPERS = 1;
const int I_HELPER_POP_MIG = 115;

//----------------CROSSOVER
const int I_PARENTS_SUBGRPS = 4;
const double I_BIAS = 1.2;
const int I_OPTIONS = I_PARENTS_SUBGRPS * (1 + I_PARENTS_SUBGRPS) / 2.0;
const int I_MAX_CUTS_CROSS = 6;

//------------VARIETY
const int I_WAIT = 13;
const int I_MIG_WAIT = 3 * I_WAIT;
const int I_CHAOS_WAIT = 7 * I_WAIT;
const int I_CHAOS_COUNT = 45;
const int I_CHAOS_GENES = 350;
const int I_ALT_PARENT_CHOOSING = 10;

//------------PENALTIES DISTRIBUTION
const double D_GENE_PEN_MEAN = 0.00008;
const double D_GENE_PEN_DEV = 0.00005;
const double D_PARENT_PEN_MEAN = 0.001;
const double D_PARENT_PEN_DEV = 0.0005;
const double D_CROSS_PEN_MEAN = 0.06;
const double D_CROSS_PEN_DEV = 0.08;
