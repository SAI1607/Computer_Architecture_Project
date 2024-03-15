/* Author: Sai Kumar, Reshwanth;                                                                                                                                                                                             
 * Description: This file defines the two required functions for the branch predictor.                                                                                                                             
*/

#include<cstdlib>
#include "predictor.h"


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

#define  global_mask  0xFFF
#define  local_mask  0xFFF
#define  pc_mask  0x3FFC
 
uint global_histbits;
uint choice;
uint pc_idx;
uint local_hist;
bool local_pred;
bool global_pred;
uint l_pred;
uint g_pred;
bool globall_prediction, locall_prediction;
bool prediction;
         
bool PREDICTOR::get_prediction(const branch_record_c* br, const op_state_c* os)
{
		  pc_idx = (pc_mask & br->instruction_addr)>>2;
      local_hist = local_mask & local_history[pc_idx];		   
	    global_histbits = global_mask & path_history;

    if(br->is_conditional)
    {   
      if(choice_prediction[global_histbits]>1){ 
        if(global_prediction[global_histbits]>1)
		    {
	            return true;
		    }
		    else{
				  return false;
		    }
		  }
		  else{
        if(local_prediction[local_hist]>3){
	          return true;
		    }
		    else{
		    	return false;
		    }
		  }
    }
    else{
	   return true;
    }
}

                                                                                                                                            
void PREDICTOR::update_predictor(const branch_record_c* br, const op_state_c* os, bool taken)
{
    pc_idx = (pc_mask & br->instruction_addr)>>2;
    local_hist = local_mask & local_history[pc_idx];                   
    global_histbits = global_mask & path_history;
	  if(br->is_conditional){	              
            if(local_prediction[local_hist]>3)
                local_pred = true;
            else
                local_pred = false;
      
            if(global_prediction[global_histbits]>1)
                global_pred= true;
            else
                global_pred = false;
     
        if(global_pred==taken && local_pred!=taken && choice_prediction[global_histbits]!=3)
            choice_prediction[global_histbits]++;
        else if(global_pred!=taken && local_pred==taken && choice_prediction[global_histbits]!=0)
            choice_prediction[global_histbits]--;
       	else
			      choice_prediction[global_histbits]=choice_prediction[global_histbits];


        if(taken)
        {
            if(global_prediction[global_histbits]!=3)
                global_prediction[global_histbits]++;
		
            if(local_prediction[local_hist]!=7)
                local_prediction[local_hist]++;
        }
        else
        {
            if(global_prediction[global_histbits]!=0)
                global_prediction[global_histbits]--;
	 
            if(local_prediction[local_hist]!=0)
                local_prediction[local_hist]--;
        }
	
        local_history[pc_idx] = ((local_history[pc_idx]<<1) | taken) & local_mask;
        path_history = ((path_history<<1) | taken) & global_mask;
    }
	else
	{
		local_history[pc_idx] = ((local_history[pc_idx]<<1) | taken) & local_mask;
		 
	}

	return;				  

}
