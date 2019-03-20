This implements Lin-Kernighan-style k-opt, but using a quad tree to limit the search.
The original LK algorithm limited search neighborhoods for each point to 5 nearest neighbors.
The LKH algorithm uses "alpha-nearness" to determine search neighborhoods, which essentially considers points neighbors if new edges are similar enough to 1-tree edges. Constructing the candidate set is done once before hill-climbing and takes O(n^2) work.

The advantage of using a quad tree is that all possible improving steps
(deletion of one edge and addition of one edge) can be guaranteed to be considered.

The efficacy of LK can be attributed to the fact that k-opt moves contain at least
one addition and one deletion of edges that occur at a common point.
LK looks directly for this feature instead of trying expensive k-opt combinations
in the typical naive manner.

This repo only implements "forward swaps", which are a subset of typical acceptable LK/LKH moves.
Removed edges must occur in traversal order. Candidates for removal are limited to edges coming
"after" (in terms of traversal order) the most recently removed edge.

Warning: this implementation will take a long time per iteration for bad tours,
as the search radius is dynamic and dependent on current-tour segment lengths,
and the best improvment is found.

Compilation:
1. Make sure "CXX" in "makefile" is set to the desired compiler.
2. Run "make".

Running:
1. Run "./k-opt.out" for usage details.

Style notes:
1. Namespaces follow directory structure. If an entire namespace is in a single header file, the header file name will be the namespace name.
2. Headers are grouped from most to least specific to this repo (e.g. repo header files will come before standard library headers).
3. Put one line break in between function definitions for convenient vim navigation via ctrl + { and ctrl + }.

TODO:
1. Check if output directory exists before writing out paths to file (currently silently fails).



