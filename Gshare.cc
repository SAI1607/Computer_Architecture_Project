/*//GSHARE Predictor///

#include<cstdlib>
#include "predictor.h"


// Handy Global for use in output routines
uint ghistoryBits; // Number of bits used for Global History
uint lhistoryBits; // Number of bits used for Local History
uint pcIndexBits;  // Number of bits used for PC index
uint bpType;       // Branch Prediction Type
uint verbose;

uint gs_pht[1024];
uint ghist;
uint gmask;

uint pcbits;
uint histbits;
uint indexxx;
uint prediction;

#define gmask  0xFFC


bool PREDICTOR::get_prediction(const branch_record_c* br, const op_state_c* os)
{
	pcbits = (br->instruction_addr & gmask)>>2;
    histbits = ghist & gmask;
    indexxx = histbits ^ pcbits;
  
    if(br->is_conditional)
	{
      
      //printf("Index: %u\n", index);
      prediction = gs_pht[indexxx];
      if(prediction>1)
        return true;
      else
        return false;
	}
	else{
    return true;
	}
}
void PREDICTOR::update_predictor(const branch_record_c* br, const op_state_c* os, bool taken)
{
 
	  if(br->is_conditional){
      pcbits = (br->instruction_addr & gmask)>>2;
      histbits = ghist & gmask;
      indexxx = histbits ^ pcbits;
     
      if(taken)
      {
        if(gs_pht[indexxx]!=3)
          gs_pht[indexxx]++;
      }
      else
      {
        if(gs_pht[indexxx]!=0)
          gs_pht[indexxx]--;
      }
      ghist = ((ghist<<1) | taken);
  }
	  else{
    ghist = ((ghist<<1) | taken);
	  }
  return;
}
