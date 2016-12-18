// lab1.cpp : 定义控制台应用程序的入口点。
//

// author: lilongyang
// date: 2016-11-25
// pagerank.cpp :

#include "stdafx.h"
#include <math.h>
#include <iostream>
#define TRACE 0
#include <Windows.h>
using namespace std;


long long int maxId=0;
PNGraph g;
std::vector<double> pi;
std::vector<double> ri;	
double teleportProb;
double threshold;

PNGraph loadGraph(TStr graphFilename)
{
	PNGraph graph;
	if(graphFilename.GetSubStr(graphFilename.Len() - 5) != ".snap")
	{
		//fprintf(stdout, "Converting %s to snap format...\n", graphFilename.CStr());
		graph = TSnap::LoadEdgeList<PNGraph>(graphFilename, false);
		TFOut fOut(graphFilename + ".snap");
		graph->Save(fOut);
	}
	else
	{
		//fprintf(stdout, "Loading Graph...\n");
		TFIn FIn(graphFilename);
		graph = TNGraph::Load(FIn);
	}
	//cout<< "图数据加载完毕"<<endl;
	//fprintf(stdout, "# nodes: %d\n", graph->GetNodes());
	//fprintf(stdout, "# edges: %lld\n", graph->GetEdges());
	
	return graph;
}

void init(double a, double b)
{
	teleportProb = a;  //0.15;
	threshold = b;  //0.00000001;

	for (TNGraph::TNodeI u = g->BegNI(); u != g->EndNI(); u++)
	{
		maxId = (maxId > u.GetId())?maxId:u.GetId();
	}
	pi.resize(maxId + 1,0);
	ri.resize(maxId + 1,0);
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

}

struct XXX
{
	int index;
	double value;
};
bool comp(const XXX &a,const XXX &b)
{
    return a.value > b.value;
}
bool comp2(const XXX &a,const XXX &b)
{
    return a.value < b.value;
}
bool comp4(const XXX &a,const XXX &b)
{
    return a.index < b.index;
}
bool comp3(std::vector<XXX>::iterator itra, std::vector<XXX>::iterator itrb)
{
	return (*itra).index < (*itrb).index;
}
//获取按值排序的index数组
std::vector<int> get_topk(std::vector<double> &v,int k)
{

	std::vector<XXX> x(v.size());
	int count=0;
	XXX temp;
	for(auto itr = v.begin(); itr!=v.end(); itr++)
	{		
		temp.index = ++count;
		temp.value = (*itr);
		x.push_back(temp);
	}
	std::sort(x.begin(),x.end(),comp);
	std::vector<int> result(v.size());
	for(auto itr =x.begin(); itr!= x.end(); ++itr)
		result.push_back((*itr).index);	
	std::vector<int> topk(k);
	topk.assign(result.end()-k,result.end());
	return topk;
}


THash<TInt, TFlt> monteCarloPPR(PNGraph g, TNGraph::TNodeI start, double teleportProb, long long int nWalks, TRnd& rnd, const THashSet<TInt>* terminationSet, long long *stepCount) 
{		
	THash<TInt, TFlt> estimates;
	for (long long int i = 0; i < nWalks; i++) {
		//Current location is v
		TNGraph::TNodeI v = start;
		while (rnd.GetUniDev() > teleportProb && (terminationSet == NULL || !terminationSet->IsKey(v.GetId()))) {
			//fprintf(stdout, "Walked to %d\n", v.GetId());
			if (v.GetOutDeg() > 0) {
				int newId = v.GetOutNId(rnd.GetUniDevInt(v.GetOutDeg()));
				v = g->GetNI(newId);
			} else {} // use imaginary self loop for sink node
			if (stepCount != NULL)
				(*stepCount)++;
		}
		//fprintf(stdout, "Completed walk at %d\n", v.GetId());
		if (!estimates.IsKey(v.GetId())) {
			estimates.AddDat(v.GetId(), 0.0);
		}
		estimates.GetDat(v.GetId()) += 1.0 / nWalks;
	}
	return estimates;
}

