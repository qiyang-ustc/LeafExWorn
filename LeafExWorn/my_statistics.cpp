#include"my_statistics.h"
#include<fstream>
#include<iostream>
#include"my_vrbls.h"
#include<numeric>
#include<math.h>
#include<iomanip>
//Block->Data;
//Data will be analyzed in Analysis();
//------------Result-----------------
const int NObs = 3;
const int NQuan = 4;
std::vector<Data> Sample_Obs;
std::vector<Data> Block_Obs;
Data::Data(double ob0)
{
	Quan[0] = ob0;
}
/*
We calculate 0-2 in Nor_data();
0. M
1. M^2
2. M^4
3. Binder's cumulant
*/
char*  OutFile= "HoneyComb.dat";
char*  InFile = "2DXY_parameters.dat";
std::vector<double> Result_Ave;
std::vector<double> Result_Var;
std::vector<double> Result_Cor;
void Get_Parameters(std::istream &fin)
{
	std::cout << "Lx,Ly,Jcp,NBlock,NSample,please:" << std::endl;
	fin >> Lx;
	fin >> Ly;
	fin >> Jcp;
	fin >> NBlock;
	fin >> NSample;
	Block_Obs.resize(NBlock);
	Sample_Obs.resize(NSample);
}
void Get_Parameters(std::ifstream &fin)
{
	std::cout << "Lx, Ly,Jcp,NBlock,NSample,please:" << std::endl;
	fin >> Lx;
	fin >> Ly;
	fin >> Jcp;
	fin >> NBlock;
	fin >> NSample;
	Block_Obs.resize(NBlock);
	Sample_Obs.resize(NSample);
}
void write2file(std::ostream &fout)
{
	fout.setf(std::ios::fixed);
	fout<<std::setprecision(10);
	//fout << "Nblock	Nsample	Lx	Ly	p" << std::endl;
	fout << NBlock << "	 " << NSample << "	 " << Lx << "     " << Ly << "	  " << Jcp<<"	";
	//fout << "		Result:		" << std::endl;
	//fout << "Lx	Ly	Poc	Quan	Ave	Var	Cor" << std::endl;
	for (int i = 0;i < NQuan;i++)
	{
		fout <<i <<"	 "<< Result_Ave[i] << " 	" << sqrt(Result_Var[i]) << " 	" << sqrt(Result_Cor[i] / Result_Var[i]) <<"  ";
	}
	fout<<std::endl;
}
void Collect_data(Block& p, int i)//collect date from block p into Sample_Obs(i)
{
	//i ranges from 0-NSample-1
	p.Calculate_Quan();
	for(int j=0;j<NObs;j++)
	{
	Sample_Obs[i].Quan[j] = p.Quan[j];
	}
}
void Normalize_data(int p) //Normalize date and write it into Block_Obs(p)
//This is what you need to change if you want calculate some quantities just like 
/*   <M^2>^4/<M^4> and so on
*/
{
	double temp;
	for (int j = 0;j < NObs;j++)
	{
		temp = 0;
		for (int i = 0;i < NSample;i++)
		{
			temp += Sample_Obs[i].Quan[j];
		}
		Block_Obs[p].Quan[j] = temp/NSample;
	}
	Block_Obs[p].Quan[3] = Block_Obs[p].Quan[1] * Block_Obs[p].Quan[1]/ Block_Obs[p].Quan[2];
	/*
	We calculate 0-2 in Nor_data();
	0. M
	*/
}
void Analyze_data()
{
	Result_Ave.resize(NQuan);
	Result_Var.resize(NQuan);
	Result_Cor.resize(NQuan);
	int i,j;
	double ave,var,cor;
	double devn, devp;  //deviation now and deviation prev
	for (i = 0;i < NQuan;i++)
	{
	//----------get ave-------------
		ave = 0;
		for (j = 0;j < NBlock;j++)
		{
			ave += Block_Obs[j].Quan[i];
		}
		ave = ave / NBlock;
		Result_Ave[i] = ave;
	//----------get ave finish-------------
	//----------get var and cor-------------
		devn = 0;devp = 0;cor = 0;var = 0;
		for (j = 0;j < NBlock;j++)
		{
			devn = Block_Obs[j].Quan[i] - ave;
			var += devn*devn;
			cor += devn*devp;
			devp = devn;
		}
		//--------------c.f. cmd markdown - need to normalize Obs by multiplying 1/sqrt(NBLOCK)
		Result_Var[i] = var / NBlock;
		Result_Cor[i] = cor / NBlock;
	//----------get var and cor finish-------------
	}
	for(i=0;i<NObs;i++)
	{
		Result_Var[i] = Result_Var[i] / NBlock;
		Result_Cor[i] = Result_Cor[i] / NBlock;
	}
}
