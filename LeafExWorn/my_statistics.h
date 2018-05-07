#include <iostream>
#include <fstream>
#include"my_vrbls.h"
#include<string.h>
#ifndef  __MY_STA__
#define __MY_STA__
/*If you want to sample more quantities
It is very important to correct class Data:: Quan[n]

*/
//-----------Class Data------------
class Data
{
public:
	Data() {}
	Data(double);
	~Data() {};
	double Quan[4];
};
//----------------------------------
extern char* OutFile;
extern char* InFile;
extern void Get_Parameters(std::ifstream&);
extern void Get_Parameters(std::istream&);
extern void write2file(std::ostream&);
extern std::vector<Data> Sample_Obs;
extern std::vector<Data> Block_Obs;
extern void Collect_data(Block&, int);
extern void Normalize_data(int);
extern void Analyze_data();
#endif // ! __MY_IO__
