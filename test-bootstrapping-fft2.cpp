#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include "lwe.h"
#include "multiplication.h"
#include "polynomials.h"
#include "lwesamples.h"

using namespace std;



// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************


void dieDramatically(string message) {
    cerr << message << endl;
    abort();
}

EXPORT void ringLweExtractKey(LWEKey* result, const RingLWEKey* key); //TODO: change the name and put in a .h
EXPORT void createBootstrappingKeyFFT(
	LWEBootstrappingKeyFFT* bk, 
	const LWEKey* key_in, 
	const RingGSWKey* rgsw_key);
EXPORT void bootstrapFFT(LWESample* result, const LWEBootstrappingKeyFFT* bk, Torus32 mu1, Torus32 mu0, const LWESample* x);

int main(int argc, char** argv) {
    //const int count = 1000; //number of tests to compare the 3 types of multiplication

    const int N = 1024;
    const int k = 1;
    const int n = 500;
    const int l_bk = 3;
    const int Bgbit_bk = 10;
    //const int ks_basebit = 4;
    const int alpha_in = 1./500.;
    const int alpha_bk = 1e-9;
    //const int alpha_ks = 1e-6;

    LWEParams* params_in = new_LWEParams(n, alpha_in, 1./16.);
    RingLWEParams* params_accum = new_RingLWEParams(N, k, alpha_bk, 1./16.);
    RingGSWParams* params_bk = new_RingGSWParams(l_bk, Bgbit_bk, params_accum);

    LWEKey* key = new_LWEKey(params_in);
    lweKeyGen(key);

    RingGSWKey* key_bk = new_RingGSWKey(params_bk);
    ringGswKeyGen(key_bk);

    LWEBootstrappingKeyFFT* bk = new_LWEBootstrappingKeyFFT(params_in, params_bk);
    createBootstrappingKeyFFT(bk, key, key_bk);

    LWESample* test = new_LWESample(params_in);
    LWESample* test_out = new_LWESample(params_in);
    
    const Torus32 mu1 = modSwitchToTorus32(1,2);
    const Torus32 mu0 = modSwitchToTorus32(0,2);

    Torus32 mu_in = modSwitchToTorus32(1,2);
    lweSymEncrypt(test, mu_in, alpha_in, key);
    
    cout << "starting bootstrapping..." << endl;

    clock_t begin = clock();
    bootstrapFFT(test_out, bk, mu1, mu0, test);
    clock_t end = clock();
    cout << "finished bootstrapping... " << end-begin << endl;
    Torus32 mu_out = lweSymDecrypt(test_out, key, 2);
    cout << "end_variance: " << test_out->current_variance << endl;

    if (mu_in != mu_out) dieDramatically("et Zut!");

    return 0;
}
