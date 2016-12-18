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
    return a.value<b.value;
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
	cout<<count<<"\t"<<(end - begin)<<"\n";
	int xx =0;
	for(auto pitr=pipi.begin(),ritr=riri.begin();pitr!=pipi.end();pitr++,ritr++)
	{
		printf("%d,%.10f,%.10f\n",xx++,*pitr,*ritr);
	}
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
	cout<<count<<"\t"<<(end - begin)<<"\t";
	return;
}

void addedge(int s, int t)
{
	//g->AddEdge(s,t);
	TNGraph::TNodeI u = g->GetNI(s);
	//-------------------------------------
	cout<<s<<" "<<t<<endl;	
	long long int du = u.GetOutDeg();
	cout<< du<<endl;
	TNGraph::TNodeI v;
	for (long long int j = 0; j < du; j++)
	{
		if(u.GetOutNId(j) == t)
		{
			v = g->GetNI(u.GetOutNId(j));
			ri[v.GetId()] = (pi[s]/teleportProb*(1-teleportProb))/du + ri[v.GetId()];
		}
		else
		{
			v = g->GetNI(u.GetOutNId(j));			
			double incr;
			incr = (pi[s]/teleportProb*(1.0-teleportProb))*(1.0/du - 1.0/(du-1));
			ri[v.GetId()] = ri[v.GetId()] + incr;			
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
			v = g->GetNI(u.GetOutNId(j));
			ri[v.GetId()] = ri[v.GetId()] - (pi[s]/teleportProb*(1-teleportProb))/(1.0+du);
		}
		else
		{
			v = g->GetNI(u.GetOutNId(j));
			ri[v.GetId()] = ri[v.GetId()] + (pi[s]/teleportProb*(1.0-teleportProb))*(1.0/du - 1.0/(du+1));
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


void print_result()
{
	int count =0;
	for(auto pitr=pi.begin(),ritr=ri.begin();pitr!=pi.end();pitr++,ritr++)
	{
		printf("%d,%.10f,%.10f\n",count++,*pitr,*ritr);
	}
}



int main(int argc, char* argv[])
{
	g = loadGraph(argv[1]);
	cout<< argv[2]<< "\n";
	init(0.15, 0.001);
	long long int src,target;

	
	FP_PageRank();	

	cout<<endl<<"初始结果"<<endl;
	print_result();
	//g->AddEdge(0,3);
	//addedge(0,3);
	long long int edge_count = g->GetEdges();
	long long int cnt=0;
	srand((unsigned) time(NULL));
	
	while(cnt<  atof(argv[2]) )
	{
		//src = rand() % (maxId+1);
		//target = rand() % (maxId+1);
		src = 2; target = 4;
		if(src == target) continue;
		if(g->IsNode(src) && g->IsNode(target))
		{
			if(g->AddEdge(src,target) == -2)
				continue;
			else
			{
				addedge(src,target);
				cout<<endl<<"调整结果"<<endl;
				print_result();
				cnt++;
				//cout<<cnt<<endl;
			}
		}
	}
	cout<<"====================="<<endl;
	//cout<<"节点添加完成"<<endl;
	FP_PageRank();   //按照增量算的
	
	cout<<endl<<"二次结果"<<endl;
	print_result();
	cout<<endl<<"精确结果"<<endl;
	std::vector<double> pexact = ExactPageRank(g,teleportProb,threshold);   //直接算的
	auto pexact_itr = pexact.begin();

	double sum_acu =0.0;
	double sum_pi = 0.0;
	for(auto pitr= pi.begin(); pitr!= pi.end(); pitr++)
	{
		//cout<< *pitr <<"vs. "<<*pexact_itr<<endl;		
		sum_acu += fabs(*pitr - *pexact_itr);
		sum_pi += *pexact_itr;
		pexact_itr++;
	}
	//printf("精度=%.40f, ", 1 - sum_acu / sum_pi  );
	printf("%f\t",1 - sum_acu / sum_pi);

	int k[5]={100,500,1000,5000,10000};
	for(int j=0;j<5;j++)
	{
		std::vector<int> i=get_topk(pi,k[j]);
		std::vector<int> exact=get_topk(pexact,k[j]);
		std::sort(i.begin(),i.end());
		std::sort(exact.begin(),exact.end());
		std::vector<int> result(k[j]);	
		std::vector<int>::iterator it = std::set_intersection( i.begin(), i.end(), exact.begin(), exact.end() , result.begin() );
		result.resize(it-result.begin());
		cout<<result.size()/(double)k[j]<<"\t";
	}
	cout<<endl;
	return 0;
}

