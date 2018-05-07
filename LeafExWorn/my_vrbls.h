
//*******************************************************************
// Ising model on the square Lattice

// Error bars are calculated using the blocking technique.
// Let ' \chi^2 = \sum_{t=1}^{T} <(O_t -<O>)^2> ' be the squared chi for
// 'T' blocks of observable 'O'. Assuming each block of data is independent
// of each other, the error bar of 'O' is given by 'Dev = \sqrt{\chi^2/T(T-1)}.

// Reliabity of the obtained errors is monitored by t=1 correlation,
// for which tolerance is set by variable 'tol' (default: tol=0.20d0).

// Composite quantities like Binder ratios are calculated in each block, and
// the associated error bars are obtained from their fluctuations.

// Results are written into a special file 'dat.***' if the number of
// blocks is less than 125 or correlation is too big. Data in each
// block will be also printed out in this case.

// Default number of extensive simulation is 'NBlck=1024'.

// For test purpose, for which huge amount of information will be
// printed out, 'NBlck' should be set smaller but >2.

// Dynamical behavior is not studied.

// 'my_vrbls.f90', 'carlo.f90', 'monte.f90', 'measure.f90',
// 'write2file.f90' and etc need to be modified for new projects.

//  Look for 'PROJECT-DEPENDENT'.

//  Author: Yuan Huang
//  Date  : April 19th, 2012.
//*******************************************************************

// Look for 'PROJECT-DEPENDENT' for different projects
#include <vector>
#include<iostream>
#include"my_rng.h"
#include <math.h>
#ifndef _MY_VRBLS_
#define _MY_VRBLS_
//------------------Project-Independed---------
extern const double tm32;	 //1/2^(32)
extern const double eps;     //very small number
extern const double tol;	 //tolerance for correlation 0.2
extern const int MaxInt;	 //Maximun integer
extern const int MinInt;	 //Minimun integer
extern double Pi;      //PI
extern double Double_Pi;  //2*Pi
extern const int MaxBlock;	     //Maximun number of block
extern const int MinBlock;		 //Minimun number of block
extern int NBlock;               //N=number of
extern int NSample;				// samples in a block
extern int TotalSample;
extern int NToss;				//samples to be trown away
extern int Collect_Interval;
//----------------Project-depended-------------
extern const int Dimension;
extern int Lx, Ly;
extern double Jcp;
extern double Jcp_Final;
extern int Vol; extern double Vol2; extern double Vol4;
extern int ident;//used to identify cluster
extern const int NObs;
extern const int NQuan;
template<class T>
/* Some algorithm which may be useful*/
void Delete_Num(typename std::vector<T>& v, T del)
//Can not Delete Points!!!
{
	for (typename std::vector<T>::iterator iter = v.begin();iter != v.end();)
	{
		if (del == *iter)
		{
			iter = v.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
//----------ALGORITHM------END!!!

//---------------Block-----------------------
class Block {
public:
	std::vector<double> Quan;
	Block()
	{
		this->p_graph = new int*[Vol];
		for (int i = 0;i < Vol;i++)
		{
			(this->p_graph)[i] = new int[4]; //There are 3 neighbourhood for each vetice.
		}
		Quan.resize(NObs);
		this->Calculate_Hash();
	}
	~Block() {}
	void Print_ele(std::ostream &fout)
	{
		for (int i = 0;i < Vol;i++)
		{
			fout << ele(i)<<"	";
			if (i % (Lx+1) == Lx-1 ){fout << std::endl;}
		}
	}//out put block configuration.
	int& ele(int i)
	{
		return (this->p_graph)[i][0];
	}//return p_block(i,j)
	int& adj(int i,int j)
	{
		return (this->p_graph)[i][j];
	}//return p_block(i,j)
	void Calculate_Hash()
	{
		for(int i=0;i<Lx;i++)
		{
			adj(i,1)=Vol-1-i;
			adj(i,2)=i+Lx;
			adj(i,3)=i+Lx+1;
		}
		// The first line adjacent setting

		for(int i=0;i<Ly;i++)
		{
			for(int j=0;j<Lx+1;j++)
			{
				adj(2 * i*(Lx + 1) + Lx + j, 1) = 2 * i*(Lx + 1) - 2 + (i == 0) + j; //periodic
				adj(2 * i*(Lx + 1) + Lx + j, 2) = 2 * i*(Lx + 1) - 1 + (i == 0) + j; //the the first of the previous line 
				adj(2 * i*(Lx + 1) + Lx + j, 3) = 2 * i*(Lx + 1) + 2 * Lx + 1 + j; //the next line
			}
			for(int j=0;j<Lx+1;j++)
			{
				adj(2 * i*(Lx + 1) + 2 * Lx + 1 + j, 1) = 2 * i*(Lx + 1) + 3 * Lx + 2 + j + 1 - (i == Ly - 1); //the next line
				adj(2 * i*(Lx + 1) + 2 * Lx + 1 + j, 2) = 2 * i*(Lx + 1) + 3 * Lx + 2 + j - (i == Ly-1); //periodic				
				adj(2 * i*(Lx + 1) + 2 * Lx + 1 + j, 3) = 2 * i*(Lx + 1) + Lx + j; //the the first of the previous line 
			}
		}
		for (int i = 0;i < Ly;i++)
		{
			adj(Lx + 2 * i*(Lx + 1), 1) = Vol - 1 - Lx - 2 * i*(Lx + 1);
			adj(Vol -1 -Lx - 2 * i*(Lx + 1), 1) = Lx + 2 * i*(Lx + 1);
		}
		adj(2 * Lx, 2) = Vol - 1 - 2 * Lx;
		adj(Vol - 1 - 2 * Lx, 2) = 2 * Lx;
		//the last line setting
		for(int i=Vol-Lx;i<Vol;i++)
		{
			adj(i,1)=i-Lx;
			adj(i,2)=i-Lx-1;
			adj(i,3)=Vol-i-1;
		}
	}
	void fresh()
	{
		for (int i = 0;i < Vol;i++)
		{
			this->ele(i) = abs(rn_i())%2;
		}
	}
	int fresh_0()
	{
		for (int i = 0;i < Vol;i++)
		{
			this->ele(i) = 0;
		}
		return 0;
	}
	void Calculate_Quan() 
	{
		double M;
		int sum=0;
		for (int i = 0;i < Vol;i++)
		{
			sum += this->ele(i);
		}
		M = (double)2.0 * sum / Vol - 1;
		this->Quan[0] = M;
		this->Quan[1] = M*M;
		this->Quan[2] = M*M*M*M;
		//if(M!=-1){std::cout << M << std::endl;}

	}
private:
	int **p_graph;
};



#endif
