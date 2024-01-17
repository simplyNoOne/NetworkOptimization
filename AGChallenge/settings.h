#pragma once
//----------------GENERAL
const int I_TIME = 300;
const double D_MUTATION_CHANCE = 0.001;
const double D_PARENT_MUTATION = 0.07;
const double D_CROSSOVER_CHANCE = 0.65;
const int I_POP_SIZE = 400;
const int I_START_POP = 40;
const int I_GAP = 3;

//----------------SUBPOPS
const int I_SUB_POPS = 3;
const int I_MIGRATION_GAP = 70;
const int I_BEST_TO_MIGRATE = 20;
const int I_POP_STEP = 8;
const int I_POP_INCR = 90;

//----------------CROSSOVER
const int I_PARENTS_SUBGRPS = 4;
const double I_BIAS = 1.36;
const int I_OPTIONS = I_PARENTS_SUBGRPS * (1 + I_PARENTS_SUBGRPS) / 2.0;
const int I_MAX_CUTS_CROSS = 4;

//------------PENALTIES
const double D_PARENT_PUSH = 0.005;
const double D_GENE_PUSH = 0.00004;
const double D_MAX_GENE_PEN = 0.003;
const double D_MAX_PARENT_PEN = 0.03;
const int I_WAIT = 12;
