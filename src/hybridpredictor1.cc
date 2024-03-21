//Hybrid Predictor ////
#include"predictor.h"

uint local_prediction[4096];
uint global_prediction[8192];
uint choice_prediction[4096];
uint path_history;

//initialization
void initialization(){

	for(uint j=0; j<4096;j++)
	{
		local_prediction[j]=0;
	}
	for(uint k=0; k<8192; k++)
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

#define global_mask 0x1FFF
#define local_mask 0x3FF
#define pcmask 0x3FFC


bool PREDICTOR::get_prediction(const branch_record_c* br, const op_state_c* os){
    pcbits = (br->instruction_addr & global_mask)>>2;
    histbits = path_history & global_mask;
    global_idx = histbits ^ pcbits;
    pcidx = (pcmask & br->instruction_addr)>>2;
	if(br->is_conditional)
	{
		if(choice_prediction[pcidx]<2)
		{
			if(local_prediction[pcidx]>1)
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
	if(br->is_conditional){
		if(local_prediction[pcidx]>1)
			local_pred = true;
		else
			local_pred = false;
	
		if(global_prediction[global_idx]>1)
			global_pred = true;
		else
			global_pred = false;

		if(global_pred==taken && local_pred!=taken && choice_prediction[pcidx]!=3)
			choice_prediction[pcidx]++;
		else if(global_pred!=taken && local_pred==taken && choice_prediction[pcidx]!=0)
			choice_prediction[pcidx]--;
		
		
		if(taken)
		{
			if(global_prediction[global_idx]!=3)
				global_prediction[global_idx]++;
			if(local_prediction[pcidx]!=3)
				local_prediction[pcidx]++;
		}
		else
		{
			if(global_prediction[global_idx]!=0)
				global_prediction[global_idx]--;
			if(local_prediction[pcidx]!=0)
				local_prediction[pcidx]--;
		}
		path_history = (path_history<<1 | taken) & global_mask;
	}
	else
		path_history = (path_history<<1 | taken) & global_mask;
    return;
}
