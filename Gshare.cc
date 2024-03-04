int gs_pht[4096];
int ghist;
int lhist;
int gmask;

int pcbits;
int histbits;
int indexxx;
int prediction;

#define gmask  0xFFF;


bool PREDICTOR::get_prediction(const branch_record_c* br, const op_state_c* os)
{
	pcbits = br->instruction_addr & gmask;
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
      pcbits = br->instruction_addr & gmask;
      histbits = ghist & gmask;
      indexxx = histbits ^ pcbits;
     
      if(taken)
      {
        if(gs_pht[indexxx]<3)
          gs_pht[indexxx]++;
      }
      else
      {
        if(gs_pht[indexxx]>0)
          gs_pht[indexxx]--;
      }
      ghist = ((ghist<<1) | taken);
  }
  else
  {
	  
    ghist = ((ghist<<1) | taken);
  }
}
