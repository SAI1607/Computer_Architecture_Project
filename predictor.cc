File Edit Options Buffers Tools C++ Help                                                                                                                                                                           
/* Author: Mark Faust;                                                                                                                                                                                            \
                                                                                                                                                                                                                   
 * Description: This file defines the two required functions for the branch predictor.                                                                                                                            \
                                                                                                                                                                                                                   
*/

#include "predictor.h"

/*int local_history[0:2][0:1023];                                                                                                                                                                                  
int local_prediction[0:2][0:1023];                                                                                                                                                                                 
int global_prediction[0:1][0:4095];                                                                                                                                                                                
int choice_prediction[0:1][0:4095];                                                                                                                                                                                
int path_history[0:11];                                                                                                                                                                                            
*/
/*int pathhistory_bits; // Number of bits used for Global History                                                                                                                                                  
int localhistory_bits; // Number of bit used for Local History                                                                                                                                                     
int pcIndex_bits;  // Number of bits used for PC index*/

uint32_t global_mask;
uint32_t local_mask;
uint32_t pc_mask;

uint32_t local_history[1024];
uint32_t local_prediction[1024];
uint32_t global_prediction[4096];
uint32_t choice_prediction[4096];
uint32_t path_history;


/*uint32_t masking(uint32_t size)                                                                                                                                                                                  
{                                                                                                                                                                                                                  
  uint32_t mask = 0;                                                                                                                                                                                               
  uint32_t i;                                                                                                                                                                                                      
  for(i=0;i<size;i++)                                                                                                                                                                                              
  {                                                                                                                                                                                                                
    mask = mask | 1<<i;                                                                                                                                                                                            
  }                                                                                                                                                                                                                
  return mask;                                                                                                                                                                                                     
}*/


 global_mask = 0x3;
 local_mask = 0x7;
 pc_mask= 0x3FF<<2;
      for(int i=0;i<1024;i++)
      {
        local_history[i] = 0;
      }
      // Local Predictor                                                                                                                                                                                           

      for(int i=0;i<1024;i++)
      {
        local_prediction[i] = 0;
      }
      // Global Predictor                                                                                                                                                                                          

      for(int i=0;i<4096;i++)
-UU-:----F1  predictor.cc   Top L1     (C++//l Abbrev) ------------------------------------------------------------------------------------------------------------------------------------------------------------

