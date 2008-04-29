/*****************************************************************************/
/*                                                                           */
/*              8888888         88888888         88888888                    */
/*            8                8                8                            */
/*           8                 8                8                            */
/*           8                  88888888         88888888                    */
/*           8      8888                8                8                   */
/*            8       8                 8                8                   */
/*              888888         888888888        888888888                    */
/*                                                                           */
/*       A Two-Dimensional General Purpose Semiconductor Simulator.          */
/*                                                                           */
/*  GSS material database Version 0.4                                        */
/*  Last update: Feb 17, 2006                                                */
/*                                                                           */
/*  Gong Ding                                                                */
/*  gdiso@ustc.edu                                                           */
/*  NINT, No.69 P.O.Box, Xi'an City, China                                   */
/*                                                                           */
/*****************************************************************************/
//
// Material Type: Si(1-x)Ge(x). (in fact, use parameters for Si instead.)


#include "PMI.h"

typedef struct
{
   PetscScalar  wavelength;
   PetscScalar  RefractionIndexRe;
   PetscScalar  RefractionIndexIm;
} table;


static table  Si[] = {
{0.1240,  0.306 , 1.38},
{0.1305,  0.332 , 1.51},
{0.1378,  0.367 , 1.66},
{0.1459,  0.414 , 1.82},
{0.1550,  0.478 , 2.00},
{0.1653,  0.563 , 2.21},
{0.1771,  0.682 , 2.45},
{0.1907,  0.847 , 2.73},
{0.2066,  1.11  , 3.05},
{0.2254,  1.340 , 3.302},
{0.2480,  1.570 , 3.565}, 
{0.2755,  2.451 , 5.082},
{0.3100,  5.010 , 3.650},
{0.3306,  5.105 , 3.111},
{0.3542,  5.610 , 3.014},
{0.3815,  6.389 , 0.880},
{0.4133,  5.222 , 0.269},
{0.4275,  4.961 , 0.203},
{0.4428,  4.753 , 0.163},
{0.4592,  4.583 , 0.130},
{0.4769,  4.442 , 0.090},
{0.4959,  4.320 , 0.073},
{0.5166,  4.215 , 0.060},
{0.5391,  4.123 , 0.048},
{0.5636,  4.042 , 0.032},
{0.5904,  3.969 , 0.030},
{0.6199,  3.906 , 0.022},
{0.6526,  3.847 , 0.016},
{0.6888,  3.796 , 0.013},
{0.7293,  3.752 , 0.010},
{0.7749,  3.714 , 0.008},
{0.8266,  3.673 , 0.005},
{1.240 ,  3.5163, 1.5e-6},
};

class GSS_SiGe_Optical : public PMIS_Optical
{
private:
  int   table_size;
public:
  complex<PetscScalar> RefractionIndex(PetscScalar lamda, PetscScalar Eg=1.12, PetscScalar Tl=1.0) const
  {
    complex<PetscScalar> n(1.0,0.0);
    if(lamda<Si[0].wavelength) 
      return complex<PetscScalar> (Si[0].RefractionIndexRe,Si[0].RefractionIndexIm);
    if(lamda>Si[table_size-1].wavelength) 
      return complex<PetscScalar> (Si[table_size-1].RefractionIndexRe,Si[table_size-1].RefractionIndexIm);
    for(int i=0;i<table_size-1;i++)
    { 
       if(lamda>=Si[i].wavelength && lamda<=Si[i+1].wavelength)
       {
          complex<PetscScalar> n1(Si[i].RefractionIndexRe,Si[i].RefractionIndexIm);
          complex<PetscScalar> n2(Si[i+1].RefractionIndexRe,Si[i+1].RefractionIndexIm);
          PetscScalar d1 = lamda-Si[i].wavelength;
          PetscScalar d2 = Si[i+1].wavelength-lamda;
          n = (n1*d2 + n2*d1)/(d1+d2); 
	  break; 
       }
    }
    return n;   
  }           

// constructions
public:
  GSS_SiGe_Optical(const PMIS_Environment &env):PMIS_Optical(env)
  {
    table_size = sizeof(Si)/sizeof(table);
    //the wave length should be scaled
    for(int i=0;i<table_size;i++)
    {
        Si[i].wavelength*=um;    
    }
  } 
    
  ~GSS_SiGe_Optical()
  {}
}   
;

extern "C"
{
  PMIS_Optical*  PMIS_SiGe_Optical_Default (const PMIS_Environment& env)
  {
    return new GSS_SiGe_Optical(env);
  }
}
