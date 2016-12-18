/*
 * pagerank_algorithms.h
 *
 *  Created on: Jan 18, 2014
 *      Author: peter
 */

#ifndef PAGERANK_ALGORITHMS_H_
#define PAGERANK_ALGORITHMS_H_

// For some reason the max macro interferes with vector
#undef max
#undef min
#include <vector>
#include <queue>
#include "Snap.h"
//#include <sys/time.h>

#define TRACE 0

THash<TInt, TFlt> computeContributions(PNGraph g, TNGraph::TNodeI target,
		double teleportProb, double epsilon, long long *stepCount = NULL);

THash<TInt, TFlt> computeContributionsWithoutPQueue(PNGraph g, TNGraph::TNodeI target,
		double teleportProb, double epsilon, long long *stepCount = NULL);

THash<TInt, TFlt> monteCarloPPR(PNGraph g, TNGraph::TNodeI start,
		double teleportProb, long long int nWalks, TRnd& rnd,
		const THashSet<TInt>* terminationSet = NULL, long long *stepCount = NULL);

double fastPPR(PNGraph g, TNGraph::TNodeI start, TNGraph::TNodeI target,
		double teleportProb, double threshold, double approximationRatio, double failureProbability, TRnd& rnd, bool verbose, bool expandInNeighbors,
		const std::vector<double>& globalPR = std::vector<double>(), double edgeCount = -1.0, double* forwardTime = NULL, double* reverseTime = NULL, long long *stepCount = NULL);

TNGraph::TNodeI samplePageRank(PNGraph g, double teleportProb, TRnd& rnd);

std::vector<double> globalPageRank(PNGraph g, double teleportProb, double threshold);

double countEdges(PNGraph g );



// utilities

void printDoubles(const TVec<double>& xs);

void printDoublesForPython(const std::vector<double> d);

void printMatrixTSV(const std::vector<std::vector<double>  >& m);

void printMatrixForPython(const std::vector<std::vector<double>  >& m);

std::vector<std::vector<double> > transposeMatrix(const std::vector<std::vector<double> >& m);
double mean(const TVec<double>& xs);

double percentile(TVec<double>& xs, double percentile);

double wallClockTime();

#endif /* PAGERANK_ALGORITHMS_H_ */
