/*
 * umbelt_opcodes.h
 * ----------------
 * This file defines all the Umbelt opcodes that are executable by the umbelt
 * device. These are received through the bleuart interface.
 *
 * The Umbelt python client will parse the opcodes in this file. This way, the
 * opcodes do not need to be specified twice.
 */

#define UMBELT_OP_HAPTIC          0x00
#define UMBELT_OP_COMPASS_ENABLE  0x01
#define UMBELT_OP_COMPASS_DISABLE 0x02
