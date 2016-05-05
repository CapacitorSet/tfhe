#include <cstdlib>
#include <iostream>
#include "lwe.h"
#include "lwebootstrappingkey.h"
#include "ringgsw.h"

const int basebit = 1;
const int kslength = 32/basebit;

using namespace std;

LWEBootstrappingKey::LWEBootstrappingKey(const LWEParams* in_out_params, const RingGSWParams* bk_params) {
    this->in_out_params = in_out_params;
    this->bk_params= bk_params;
    this->accum_params= bk_params->ringlwe_params;
    this->extract_params=&accum_params->extracted_lweparams;
    const int n = in_out_params->n;

    this->bk=new_RingGSWSample_array(n,this->bk_params);
    this->ks=new_LWEKeySwitchKey(basebit, kslength, extract_params, in_out_params);
}

LWEBootstrappingKey::~LWEBootstrappingKey() {
    delete_LWEKeySwitchKey(ks);
    delete_RingGSWSample_array(in_out_params->n,bk);
}



LWEBootstrappingKeyFFT::LWEBootstrappingKeyFFT(const LWEParams* in_out_params, const RingGSWParams* bk_params) {
    this->in_out_params = in_out_params;
    this->bk_params= bk_params;
    this->accum_params= bk_params->ringlwe_params;
    this->extract_params=&accum_params->extracted_lweparams;
    const int n = in_out_params->n;

    this->bk=new_RingGSWSampleFFT_array(n,this->bk_params);
    this->ks=new_LWEKeySwitchKey(basebit, kslength, extract_params, in_out_params);
}

LWEBootstrappingKeyFFT::~LWEBootstrappingKeyFFT() {
    delete_LWEKeySwitchKey(ks);
    delete_RingGSWSampleFFT_array(in_out_params->n,bk);
}



 






