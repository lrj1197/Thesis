/*
   pulse.c

   gcc -o pulse pulse.c -lpigpio -lrt -lpthread

   sudo ./pulse
*/

#include <stdio.h>
#include <pigpio.h>
#include <string.h>

#define DIR 20 // Direction GPIO Pin
#define STEP 21 // Step GPIO Pin
#define CW 1 // Clockwise Rotation
#define CCW 0 // Counterclockwise Rotation
#define SPR 48 // Steps per Revolution (360 / 7.5)


int main(int argc, char *argv[])
{
   double start;  	
   int step_count = SPR;
	float delay = 1.0/SPR; 

   if (gpioInitialise() < 0)
   {
      fprintf(stderr, "pigpio initialisation failed\n");
      return 1;
   }

   /* Set GPIO modes */
   gpioSetMode(DIR, PI_OUTPUT);
   gpioSetMode(STEP, PI_OUTPUT);

   int condition = 1;
   while(condition)
   {
      char cmd;
      scanf("%s", cmd);

      if(strcmp(cmd,"w"))
      {
         gpioWrite(DIR,CW); 
         for(int i = 0; i < step_count/8; i++)
         {
            gpioWrite(STEP, 1); /* on */
            time_sleep(delay);
            gpioWrite(STEP,0);
            time_sleep(delay);
         }   
      }

      else if(strcmp(cmd,"s"))
      {
         gpioWrite(DIR,CCW); 
         for(int i = 0; i < step_count/8; i++)
            {
               gpioWrite(STEP, 1); /* on */
               time_sleep(delay);
               gpioWrite(STEP,0);
               time_sleep(delay);
            }
      }

      else if(strcmp(cmd,"q"))
      {
         condition = 0;
      }

      else
      {
         printf("%s\n","Invalid command");
      }
   }

   /* Stop DMA, release resources */
   gpioTerminate();

   return 0;
}