#pragma once
//----------------GENERAL
const int I_TIME = 300;
const double D_MUTATION_CHANCE = 0.001;
const double D_PARENT_MUTATION = 0.008;
const double D_CROSSOVER_CHANCE = 0.65;
const int I_POP_SIZE = 200;
const int I_START_POP = 60;
const int I_GAP = 3;

//----------------SUBPOPS
const int I_SUB_POPS = 3;
const int I_MIGRATION_GAP = 40;
const int I_BEST_TO_MIGRATE = 20;
const int I_POP_STEP = 8;
const int I_POP_INCR = 90;

//----------------CROSSOVER
const int I_PARENTS_SUBGRPS = 5;
const double I_BIAS = 1.32;
const int I_OPTIONS = I_PARENTS_SUBGRPS * (1 + I_PARENTS_SUBGRPS) / 2.0;
const int I_MAX_CUTS_CROSS = 4;

//------------PENALTIES
const double D_PARENT_PUSH = 0.0000;
const double D_GENE_PUSH = 0.000002;
const double D_MAX_GENE_PEN = 0.0001;
const double D_MAX_PARENT_PEN = 0.002;
const int I_WAIT = 12;
