#ifndef HPP_COLORING
#define HPP_COLORING

/* Module provides a functions for coloring graphs */

/* TODO:                                     *
 * - Add syntax const                        *
 * - Rework color and edges/vertexes finding */

#include <vector>
#include <unordered_set>

#include <utils/general.h>
#include <matrix/matrix.h>

#include <report/reporter.h>


typedef int color_t;
typedef std::vector<bool> cmask_t;
typedef std::vector<int> fan_t;


/* Misra & Gries edge coloring algorithm */
matrix color_edges(matrix& adj);


#endif
