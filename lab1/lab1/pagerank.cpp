// author: lilongyang
// date: 2016-11-25
// pagerank.cpp :

#include "stdafx.h"
#include <iostream>
using namespace std;
#define TRACE 0
 
void myprintf(_Inout_ FILE * _File, _In_z_ _Printf_format_string_ const char * _Format, ...)
{
	if(TRACE)
	{
		va_list arg_ptr;
		fprintf(_File, _Format, arg_ptr);
	}
}

PNGraph loadGraph(TStr graphFilename)
{
	PNGraph graph;
	if(graphFilename.GetSubStr(graphFilename.Len() - 5) != ".snap")
	{
		myprintf(stdout, "Converting %s to snap format...\n", graphFilename.CStr());
		graph = TSnap::LoadEdgeList<PNGraph>(graphFilename, false);
		TFOut fOut(graphFilename + ".snap");
		graph->Save(fOut);
	}
	else
	{
		myprintf(stdout, "Loading Graph...\n");
		TFIn FIn(graphFilename);
		graph = TNGraph::Load(FIn);
	}
	myprintf(stdout, "# nodes: %d\n", graph->GetNodes());
	myprintf(stdout, "# edges: %lld\n", graph->GetEdges());
	return graph;
}


void count_in_out(PNGraph graph)
{
	int in_count=0;
	int out_count=0;
	for (TNGraph::TNodeI u = graph->BegNI(); u != graph->EndNI(); u++)
	{
		if(u.GetOutDeg()==0)
			out_count++;
		if(u.GetInDeg()==0)
			in_count++;
	}
	myprintf(stdout, "in_count=%d,out_count=%d\n",in_count,out_count);
}


std::vector<double> ExactPageRank(PNGraph g, double teleportProb,
	double threshold)
{
	int maxId = 0;
	for (TNGraph::TNodeI u = g->BegNI(); u != g->EndNI(); u++)
	{
		maxId = (maxId > u.GetId())?maxId:u.GetId();
	}
	std::vector<double> pi(maxId + 1, 0.0);
	std::vector<double> ri(maxId + 1, 0.0);

	for (TNGraph::TNodeI u = g->BegNI(); u != g->EndNI(); u++)
	{
		if(u == g->BegNI())
		{
			pi[u.GetId()] = 0.0;
			ri[u.GetId()] = 1.0;
		}

		else
		{
			pi[u.GetId()] = 0.0;
			ri[u.GetId()] = 0.0;
		}
	}
	int flag = 1;
	int sum2=0;
	int count = 0;

	while(flag)
	{
		count++;
		for (TNGraph::TNodeI u = g->BegNI(); u != g->EndNI(); u++)
		{
			if(u.GetOutDeg()==0)
			{
				pi[u.GetId()] += ri[u.GetId()]*teleportProb;
				ri[u.GetId()] = 0;
				sum2++;
			}
			else if(ri[u.GetId()]/u.GetOutDeg()>threshold)
			{
				for (long long int j = 0; j < u.GetOutDeg(); j++)
				{
					TNGraph::TNodeI v = g->GetNI(u.GetOutNId(j));
					ri[v.GetId()] += (1.0 - teleportProb) * ri[u.GetId()] / u.GetOutDeg();
				}
				pi[u.GetId()] = pi[u.GetId()] + ri[u.GetId()]*teleportProb;
				ri[u.GetId()] = 0;
			}
		}
		for (TNGraph::TNodeI u = g->BegNI(); u != g->EndNI(); u++)
		{
			if(u.GetOutDeg()>0)
			{
				if(ri[u.GetId()]/u.GetOutDeg()>threshold)
				{
					flag=1;
					break;
				}
				else
				{
					flag = 0;
				}
			}
		}
		myprintf(stdout,"flag= %d,count = %d\n", flag,count);
	}

	double sum1 = 0.0;
	for (unsigned int i = 0; i < pi.size(); i++)
	{
		sum1 =sum1+ pi[i]+ri[i];
	}
	myprintf(stdout,"sum of pi+ri = %g, dangling nodes=%d\n", sum1,sum2/count);
	return pi;
}



