#include "my_vrbls.h"
#include "my_rng.h"
#include <math.h>
#include "my_algorithm.h"
#include "my_graph.h"
#define p1 0.1
#define p2 0.4
#define p3 0.5
bool No_Leaf(Block& g,std::vector<int>& temp)
{
    int sum;
    for(int i=0;i<Vol;i++)
    {
		//std::cout << g.adj(i, 1) << "	" << g.adj(i, 2) << "	" << g.adj(i, 3) << "	" << std::endl;
		sum=temp[g.adj(i,1)]+temp[g.adj(i,2)]+temp[g.adj(i,3)];
        if(sum==1)
        {   
            return 0;
        }
    }
    return 1;
}
bool No_Leaf(Block& g)
{
	int sum;
	for (int i = 0;i<Vol;i++)
	{
		//std::cout << g.adj(i, 1) << "	" << g.adj(i, 2) << "	" << g.adj(i, 3) << "	" << std::endl;
		sum = g.ele(g.adj(i, 1)) + g.ele(g.adj(i, 2)) + g.ele(g.adj(i, 3));
		if (sum == 1)
		{
			return 0;
		}
	}
	return 1;
}

void Metropolis(Block& g)
{
    static std::vector<int> temp(Vol);
    for(int i=0;i<Vol;i++)
    {
        if(g.ele(i)=0 && exp(-Jcp)<rn())
        {
            temp[i]=g.ele(i);
        }
        else
        {
            temp[i]=-g.ele(i);   
        }
    }
    if(No_Leaf(g,temp)==1)
    {
        for(int i=0;i<Vol;i++)
        {
            g.ele(i)=temp[i];
        }
    }

}
void Random(Block& b)
{
	b.fresh();
	int a = 0;
	while (No_Leaf(b) == 0)
	{
		b.fresh();
		a++;
	}
}
void Worm(Block& b)
{
	static Graph g;
	static int u = b.fresh_0(), v = 0;
	int temp_int; 
	int d;
	do 
	{
		if (rn() < p1)
		{
			temp_int = u;
			u = v;
			v = temp_int;  //method 1
		}
		else if (rn() < p2)
		{
			temp_int = abs(rn_i() % Vol);
			if (g.degree(temp_int) != 1 && g.degree(u) != 1)
			{
				u = temp_int;
			}				//method 2
		}
		else
		{
			temp_int = b.adj(u, abs(rn_i() % 3 + 1));
			if (g.Find_Delete(temp_int, u) == 0)
			{
				d = (b.ele(temp_int) == 0) + (b.ele(u) == 0);
				if (exp(-Jcp*d) > rn())
				{
					g.Insert(u, temp_int);
					b.ele(temp_int) = 1;
					b.ele(u) = 1;
				}
			}
			else
			{
				if (g.degree(u) == 0)
				{
					b.ele(u) = 0;
				}
				if (g.degree(temp_int) == 0)
				{
					b.ele(temp_int) = 0;
				}
			}
			u = temp_int;
		}
	} while(No_Leaf(b)==0);
}

void Markov(Block& b)
{
	//Metropolis(b);
	Random(b);
	//Worm(b);
}
