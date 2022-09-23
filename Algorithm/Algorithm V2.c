/*This Algorithm is Created by Shady Magdy Hussein*/
/*
 * Include Files
 *
 */
#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif



/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
#include <math.h>
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 1
/*STD_TYPES include*/
#include "STD_TYPES.h"
/*_MACROS_*/
#define     TTC1             0.6
#define     TTC2             0.8
#define     TTC3             1.2

#define     DRIVER_CONTROL    1
#define     SYSTEM_CONTROL    2

#define     MAX_SPEED             25
#define     MAX_BRAKE_FORCE       150
#define     TRIGGER_BRAKE_FORCE   50


/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
/* extern double func(double a); */
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Output function
 *
 */
f32 calculate_critical_time(const real32_T speed)
{
	f32 Local_f32_TTC_Value = 100;
    if (speed <= 8)
	{
		Local_f32_TTC_Value = TTC1; // 30km/h
	}
		
    else if (speed <= 13) 
	{
		Local_f32_TTC_Value = TTC2; // 50km/h
	}
    else if (speed <= 25) 
	{
		Local_f32_TTC_Value =TTC3; //90km/h
	}
	else {/*MISRA*/}
    return Local_f32_TTC_Value;
}

void braking_Outputs_wrapper(const real32_T *range,
			const real32_T *relative_velocity,
			const real32_T *car_velocity,
			real_T *brake,
			real_T *control)
{
    *control = DRIVER_CONTROL;
	/*When car velocity is > MAX_SPEED  (25m/s) system will not operate*/
    if (*car_velocity > MAX_SPEED );  
    else
    {

       f32 alt_relative_velocity = -1 * (*relative_velocity);
       f32 critical_time = calculate_critical_time(alt_relative_velocity); // calculate critical time   
       f32 collosion_time = (*range)/(alt_relative_velocity);
       
       
       if ((alt_relative_velocity > 0) &&(0 < collosion_time) && (collosion_time <= critical_time))   
       {
           *control = SYSTEM_CONTROL ;   
           *brake = MAX_BRAKE_FORCE ;
       }
       else if((alt_relative_velocity > 0) &&(0 < collosion_time) && (collosion_time <= critical_time + .6)){
          //This Equation is a brake function to make the brake force change gradually
		  *brake = TRIGGER_BRAKE_FORCE * (2 / ( collosion_time) );
          if (*brake > MAX_BRAKE_FORCE ) *brake = MAX_BRAKE_FORCE ;
       
       }else if(*range>20){
           *control = DRIVER_CONTROL;
           *brake = 0;
       
       }
    }
}