THash<TInt, TFlt> monteCarloPPR(PNGraph g, TNGraph::TNodeI start, double teleportProb, long long int nWalks, TRnd& rnd, const THashSet<TInt>* terminationSet, long long *stepCount) 
{		
	THash<TInt, TFlt> estimates;
	for (long long int i = 0; i < nWalks; i++) {
		//Current location is v
		TNGraph::TNodeI v = start;
		while (rnd.GetUniDev() > teleportProb && (terminationSet == NULL || !terminationSet->IsKey(v.GetId()))) {
			myprintf(stdout, "Walked to %d\n", v.GetId());
			if (v.GetOutDeg() > 0) {
				int newId = v.GetOutNId(rnd.GetUniDevInt(v.GetOutDeg()));
				v = g->GetNI(newId);
			} else {} // use imaginary self loop for sink node
			if (stepCount != NULL)
				(*stepCount)++;
		}
		myprintf(stdout, "Completed walk at %d\n", v.GetId());
		if (!estimates.IsKey(v.GetId())) {
			estimates.AddDat(v.GetId(), 0.0);
		}
		estimates.GetDat(v.GetId()) += 1.0 / nWalks;
	}
	return estimates;
}

int main(int argc, char* argv[])
{
	PNGraph graph = loadGraph(argv[1]);

	//PNGraph graph = loadGraph(argv[1]);
	//PNGraph graph = TSnap::GenRndGnm<PNGraph>(atoi(argv[1]), atoi(argv[2]));
	//TSnap::SaveEdgeList(graph, "gen.txt", "Save the running graph");

	double teleportProb = 0.15;
	long long int nWalks = atoi(argv[2]);
	//double threshold = 4.0 / graph->GetNodes();
	double threshold = 0.00000001;
	count_in_out(graph);

	TRnd rnd;
	myprintf(stdout, "--node 0 running monteCarloPPR----------------------------------------------------------------------------\n");
	FILE *f0 = fopen("data\\monto0.txt","w");    
	TNGraph::TNodeI start = graph->GetNI(0);	
	THash<TInt, TFlt> estimates = monteCarloPPR(graph, start,  teleportProb, nWalks, rnd, NULL, NULL);	
	for(THash<TInt, TFlt>::TIter key = estimates.BegI(); key<estimates.EndI(); key++)
	{
		fprintf(f0, "%d, %.40f\n",key.GetKey(),key.GetDat());
	}
	fclose(f0);
	myprintf(stdout, "--jump running monteCarloPPR----------------------------------------------------------------------------\n");

	int num = start.GetOutDeg();
	std::vector< THash<TInt, TFlt> > num_estimates(num);
	FILE *fn;
	char *filename = (char*) malloc(16);

	nWalks = nWalks/num;
	char jstr[32];
	for(int j=0;j<num;j++)
	{		
		strcpy(filename, "data\\temp\\");
		itoa(start.GetOutNId(j),jstr,10);
		strcat( strcat(filename,jstr ) , ".txt");
		fn = fopen(filename,"w");
		TNGraph::TNodeI node = graph->GetNI(start.GetOutNId(j));
		num_estimates[j] = monteCarloPPR(graph, node,  teleportProb, nWalks, rnd, NULL, NULL);
		for(THash<TInt, TFlt>::TIter key = num_estimates[j].BegI(); key<num_estimates[j].EndI(); key++)
		{
			fprintf(fn, "%d, %.40f\n",key.GetKey(),key.GetDat());
		}
		fclose(fn);
	}
	myprintf(stdout, "--Synthesis jump data----------------------------------------------------------------------------\n");
	FILE *fsum = fopen("data\\monto-sum.txt","w");
	THash<TInt, TFlt> final=estimates;

	for(THash<TInt, TFlt>::TIter key = estimates.BegI(); key<estimates.EndI(); key++)
	{
		double sss=0.0;  

		for(int k=0;k<num;k++)
		{
			if(num_estimates[k].IsKey(key.GetKey()))
				sss += num_estimates[k].GetDat(key.GetKey());
		}
		final.GetDat(key.GetKey()) = sss*(1-teleportProb)/num;
		if(key.GetKey() == 0)
			final.GetDat(key.GetKey()) += teleportProb;
	}
	for(THash<TInt, TFlt>::TIter key = final.BegI(); key < final.EndI(); key++)
	{
		fprintf(fsum, "%d, %.40f\n",key.GetKey(),key.GetDat());
	}

	fprintf(stdout, "--running exactPageRank----------------------------------------------------------------------------\n");

	FILE *fexact = fopen("data\\exact.txt","w");
	std::vector<double> global = ExactPageRank(graph, teleportProb, threshold);

	for(THash<TInt, TFlt>::TIter key = estimates.BegI(); key<estimates.EndI(); key++)
	{
		fprintf(fexact, "%d,%.40f\n", key.GetKey(), global[key.GetKey()]);
	}
	fclose(fexact);

	myprintf(stdout, "exit\n");
	return 0;
}