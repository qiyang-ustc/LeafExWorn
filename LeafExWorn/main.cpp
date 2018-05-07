#include<iostream>
#include<fstream>
#include"my_rng.h"
#include"my_vrbls.h"
#include"my_statistics.h"
#include"Markov.h"
#include<vector>
#include<stdlib.h>
void Initialize()
{
	Vol = (Lx+1)*Ly*2+2*Lx;
	Vol2 = pow(Vol,2);
	Vol4 = pow(Vol2,2);
	Block_Obs.resize(NBlock);
	Sample_Obs.resize(NSample);
	TotalSample = NBlock*NSample;
	NToss = NSample/5;   //We throw 20% of sample to let system converge to a new state.
}
time_t tm;

int main(int argc,char *argv[])
{
	set_elapse_time();
/*
	Lx = atoi(argv[1]);
	Ly = atoi(argv[2]);
	Jcp=atof(argv[3]);
	NBlock=atoi(argv[4]);
	NSample=atoi(argv[5]);
*/
	std::ofstream fout(OutFile, std::ios::app);

	//We use System parameters to get parameters;

	std::ifstream fin(InFile, std::ios::in);
	if (fin.is_open())
	{
		Get_Parameters(fin);
	}
	else
	{
		Get_Parameters(std::cin);
	}
	Initialize();
	Block b;// Honey-comb
	/*
	//test for adjacent matrix.
	//==========================TEST PASS============================
	int test = 1;
	while (test!=-1)
	{
		std::cout << b.adj(test, 1) << "	";
		std::cout << b.adj(test, 2) << "	";
		std::cout << b.adj(test, 3) << "	";
		std::cout << std::endl;
		std::cin >> test;
	}
	//test end
	//==========================TEST PASS============================
	 
	*/
	for (int i = 0;i < NBlock;i++)
	{
		for(int j = 0;j < NToss;j++)
		{
			Markov(b);
		}
		for (int j = 0;j < NSample;j++)
		{
			Markov(b);
			b.Calculate_Quan();
			Collect_data(b,j);
		}
		Normalize_data(i);
	}
	Analyze_data();
	write2file(fout);
	elapse_time();
	system("pause");
}
