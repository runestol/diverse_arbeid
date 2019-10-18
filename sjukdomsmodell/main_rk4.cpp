#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;

class Population
{
public:
  int N;      // total population
  double* S;  // susceptible array
  double* I;  // infected array
  double* R;  // recovered array
  //int a;      // rate of transmission
  //int b;      // rate of recovery
  int b;      // birth rate
  float c;    // rate of immunity loss
  float d;    // death rate
  float dI;   // death rate of infected people due to disease
  float e;    // susceptibility rate, e.g. all newborns
  float f;    // vaccination

  Population() {
  }

  ~Population() {}

  void initiate(double S0, double I0, double R0, int birth_rate, float imloss_rate, float death_rate, float death_inf_rate, int steps) {
    S = new double[steps]; S[0] = S0;
    I = new double[steps]; I[0] = I0;
    R = new double[steps]; R[0] = R0;
    N = S[0] + I[0] + R[0];
    //b = recov_rate;
    b = birth_rate;
    c = imloss_rate;
    d = death_rate;
    dI = death_inf_rate;
    e = birth_rate;
  }

  double a(double t) {
    // Seasonal variation of a
    return 1.0*cos(0.05*t) + 4.0;
  }

  double dSdt(double t, double s, double i, double r) {
    return c*r - (a(t)*s*i)/N - d*s + e*N;
  }

  double dIdt(double t, double s, double i) {
    return (a(t)*s*i)/N - b*i - d*i - dI*i;
  }

  double dRdt(double i, double r) {
    return b*i - c*r - d*i;
  }
};

class RungeKutta4
{
public:
  RungeKutta4() {
  }

  void integrate(Population* X, double h, int steps) {
    /*
    Perform the RungeKutta4-method by iterating the algorithm
    for a given number of steps
    */
    double* S = X->S;
    double* I = X->I;
    double* R = X->R;

    for (int i=0; i<steps-1; i++) {
      double s_k1 = h*X->dSdt(i*h, S[i], I[i], R[i]);
      double i_k1 = h*X->dIdt(i*h, S[i], I[i]);
      double r_k1 = h*X->dRdt(I[i], R[i]);

      double s_k2 = h*X->dSdt(i*h + h/2, S[i] + s_k1/2, I[i] + s_k1/2, R[i] + s_k1/2);
      double i_k2 = h*X->dIdt(i*h + h/2, S[i] + i_k1/2, I[i] + i_k1/2);
      double r_k2 = h*X->dRdt(I[i] + r_k1/2, R[i] + r_k1/2);

      double s_k3 = h*X->dSdt(i*h + h/2, S[i] + s_k2/2, I[i] + s_k2/2, R[i] + s_k2/2);
      double i_k3 = h*X->dIdt(i*h + h/2, S[i] + i_k2/2, I[i] + i_k2/2);
      double r_k3 = h*X->dRdt(I[i] + r_k2/2, R[i] + r_k2/2);

      double s_k4 = h*X->dSdt(i*h + h, S[i] + s_k3, I[i] + s_k3, R[i] + s_k3);
      double i_k4 = h*X->dIdt(i*h + h, S[i] + i_k3, I[i] + i_k3);
      double r_k4 = h*X->dRdt(I[i] + r_k3, R[i] + r_k3);

      S[i+1] = S[i] + (s_k1 + 2*s_k2 + 2*s_k3 + s_k4)/6;
      I[i+1] = I[i] + (i_k1 + 2*i_k2 + 2*i_k3 + i_k4)/6;
      R[i+1] = R[i] + (r_k1 + 2*r_k2 + 2*r_k3 + r_k4)/6;
      //R[i+1] = X->N - S[i+1] - I[i+1];
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
  int days = 365;          // days of simulation time
  double h = 0.1;         // Step size in days
  int steps = days/h;     // number of iterations for RK4-method

  // Setting up the different populations
  Population A; A.initiate(300, 100, 0, 1, 0.5, 0.6, 1.0, steps);
  Population B; B.initiate(300, 100, 0, 2, 0.5, 0.8, 1.3, steps);
  Population C; C.initiate(300, 100, 0, 3, 0.5, 1.0, 1.6, steps);
  Population D; D.initiate(300, 100, 0, 4, 0.5, 1.2, 1.9, steps);
  Population pops[] = {A, B, C, D};

  RungeKutta4 integrator;

  char filename_ending[] = {"ABCD"};
  // Iterating over the populations
  for (int x=0; x<4; x++) {
    integrator.integrate(&pops[x], h, steps);

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
  }

  return 0;
}
