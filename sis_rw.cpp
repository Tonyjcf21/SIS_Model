#include<iostream>
#include<cstdlib>
#include<fstream>
#include<iomanip>
#include<cmath>
#include<time.h>
#include<ctime>
#include<random>
#include<thread>
#include<sstream>
//#include<graphics.h> buscar alternativa
#include "gnuplot.h"

using namespace std ;

// Intento arreglar los outputs. En esta version automatizo un poco la generacion de output files. 
// Usar avgresults.sh para calcular el promedio y promedio cuadrado de las columnas en el output.

#define TAU 6.283185307

float beta;
int betastr;

class Person
{
	public:
		int x,y; //position
		int status; //0=S, 1=I, 2=D
		float recovery_time;
		int dead_time;

		Person()
		{
			status = 0;
			recovery_time = 10;
			dead_time = 2;
		}
};

void tostr(float);

int main(){

	srand(time(0));

	ifstream fileIn; //input-stream variable
	
	//these values are just references. Let's see if they change when reading the input file.
	//float angle, t = 0, dt = 0.05, flag, vel;
	//int tf = 150, radius = 2;
	//float beta=0.8;
	//int n = 500; //number of nodes or people
	//int BoxX = 150, BoxY = 150; //Dimensions of the grid.
	int n_S=0,n_I=0,n_D=0;//number of total S,I,D.
	int status_for_switch;
	float angle, t=0,dt,flag, vel;
	int tf, radius, n, BoxX, BoxY, num_sim;
	float beta;
	//Person P[2*n], tmp[2*n];

	//cout << "Write the name of the file you want to open: ";
	//cin >> inputfilename;

	fileIn.open("datin.txt");
	if(!fileIn)
	{
		cout << "Couldn't find the input file" << endl;
		exit(-1);
	}

	fileIn >> num_sim >> beta >> n >> BoxX >> BoxY >> tf >> dt >> radius;

	fileIn.close();

	cout << "simulaciones: " << num_sim << ", "
		<< "beta: " << beta << ", "
		<< "N: " << n << ", "
		<< "BoxX: " << BoxX << ", "
		<< "BoxY: " << BoxY << ", "
		<< "tf: " << tf << ", "
		<< "dt: " << dt << ", "
		<< "radius: " << radius << ", " << endl;

	tostr(beta);

	for (int rw = 1; rw <= num_sim; ++rw)
	{
		Person P[2*n], tmp[2*n]; // Have to initialize here, inside the rw loop.
		                         // in order to start generate different configurations every time.
	//=============================================================================
	//Generating the initial position for the walkers
		for(int i=0; i<n; i++)
		{
			tmp[i].x = int(rand())%BoxX;
			tmp[i].y = int(rand())%BoxY;
			n_S ++;
		}

		//Infect 1% of the population. 1% is too much.
		for(int i=0; i<5; i++) //(n/100)
		{
			tmp[i].status = 1;
			n_S --;
			n_I ++;
		}

		std::string output1 = "timeseries_" + std::to_string(0) + std::to_string(betastr) + "_" + std::to_string(rw);
		std::ofstream qq(output1);
		cout << output1 << endl;
		/*
		output1 = "timeseries" + to_string(0) + to_string(betastr);
		output2 = "IvsS" + to_string(0) + to_string(betastr);
		qq.open(output1);
		pp.open(output2);
		*/
		while(t<tf)
		{
			n_S = 0, n_I = 0, n_D = 0;

			for (int i = 0; i < n; ++i)
			{
				P[i] = tmp[i];
				status_for_switch = P[i].status;
				/*
				if (status_for_switch == 1)
				{
					cout << "P[i]: " << P[i].status << "    " << "ss: " << status_for_switch << endl;
				}
				*/
				switch(status_for_switch){
					case 0:
					n_S ++;
					break;
					case 1:
					n_I ++;
					break;
					case 2:
					n_D ++;
					break;
				}
			}
			//=============================================================================
			//print stats

			//in case of percentages --> t,n_S*100.0/n,n_I*100.0/n,n_D*100.0/n
			qq << t << " " << n_S << " " << n_I << " " << n_D << endl;
			//timeseries_sis2 << n_S << " " << n_I << endl;
			//pp << n_S << " " << n_I << endl;
			//=============================================================================
			//update status and positions
			if(t > 5){
			for (int i = 0; i < n; ++i)
			{
				status_for_switch = P[i].status;

				//cout << "status: " << status_for_switch << endl;

				switch(status_for_switch){
					case 0:
					//check for infections
					for (int j = 0; j < n; ++j)
					{
						if ((i!=j) && (P[j].status==1) && (sqrt((P[i].x-P[j].x)*(P[i].x-P[j].x) + (P[i].y-P[j].y)*(P[i].y-P[j].y)) < radius))
						{
							flag = (float)rand()/RAND_MAX;
							
							if(flag < beta){
								tmp[i].status = 1; //supposing the chances of getting the infection are X% when in contact.
								//cout << "se infecto" << endl;
							}
						}
					}
					vel = (rand() % 15);
					angle = TAU*rand()/RAND_MAX;
					tmp[i].x = (int)(BoxX + P[i].x + vel*cos(angle)) % BoxX;
					tmp[i].y = (int)(BoxY + P[i].y + vel*sin(angle)) % BoxY;
					break;
					case 1:
					flag = (float)rand()/RAND_MAX;
					
					if (flag > 0.97)
					{
						tmp[i].status = 2;
						tmp[i].recovery_time = 15;
					}
					else
					{
						tmp[i].recovery_time -= dt;
					}

					if(P[i].recovery_time <= 0){
						tmp[i].status = 0;
					}
					vel = (rand() % 15);
					angle = TAU*rand()/RAND_MAX;
					tmp[i].x = (int)(BoxX + P[i].x + vel*cos(angle)) % BoxX;
					tmp[i].y = (int)(BoxY + P[i].y + vel*sin(angle)) % BoxY;
					break;
					case 2:
					tmp[i].dead_time -= dt;

					if (P[i].dead_time <= 0)
					{
						tmp[i].status = 0;
						tmp[i].x = int(rand())%BoxX;
						tmp[i].y = int(rand())%BoxY;
					}
					break;
				}
			}
			}
			/*
			//=============Code to output a different file for every Step.===========	
			// Used to plot the position of particles for every monte carlo step and watch the spread.
			std::string filename = "prueba" + std::to_string(t);//Open a new file for every step.
	    	std::ofstream file(filename);

			for(int k = 0;k<n;k++){ //Trying to print every walker after a step.
				file << tmp[k].x << " " << tmp[k].y << " " << tmp[k].status << endl;
			}
			file.close();
			//-----------------------------------------------------------------------
			*/
			t += dt;
		}
		qq.close();
		t = 0;
	}
	
	
	gnuplot h;
	h("set terminal pdfcairo");
	h("set output \"IvsS08.pdf\" ");
	h("plot 'IvsS08' u 1:2 w l");
	

	gnuplot p;
	p("set terminal pdfcairo");
	p("set output \"timeseries08.pdf\" ");
	//p("plot 'timeseries_sis2' u 1:2 w l");
	p("plot 'timeseries08' u 1:2 w l title 'S', 'timeseries08' u 1:3 w l title 'I', 'timeseries08' u 1:4 w l title 'D'");
	return 0;
}

void tostr(float beta){
	betastr = int(beta*10);
}
//--------------------------------------------------------
//gnuplot -e 'set terminal png size 900,800; set output "rw1.png"; plot "timeseries_sis2" u 1:2' 
//gnuplot --persist -e 'plot "timeseries_sis2" u 1:2'
//fit a*x+b "R2vsN"  u (log($1)):(log($2)) via a,b
//plot a*x+b,"R2vsN"  u (log($1)):(log($2)) w e

