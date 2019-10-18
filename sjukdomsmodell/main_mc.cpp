#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <random>
#include "lib.h"

using namespace std;

class Population
{
public:
  int N;      // total population
  double* S;  // susceptible array
  double* I;  // infected array
  double* R;  // recovered array
  int a;      // rate of transmission
  int b;      // rate of recovery
  float c;    // rate of immunity loss

  Population() {
  }

  ~Population() {}

  void initiate(double S0, double I0, double R0, int transm_rate,
                int recov_rate, float imloss_rate, int steps) {
    S = new double[steps]; S[0] = S0;
    I = new double[steps]; I[0] = I0;
    R = new double[steps]; R[0] = R0;
    N = S[0] + I[0] + R[0];
    a = transm_rate;
    b = recov_rate;
    c = imloss_rate;
  }

  double dSdt(double s, double i, double r) {
    return c*r - (a*s*i)/N;
  }

  double dIdt(double s, double i) {
    return (a*s*i)/N - b*i;
  }

  double dRdt(double s, double i, double r) {
    return b*i - c*r;
  }
};

// Methods in this class is largely from Piazza
class MonteCarlo
{
public:
  double dt_;

  MonteCarlo(Population* X) {
    // Calculate sufficiently small time step
    dt_ = 1.0;
    if (4.0/(X->a*X->N) < dt_) dt_ = 4.0/(X->a*X->N);
    if (1.0/(X->b*X->N) < dt_) dt_ = 1.0/(X->b*X->N);
    if (1.0/(X->c*X->N) < dt_) dt_ = 1.0/(X->c*X->N);
  }

  void solve(Population* X, string filename, int nsamples, double tf) {

    mt19937 generator;
    uniform_real_distribution<double> rand01(0.0, 1.0);

    vector<double> time;
    for (double t=0.0; t<tf; t+=dt_) time.push_back(t);
    int ntimes = time.size();

    vector<double> avgS(ntimes, 0.0), avgI(ntimes, 0.0), avgR(ntimes, 0.0);

    // Create random samples
    for (int n=0; n<nsamples; ++n) {
      ofstream outfile;
      outfile.open(filename+to_string(n)+".dat");
      cout << "write to ---> " << "'" << filename+to_string(n)+".dat'" << endl;

      int S = X->S[0];
      int I = X->I[0];
      int R = X->R[0];

      for (int i=0; i<ntimes; ++i) {
        // Calculate averages
        avgS[i] = S/(double) nsamples;
        avgI[i] = I/(double) nsamples;
        avgR[i] = R/(double) nsamples;

        // keep-or-reject
        if (rand01(generator) < X->a*S*I*dt_/X->N) {I+=1; S-=1;}
        if (rand01(generator) < X->b*I*dt_) {R+=1; I-=1;}
        if (rand01(generator) < X->c*R*dt_) {S+=1; R-=1;}
      }
    }

    double S, I, R, N;
    double avg_sigS = 0.0, avg_sigI = 0.0, avg_sigR = 0.0;
    vector<double> varS(ntimes, 0.0);
    vector<double> varI(ntimes, 0.0);
    vector<double> varR(ntimes, 0.0);
    int count = 0;

    // Calculate variances
    for (int n=0; n<nsamples; ++n) {
      ifstream infile;
      infile.open(filename + to_string(n) + ".dat");

      // Calculate variance and average standard deviation
      for (int i=0; i<ntimes; ++i) {
        infile >> S >> I >> R >> N;

        varS[i] += (avgS[i]-S)*(avgS[i]-S)/(nsamples-1);
        varI[i] += (avgI[i]-I)*(avgI[i]-I)/(nsamples-1);
        varR[i] += (avgR[i]-R)*(avgR[i]-R)/(nsamples-1);

        avg_sigS += sqrt(varS[i])/(nsamples*ntimes);
        avg_sigI += sqrt(varI[i])/(nsamples*ntimes);
        avg_sigR += sqrt(varR[i])/(nsamples*ntimes);
      }
      infile.close();
    }

    for (int n=0; n<nsamples; ++n) {
      outfile.open(filename+to_string(n)+".dat");
      for (int i=0; i<ntimes; ++i) {
        outfile << setw(15) << setprecision(8) << varS[i];
        outfile << setw(15) << setprecision(8) << varI[i];
        outfile << setw(15) << setprecision(8) << varR[i] << endl;;
        //outfile << setw(15) << setprecision(8) << avg_sigS[i];
        //outfile << setw(15) << setprecision(8) << avg_sigI[i];
        //outfile << setw(15) << setprecision(8) << avg_sigR[i] << endl;
      }
    }
  }
};


int main(int argc, char* argv[])
{
  // Reading output filename from command line
  ofstream ofile;
  string filename;
  if (argc<=1) {
    cout << "Please provide name of output file with no ending:" << endl;
    cin >> filename;
  } else {
    filename = argv[1];
  }

  // Defining time parameters
  int days = 15;          // days of simulation time
  double h = 0.1;         // Step size in days
  int steps = days/h;     // number of iterations for RK4-method

  // Setting up the different populations
  Population A; A.initiate(300, 100, 0, 4, 1, 0.5, steps);
  Population B; B.initiate(300, 100, 0, 4, 2, 0.5, steps);
  Population C; C.initiate(300, 100, 0, 4, 3, 0.5, steps);
  Population D; D.initiate(300, 100, 0, 4, 4, 0.5, steps);
  Population pops[] = {A, B, C, D};

  int nsamples;
  cout << "Provide number of samples:" << endl;
  cin >> nsamples;

  char filename_ending[] = {"ABCD"};
  // Iterating over the populations
  for (int x=0; x<4; x++) {
    MonteCarlo solver(&pops[x]);
    solver.solve(&pops[x], filename+filename_ending[x]+"_", nsamples, days);
    /*
    // Print results to output file
    string outfile = filename + filename_ending[x] + ".dat";
    ofile.open(outfile);
    for (int i=0; i<steps; i++) {
      ofile << setw(15) << pops[x].S[i];
      ofile << setw(15) << pops[x].I[i];
      ofile << setw(15) << pops[x].R[i];
      ofile << setw(15) << pops[x].S[i] + pops[x].I[i] + pops[x].R[i] << endl;
    }
    ofile.close();
    */
  }

  return 0;
}
