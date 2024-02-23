/* Author: Sai Kumar, Reshwanth;                                                                                                                                                                                             
 * Description: This file defines the two required functions for the branch predictor.                                                                                                                             
*/

#include "predictor.h"

int local_history[1024];
int local_prediction[1024];
int global_prediction[4096];
int choice_prediction[4096];
int path_history;


#define  global_mask  0xFFF
#define  local_mask  0x3FF
#define  pc_mask  0xFFC
 
   int global_histbits;
		   int choice;
		   int pc_idx;
		   int local_hist;
		   bool local_pred;
		   bool global_pred;
		   int l_pred;
		   int g_pred;
         
       
bool PREDICTOR::get_prediction(const branch_record_c* br, const op_state_c* os)
       {
		  // bool prediction;
		 pc_idx = (pc_mask & br->instruction_addr)>>2;
         local_hist = local_mask & local_history[pc_idx];		   
	     global_histbits = global_mask & path_history;
        choice = choice_prediction[global_histbits];
	     g_pred=global_prediction[global_histbits];
		 l_pred=local_prediction[local_hist];
           
		   
 if(choice<2)
      {
        
            if(l_pred>3){
	           return true;
		}
		else{
			return false;
		}
      }
      else
      {
         if(g_pred>1)
		 {
			return true;
		 }
		 else{
			 return false;
		 }
      }
        //return prediction;                                                                                                                                       
 }


    // Update the predictor after a prediction has been made.  This should accept                                                                                                                                  
    // the branch record (br) and architectural state (os), as well as a third                                                                                                                                     
    // argument (taken) indicating whether or not the branch was taken.                                                                                                                                            
    void PREDICTOR::update_predictor(const branch_record_c* br, const op_state_c* os, bool taken)
        {
         /*bool local_pred;
	 bool global_pred;*/
         pc_idx = (pc_mask & br->instruction_addr)>>2;
        local_hist = local_mask & local_history[pc_idx];                   
         global_histbits = global_mask & path_history;
        choice = choice_prediction[global_histbits];
		 g_pred=global_prediction[global_histbits];
		 l_pred=local_prediction[local_hist];
        	  
      
     // if(choice_prediction[global_histbits]<2){
      if(l_pred>3)
        local_pred = true;
      else
        local_pred = false;
      //}
     // else {
      if(g_pred>1)
        global_pred= true;
      else
        global_pred = false;
      //}
	    

      if(global_pred==taken && local_pred!=taken && choice_prediction[global_histbits]!=3)
       choice_prediction[global_histbits]++;
      else if(global_pred!=taken && local_pred==taken && choice_prediction[global_histbits]!=0)
        choice_prediction[global_histbits]--;


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
      return;
			  /* replace this code with your own */
                      /*  printf("%1d\n",taken);*/

        }


