#pragma once
//----------------GENERAL
const double D_MUTATION_CHANCE = 0.001;
const double D_PARENT_MUTATION = 0.005;
const double D_CROSSOVER_CHANCE = 0.65;
const int I_POP_SIZE = 230;

//----------------SUBPOPS
const int I_SUB_POPS = 1;
const int I_MIGRATION_GAP = 40;
const int I_BEST_TO_MIGRATE = 16;
//----------------CROSSOVER
const int I_PARENTS_SUBGRPS = 5;
const int I_SUB_GRP_SIZE = I_POP_SIZE / I_PARENTS_SUBGRPS;
const double I_BIAS = 1.32;
const int I_OPTIONS = I_PARENTS_SUBGRPS * (1 + I_PARENTS_SUBGRPS) / 2.0;
const int I_MAX_CUTS_CROSS = 4;

//------------MODIFIER
inline double D_PENALTY = 0.0;
const double D_PUSH = 0.00004;
const int I_WAIT = 11;