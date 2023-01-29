#include "umbelt_haptics.h"

/* --------- Umbelt Core Haptices ----------- */

static pulse batt_warn_pulse[6] = {
  {RESONANT_FREQ, 50},
  {PAUSE_FREQ, 50},
  {RESONANT_FREQ, 50},
  {PAUSE_FREQ, 50},
  {RESONANT_FREQ, 50},
  {PAUSE_FREQ, 50}
};

/* --------- Haptic Sample Library ---------- */
static float noteFrequenciesExt[11] {98, 110, 123, 131, 147, 165, 175, 196, 220, 247, 261}; // G2 --> C4

static pulse jackhammer[2] = {
  {150, 100},
  {0, 100}
};

static pulse bach[32] = {
  {noteFrequenciesExt[0], 100},
  {noteFrequenciesExt[4], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[8], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[4], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[4], 100},
  {noteFrequenciesExt[0], 100},
  {noteFrequenciesExt[4], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[8], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[4], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[4], 100},
  {noteFrequenciesExt[1], 100},
  {noteFrequenciesExt[5], 100},
  {noteFrequenciesExt[10], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[10], 100},
  {noteFrequenciesExt[5], 100},
  {noteFrequenciesExt[10], 100},
  {noteFrequenciesExt[5], 100},
  {noteFrequenciesExt[1], 100},
  {noteFrequenciesExt[5], 100},
  {noteFrequenciesExt[10], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[10], 100},
  {noteFrequenciesExt[5], 100},
  {noteFrequenciesExt[10], 100},
  {noteFrequenciesExt[5], 100}
};
static pulse maryHadALittleLamb [29] = {
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[3], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[5], 1000},
  {0, 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[4], 1000},
  {0, 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[7], 1000},
  {noteFrequenciesExt[7], 1000},
  {0, 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[3], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[3], 1000},
};
