#pragma once
//----------------GENERAL
const double D_MUTATION_CHANCE = 0.001;
const double D_CROSSOVER_CHANCE = 0.65;
const int I_POP_SIZE = 520;
const bool B_ASYNC = false;
//----------------ASYNC
const int I_SUB_POPS = 4;
const int I_MIGRATION_GAP = 700;
const int I_BEST_TO_MIGRATE = 10;
//----------------CROSSOVER
const int I_PARENTS_SUBGRPS = 5;
const int I_SUB_GRP_SIZE = I_POP_SIZE / I_PARENTS_SUBGRPS;
const double I_BIAS = 1.32;
const int I_OPTIONS = I_PARENTS_SUBGRPS * (1 + I_PARENTS_SUBGRPS) / 2.0;
const int I_MAX_CUTS_CROSS = 4;
