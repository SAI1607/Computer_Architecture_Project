//HYBRID PREDICTOR//
#include"predictor.h"

uint local_history[1024];
uint local_prediction[2048];
uint global_prediction[4096];
uint choice_prediction[4096];
uint path_history;

//initialization
void initialization(){

	for(uint i=0; i<1024; i++)
	{
		local_history[i]=0;
	}
	for(uint j=0; j<2048; j++)
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
uint global_idx;
uint pcidx;
uint local_idx;

bool local_pred;
bool global_pred;

#define global_mask 0xFFF
#define local_mask 0x7FF
#define pcmask 0xFFC


bool PREDICTOR::get_prediction(const branch_record_c* br, const op_state_c* os){
    pcbits = (br->instruction_addr & global_mask)>>2;
    histbits = path_history & global_mask;
    global_idx = histbits ^ pcbits;
	pcidx = (pcmask & br->instruction_addr)>>2;
    local_idx = local_mask & local_history[pcidx];
	if(br->is_conditional)
	{
		if(choice_prediction[global_idx]<2)
		{
			if(local_prediction[local_idx]>3)
				return true;
			else
				return false;
		}
		else
		{
			if(global_prediction[global_idx]>1)
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
    global_idx = histbits ^ pcbits;
    pcidx = (pcmask & br->instruction_addr)>>2;
    local_idx = local_mask & local_history[pcidx];
	if(br->is_conditional){
		if(local_prediction[local_idx]>3)
			local_pred = true;
		else
			local_pred = false;
	
		if(global_prediction[global_idx]>1)
			global_pred = true;
		else
			global_pred = false;

		if(global_pred==taken && local_pred!=taken && choice_prediction[global_idx]!=3)
			choice_prediction[global_idx]++;
		else if(global_pred!=taken && local_pred==taken && choice_prediction[global_idx]!=0)
			choice_prediction[global_idx]--;
		else 
			choice_prediction[global_idx]=choice_prediction[global_idx];
		
		if(taken)
		{
			if(global_prediction[global_idx]!=3)
				global_prediction[global_idx]++;
			if(local_prediction[local_idx]!=7)
				local_prediction[local_idx]++;
		}
		else
		{
			if(global_prediction[global_idx]!=0)
				global_prediction[global_idx]--;
			if(local_prediction[local_idx]!=0)
				local_prediction[local_idx]--;
		}
		local_history[pcidx] = (local_history[pcidx]<<1 | taken) & local_mask;
		path_history = (path_history<<1 | taken) & global_mask;
	}
	else
		path_history = (path_history<<1 | taken) & global_mask;
    return;
}
