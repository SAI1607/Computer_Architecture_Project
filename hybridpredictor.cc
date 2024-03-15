//Hybrid Predictor ////

#include"predictor.h"

uint local_history[4096];
uint local_prediction[4096];
uint global_prediction[4096];
uint choice_prediction[4096];
uint path_history;

//initialization
void initialization(){

	for(uint i=0; i<4096; i++)
	{
		local_history[i]=0;
	}
	for(uint j=0; j<4096; j++)
	{
		local_prediction[j]=0;
	}
	for(uint k=0; k<4096; k++)
	{
		global_prediction[k]=0;
	}
	for(uint l=0; l<4096; l++)
	{
		choice_prediction[l]=0;
	}
	path_history=0;
	return;
}

uint pcbits;
uint histbits;
uint indexxx;
uint pcidx;
uint lhist;

bool local_pred;
bool global_pred;

#define global_mask 0xFFF
#define local_mask 0xFFF
#define pcmask 0x3FFC


bool PREDICTOR::get_prediction(const branch_record_c* br, const op_state_c* os){
    pcbits = (br->instruction_addr & global_mask)>>2;
    histbits = path_history & global_mask;
    indexxx = histbits ^ pcbits;
	pcidx = (pcmask & br->instruction_addr)>>2;
    lhist = local_mask & local_history[pcidx];
	if(br->is_conditional)
	{
		if(choice_prediction[indexxx]<2)
		{
			if(local_prediction[lhist]>3)
				return true;
			else
				return false;
		}
		else
		{
			if(global_prediction[indexxx]>1)
				return true;
			else
				return false;
		}
	}
	else
		return true;
}

void PREDICTOR::update_predictor(const branch_record_c* br, const op_state_c* os, bool taken)
{
	pcbits = (br->instruction_addr & global_mask)>>2;
    histbits = path_history & global_mask;
    indexxx = histbits ^ pcbits;
    pcidx = (pcmask & br->instruction_addr)>>2;
    lhist = local_mask & local_history[pcidx];

      
	if(br->is_conditional){
		if(local_prediction[lhist]>3)
			local_pred = true;
		else
			local_pred = false;
	
		if(global_prediction[indexxx]>1)
			global_pred = true;
		else
			global_pred = false;

		if(global_pred==taken && local_pred!=taken && choice_prediction[indexxx]!=3)
			choice_prediction[indexxx]++;
		else if(global_pred!=taken && local_pred==taken && choice_prediction[indexxx]!=0)
			choice_prediction[indexxx]--;
		if(taken)
		{
			if(global_prediction[indexxx]!=3)
				global_prediction[indexxx]++;
			if(local_prediction[lhist]!=7)
				local_prediction[lhist]++;
		}
		else
		{
			if(global_prediction[indexxx]!=0)
				global_prediction[indexxx]--;
			if(local_prediction[lhist]!=0)
				local_prediction[lhist]--;
		}
		local_history[pcidx] = local_history[pcidx]<<1 | taken;
		path_history = path_history<<1 | taken;
	}
	else
		local_history[pcidx] = local_history[pcidx]<<1 | taken;
    return;
}