std::vector<double> ExactPageRank(PNGraph g, double teleportProb,
	double threshold)
{
	std::vector<double> pipi(maxId + 1, 0.0);
	std::vector<double> riri(maxId + 1, 0.0);

	for (TNGraph::TNodeI u = g->BegNI(); u != g->EndNI(); u++)
	{
		if(u == g->BegNI())
		{
			pipi[u.GetId()] = 0.0;
			riri[u.GetId()] = 1.0;
		}

		else
		{
			pipi[u.GetId()] = 0.0;
			riri[u.GetId()] = 0.0;
		}
	}
	int flag = 1;
	int count = 0;
	double begin = GetTickCount();
	while(flag)
	{		
		for (TNGraph::TNodeI u = g->BegNI(); u != g->EndNI(); u++)
		{
			if(u.GetOutDeg()==0)
			{
				pipi[u.GetId()] += riri[u.GetId()]*teleportProb;
				riri[u.GetId()] = 0;
			}
			else if(riri[u.GetId()]/u.GetOutDeg()>threshold)
			{
				for (long long int j = 0; j < u.GetOutDeg(); j++)
				{
					TNGraph::TNodeI v = g->GetNI(u.GetOutNId(j));
					riri[v.GetId()] += (1.0 - teleportProb) * riri[u.GetId()] / u.GetOutDeg();
				}
				count++;
				pipi[u.GetId()] = pipi[u.GetId()] + riri[u.GetId()]*teleportProb;
				riri[u.GetId()] = 0;
			}
		}
		for (TNGraph::TNodeI u = g->BegNI(); u != g->EndNI(); u++)
		{
			if(u.GetOutDeg()>0)
			{
				if(riri[u.GetId()]/u.GetOutDeg()>threshold)
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
		//fprintf(stdout,"flag= %d,count = %d\n", flag,count);
	}
	
	double end = GetTickCount();
	
	double sum1 = 0.0;
	double sum3 = 0.0;
	for (unsigned int i = 0; i < pipi.size(); i++)
	{
		sum1 =sum1+pipi[i];
		sum3 += riri[i];
	}
	//fprintf(stdout,"\nsum of pi = %.8f, ri = %.8f, 传递次数=%d\n", sum1,sum3,count);
	//cout<<"exact的时间"<<(end - begin)<<"ms"<<endl;
	cout<<count<<","<<(end - begin)<<",";
	return pipi;
}

std::vector<double> Exact2PageRank(PNGraph g, double teleportProb,
	double threshold)
{
	std::vector<double> pipi(maxId + 1, 0.0);
	std::vector<double> riri(maxId + 1, 0.0);
	for (TNGraph::TNodeI u = g->BegNI(); u != g->EndNI(); u++)
	{
		if(u == g->BegNI())
		{
			pipi[u.GetId()] = 0.0;
			riri[u.GetId()] = 1.0;
		}
		else
		{
			pipi[u.GetId()] = 0.0;
			riri[u.GetId()] = 0.0;
		}
	}
	int flag = 1;
	int sum2=0;
	int count = 0;

	set<int> newset ;
	set<int> oldset;

	oldset.insert(0);
	double begin = GetTickCount();
	while(flag)
	{
		count++;
		for( auto itr = oldset.begin(); itr!=oldset.end(); itr++)
		{
			TNGraph::TNodeI u = g->GetNI( *itr );

			if(u.GetOutDeg()==0)
			{
				pipi[u.GetId()] += riri[u.GetId()]*teleportProb;
				riri[u.GetId()] = 0;
				sum2++;
			}
			else if(riri[u.GetId()]/u.GetOutDeg()>threshold)
			{
				for (long long int j = 0; j < u.GetOutDeg(); j++)
				{
					newset.insert(u.GetOutNId(j));
					TNGraph::TNodeI v = g->GetNI(u.GetOutNId(j));
					riri[v.GetId()] += (1.0 - teleportProb) * riri[u.GetId()] / u.GetOutDeg();
				}
				pipi[u.GetId()] = pipi[u.GetId()] + riri[u.GetId()]*teleportProb;
				riri[u.GetId()] = 0;
			}			
		}
		oldset.swap(newset);     //oldset <= newset;
		newset.clear();
		for (TNGraph::TNodeI u = g->BegNI(); u != g->EndNI(); u++)
		{
			if(u.GetOutDeg()>0)
			{
				if(riri[u.GetId()]/u.GetOutDeg()>threshold)
				{
					flag = 1;
					break;
				}
				else
				{
					flag = 0;
				}
			}
		}
	}
	double end = GetTickCount();
	cout<<"精确值的时间"<<(end - begin)<<"ms"<<endl;


	cout<<count<<endl;
	double sum1 = 0.0;
	double sum3 = 0.0;
	for (unsigned int i = 0; i < pipi.size(); i++)
	{
		sum1 =sum1+pipi[i];
		sum3 += riri[i];
	}
	fprintf(stdout,"sum of pi = %.8f, ri = %.8f, dangling nodes=%d\n", sum1,sum3,sum2/count);
	return pipi;
}


void FP_PageRank()
{		
	int flag = 1;
	int count = 0;
	double begin = GetTickCount();
	while(flag != 0)
	{		
		//fprintf(stdout,"flag= %d,count = %d\n", flag,count);
		for (TNGraph::TNodeI u = g->BegNI(); u != g->EndNI(); u++)
		{
			if(u.GetOutDeg()==0)
			{
				pi[u.GetId()] += ri[u.GetId()]*teleportProb;
				ri[u.GetId()] = 0;
			}
			else if(ri[u.GetId()]/u.GetOutDeg()>threshold || ri[u.GetId()]/u.GetOutDeg() < (0 - threshold) )
			{
				for (long long int j = 0; j < u.GetOutDeg(); j++)
				{
					TNGraph::TNodeI v = g->GetNI(u.GetOutNId(j));
					ri[v.GetId()] += (1.0 - teleportProb) * ri[u.GetId()] / u.GetOutDeg();
				}
				++count;
				pi[u.GetId()] = pi[u.GetId()] + ri[u.GetId()]*teleportProb;
				ri[u.GetId()] = 0;
			}
		}
		for (TNGraph::TNodeI u = g->BegNI(); u != g->EndNI(); u++)
		{
			if(u.GetOutDeg()>0)
			{
				if(ri[u.GetId()]/u.GetOutDeg()>threshold || ri[u.GetId()]/u.GetOutDeg() < (0 - threshold))
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
			
	}

	//cout<<endl;
	double sum1 = 0.0;
	double sum3 = 0.0;
	for (unsigned int i = 0; i < pi.size(); i++)
	{
		sum1 =sum1+ pi[i];
		sum3 = sum3 + ri[i];
	}
	//fprintf(stdout,"sum of pi = %.8f, ri = %.8f, 传递次数=%d\n", sum1,sum3,count);
	double end = GetTickCount();
	//cout<<"FP的时间"<<(end - begin)<<"ms"<<endl;
	//cout<<count<<","<<(end - begin)<<",";
	return;
}

void addedge(int s, int t)
{
	//g->AddEdge(s,t);
	TNGraph::TNodeI u = g->GetNI(s);
	//-------------------------------------
		
	long long int du = u.GetOutDeg();
	//cout<< du<<endl;
	TNGraph::TNodeI v;
	for (long long int j = 0; j < du; j++)
	{
		if(u.GetOutNId(j) == t)
		{
			ri[j] = (pi[s]/teleportProb*(1-teleportProb))/du + ri[j];
		}
		else
		{
			v = g->GetNI(u.GetOutNId(j));			
			double incr;
			incr = (pi[s]/teleportProb*(1.0-teleportProb))*(1.0/du - 1.0/(du-1));
			ri[j] = ri[j] + incr;			
		}			
	}
}

void deledge(int s, int t)
{
	g->DelEdge(s,t);
	TNGraph::TNodeI u = g->GetNI(s);
	//-------------------------------------
		
	long long int du = u.GetOutDeg();
	TNGraph::TNodeI v;
	for (long long int j = 0; j < du; j++)
	{
		if(u.GetOutNId(j) == t)
		{
			ri[j] = ri[j] - (pi[s]/teleportProb*(1-teleportProb))/(1.0+du);
		}
		else
		{
			v = g->GetNI(u.GetOutNId(j));
			ri[j] = ri[j] + (pi[s]/teleportProb*(1.0-teleportProb))*(1.0/du - 1.0/(du+1));
		}			
	}
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
	fprintf(stdout, "in_count=%d,out_count=%d\n",in_count,out_count);
}

int main(int argc, char* argv[])
{
	g = loadGraph(argv[1]);
	//cout<< argv[2]<< ",";
	init(0.15, 0.00000001);
	//long long int src,target;
	FP_PageRank();	
	std::vector<XXX> x(maxId+1);
	int count=0;
	XXX temp2;
	for(auto itr = pi.begin(); itr!=pi.end(); itr++)
	{		
		temp2.index = count++;
		temp2.value = (*itr);
		x.push_back(temp2);
	}
	
	/*FILE * f1;
	f1 =fopen("FP.txt", "w");
	for(auto pitr= x.begin(); pitr!= x.end(); pitr++)
		fprintf(f1,"%d,%f\n",(*pitr).index,(*pitr).value);
	fclose(f1);*/


	/////////////////////////////////////////////////////////////
	long long int nWalks = atoi(argv[2]);
	TRnd rnd;
	TNGraph::TNodeI start = g->GetNI(1);
	THash<TInt, TFlt> estimates = monteCarloPPR(g, start,  teleportProb, nWalks, rnd, NULL, NULL);	
	vector<XXX> random(atoi(argv[2]));
	for(THash<TInt, TFlt>::TIter key = estimates.BegI(); key<estimates.EndI(); key++)
	{
		XXX temp;
		temp.index = key.GetKey(); temp.value = key.GetDat();
		random.push_back(temp);
	}

	std::sort(x.begin(),x.end(),comp);
	std::sort(random.begin(),random.end(),comp);
	int R = atoi(argv[2]);
	int k = atoi(argv[3]);
	cout<<R<<"\t"<<k<<"\t";
	double beta = random[k-1].value - 0.0325 / std::sqrt( (double)atoi(argv[2]) ); //限定阈值
	if(beta<0) {
		cout<<"error"<<endl;
		exit(-1);
	}
	int random_count = 0;
	vector<XXX> random_sub;
	vector<XXX> x_sub;
	vector<XXX> random_mr_sub;
	
	x_sub.assign(x.begin(),x.begin() + k);
	random_mr_sub.assign(random.begin(),random.begin() + k);

	for(auto itr = random.begin();itr != random.end(); ++itr)
	{
		if( (*itr).value < beta )
		{
			random_sub.assign(random.begin(),itr);
			break;
		}
	}

	std::sort(x_sub.begin(),x_sub.end(),comp4);
	std::sort(random_sub.begin(),random_sub.end(),comp4);
	std::vector<XXX> result(x_sub.size());	
	std::vector<XXX>::iterator it = std::set_intersection( x_sub.begin(), x_sub.end(), random_sub.begin(), random_sub.end() , result.begin(),comp4 );
	result.resize(it-result.begin());
	int card = result.size();
	cout<<card<<"\t";
	double a = card/(double)random_sub.size();
	double b = card/(double)x_sub.size();
	double c = 2*a*b /(a+b);
	cout<<a<<"\t"<<b<<"\t"<<c<<"\t";

	std::sort(x_sub.begin(),x_sub.end(),comp4);
	std::sort(random_mr_sub.begin(),random_mr_sub.end(),comp4);
	std::vector<XXX> result2(x_sub.size());
	std::vector<XXX>::iterator it2 = std::set_intersection( x_sub.begin(), x_sub.end(), random_mr_sub.begin(), random_mr_sub.end() , result2.begin(),comp4 );
	result2.resize(it2-result2.begin());
	int card2 = result2.size();
	cout<<card2<<endl;
	
	

	/*FILE * f2;
	f2 =fopen("random.txt", "w");
	for(auto pitr= random.begin(); pitr!= random.end(); pitr++)
		fprintf(f2,"%d,%f\n",(*pitr).index,(*pitr).value);
	fclose(f2);*/
	return 0;
}

