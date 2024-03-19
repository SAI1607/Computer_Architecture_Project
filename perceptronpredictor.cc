// A Perceptron Based predictor.

#include "predictor.h"

// Number of weights
#define N 28


// Total budget of the predictor
#define Budget (32800)

// Threshold values
#define ThetaMax (N*1.93+14)
#define ThetaMin ( -1*ThetaMax )

// Number of bits in a weight
#define BitsInWeight 8
#define MAXVAL 127
#define MINVAL -128

// calculation of entries of perceptron table
#define SizeOfPerceptron ((N+1)*BitsInWeight)
#define NumberPerceptron 128

int perceptron[NumberPerceptron][N];
int bias[NumberPerceptron];
int GHR;
int indexxx;
int y;
bool prediction;

void init_predictor ()
{
  int i,j;
  for(i=0; i<NumberPerceptron; i++)
	for (j=0 ; j <= N ; j++)
		perceptron[i][j] = 0;
  GHR = 0; 
}

bool PREDICTOR::get_prediction(const branch_record_c* br, const op_state_c* os)
{
  int i,j,x;
  prediction = false;

  // hash function to map pc to a row in the perceptron table
  indexxx = br->instruction_addr % NumberPerceptron;
  j = 1;
  y = bias[indexxx];
  for(i=0; i < N; i++){
	  // checking jth bit of GHR 
	  if((GHR&j) == 0) 
		x = -1;
	  else
		x = 1;

	  // dot product calculation
	  y += perceptron[indexxx][i]*x;

	  // used in finding out jth bit of GHR above
	  j = j << 1;
	}

  // making a prediction
  if( y >= 0 )
	prediction = true;
  else
	prediction = false;

 return prediction;
}

void PREDICTOR::update_predictor(const branch_record_c* br, const op_state_c* os, bool taken)
{
 int i,j,x,t;

 // t used to update bias and numerical outcome is used to update GHR
 if(taken){
	t = 1;
 }
 else{
	t = -1;
 }
 
 j = 1;

 // updating weights and bias
 if( taken != prediction || (y < ThetaMax && y > ThetaMin) ){

	// checking whether bias is within a value that is 8 bit long
	if ( bias[indexxx] > MINVAL && bias[indexxx] < MAXVAL) {
		// updating bias
		bias[indexxx] = bias[indexxx] + t;
	}

	for(i=0; i < N; i++){	

		// checking whether correlation is positive or negative
		if( ( ( (GHR&j) != 0) && taken ) ||  ( ((GHR&j) == 0) && !taken ) ) 
			x = 1;
		else
			x = -1;

		// checking whether ith weight at index is within a value that is 8 bit long
		if ( perceptron[indexxx][i] > MINVAL && perceptron[indexxx][i] < MAXVAL ) {
			// updating weight 
			perceptron[indexxx][i] = perceptron[indexxx][i] + x;
		}

		// j used in finding out jth bit of GHR
		j = j << 1;
	}

 }
 
 // updating .. we take care of number of bits in GHR by doing an & with j everywhere we use GHR
 // in any case since it is an int it is just 32 bits well under 256 bits reserved for GHR
 GHR = GHR << 1 | taken;
}
