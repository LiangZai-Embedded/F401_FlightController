#include "filter.h"

BiquadFliter_t lpf_ax,lpf_ay,lpf_az,lpf_gx,lpf_gy,lpf_gz;

/**
  * @brief  biquad filter initialization 
  * @param  none
  * @retval none
  * @date   2021-03-04
  */
void biquad_filter_init(void)
{
    memset(&lpf_ax,0,sizeof(BiquadFliter_t));
    memset(&lpf_ay,0,sizeof(BiquadFliter_t));
    memset(&lpf_az,0,sizeof(BiquadFliter_t));
    memset(&lpf_gx,0,sizeof(BiquadFliter_t));
    memset(&lpf_gy,0,sizeof(BiquadFliter_t));
    memset(&lpf_gz,0,sizeof(BiquadFliter_t));

	  lpf_ax.coeff.gain = lpf_ay.coeff.gain = lpf_az.coeff.gain = 0.0078202080334971915f;
    lpf_ax.coeff.A[0] = lpf_ay.coeff.A[0] = lpf_az.coeff.A[0] = 1.0f;
    lpf_ax.coeff.A[1] = lpf_ay.coeff.A[1] = lpf_az.coeff.A[1] = -1.7347257688092752f;
    lpf_ax.coeff.A[2] = lpf_ay.coeff.A[2] = lpf_az.coeff.A[2] = 0.76600660094326389f;
    lpf_ax.coeff.B[0] = lpf_ay.coeff.B[0] = lpf_az.coeff.B[0] = 1.0f;
    lpf_ax.coeff.B[1] = lpf_ay.coeff.B[1] = lpf_az.coeff.B[1] = 2.0f;
    lpf_ax.coeff.B[2] = lpf_ay.coeff.B[2] = lpf_az.coeff.B[2] = 1.0f;
	
	  lpf_gx.coeff.gain = lpf_gy.coeff.gain = lpf_gz.coeff.gain = 0.0036216815149286421f;
    lpf_gx.coeff.A[0] = lpf_gy.coeff.A[0] = lpf_gz.coeff.A[0] = 1.0f;
    lpf_gx.coeff.A[1] = lpf_gy.coeff.A[1] = lpf_gz.coeff.A[1] = -1.8226949251963083f;
    lpf_gx.coeff.A[2] = lpf_gy.coeff.A[2] = lpf_gz.coeff.A[2] = 0.83718165125602262f;
	  lpf_gx.coeff.B[0] = lpf_gy.coeff.B[0] = lpf_gz.coeff.B[0] = 1.0f;
    lpf_gx.coeff.B[1] = lpf_gy.coeff.B[1] = lpf_gz.coeff.B[1] = 2.0f;
    lpf_gx.coeff.B[2] = lpf_gy.coeff.B[2] = lpf_gz.coeff.B[2] = 1.0f;
}



/**
  * @brief  biquad filter
  * @param  filter: filter structure ptr
  *         axis6_out: 6axis output structure  ptr
  * @retval filter result
  * @date   2021-03-04
  */
float biquad_filter(BiquadFliter_t* filter,float input)
{
  float A[3] = {0.0f}; 
  float B[3] = {0.0f};

  float x[3] = {0.0f}; 
  float y[3] = {0.0f};


  float gain = filter->coeff.gain;

  for(uint8_t i=0;i<3;i++)
  {
    A[i] = filter->coeff.A[i];
    B[i] = filter->coeff.B[i];
		x[i] = filter->x[i];
		y[i] = filter->y[i];
	}
  
	
	x[2] = x[1];
	x[1] = x[0];
	y[2] = y[1];
	y[1] = y[0];
	
	x[0] = input;
	y[0] = (gain*(B[0]*x[0]+B[1]*x[1]+B[2]*x[2]) - A[1]*y[1] - A[2]*y[2])/A[0];  
	

  for(uint8_t i=0;i<3;i++)
  {
    filter->x[i] = x[i];
    filter->y[i] = y[i];
	}
	
	return y[0];
}



/**
  * @brief  biquad filter application
  * @param  axis6_in: accelermeter structure input
  *         axis6_out: accelermeter structure output
  * @retval none
  * @date   2021-03-04
  */
void biquad_filter_apply(Axis6_t* axis6_in, Axis6_t* axis6_out)
{
	axis6_out->ax = biquad_filter(&lpf_ax,axis6_in->ax);
  axis6_out->ay = biquad_filter(&lpf_ay,axis6_in->ay);
  axis6_out->az = biquad_filter(&lpf_az,axis6_in->az);
  axis6_out->gx = biquad_filter(&lpf_gx,axis6_in->gx);
	axis6_out->gy = biquad_filter(&lpf_gy,axis6_in->gy);
	axis6_out->gz = biquad_filter(&lpf_gz,axis6_in->gz);
}
