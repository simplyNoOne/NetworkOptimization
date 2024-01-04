#pragma once
const double D_MUTATION_CHANCE = 0.001;
const double dCrossoverChance = 0.65;
const int I_POP_SIZE = 500;
//----------------
const int I_SUB_POPS = 8;
const int iMigrationGap = 500;
const int iBestToMigrate = 20;
const bool B_ASYNC = false;
//----------------
const int I_PARENTS_SUBGRPS = 5;
const int I_SUB_GRP_SIZE = I_POP_SIZE / I_PARENTS_SUBGRPS;
const double I_BIAS = 1.30;
const int I_OPTIONS = I_PARENTS_SUBGRPS * (1 + I_PARENTS_SUBGRPS) / 2.0;