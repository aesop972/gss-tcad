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
/*       A Two-Dimensional General Purpose Semiconductor GaAsmulator.        */
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
// Material Type: AlGaAs

// We do not have the real data for AlGaAs, use parameters for GaAs instead

#include "PMI.h"

typedef struct
{
   PetscScalar  wavelength;
   PetscScalar  RefractionIndexRe;
   PetscScalar  RefractionIndexIm;
} table;


static table  GaAs[] =  {
{0.0620,  1.025,  0.144},
{0.0827,  0.836,  0.282},
{0.1240,  0.913,  0.974},
{0.1378,  0.901,  1.136},
{0.1550,  0.899,  1.435},
{0.1771,  1.063,  1.838},
{0.2066,  1.264,  2.472},
{0.2254,  1.383,  2.936},
{0.2480,  2.273,  4.084},
{0.2755,  3.913,  2.919},
{0.3100,  3.601,  1.920},
{0.3542,  3.531,  2.013},
{0.4133,  4.509,  1.948},
{0.4275,  5.052,  1.721}, 
{0.4428,  4.959,  0.991},
{0.4592,  4.694,  0.696},
{0.4769,  4.492,  0.539},
{0.4959,  4.333,  0.441},
{0.5166,  4.205,  0.371},
{0.5391,  4.100,  0.320},
{0.5636,  4.013,  0.276},
{0.5904,  3.904,  0.240},
{0.6199,  3.878,  0.211},
{0.6526,  3.826,  0.179},
{0.6888,  3.785,  0.151},
{0.7293,  3.742,  0.112},
{0.7749,  3.700,  0.091},
{0.8266,  3.666,  0.080},
{0.8856,  3.614,  0.002},
{1.0000,  3.509,  8.5e-5},
{2.0000,  3.509,  0.000}, 
};

class GSS_AlGaAs_Optical : public PMIS_Optical
{
private:
  int   table_size;
public:
  complex<PetscScalar> RefractionIndex(PetscScalar lamda, PetscScalar Eg=1.12, PetscScalar Tl=1.0) const
  {
    complex<PetscScalar> n(1.0,0.0);
    if(lamda<GaAs[0].wavelength) 
      return complex<PetscScalar> (GaAs[0].RefractionIndexRe,GaAs[0].RefractionIndexIm);
    if(lamda>GaAs[table_size-1].wavelength) 
      return complex<PetscScalar> (GaAs[table_size-1].RefractionIndexRe,GaAs[table_size-1].RefractionIndexIm);
    for(int i=0;i<table_size-1;i++)
    { 
       if(lamda>=GaAs[i].wavelength && lamda<=GaAs[i+1].wavelength)
       {
          complex<PetscScalar> n1(GaAs[i].RefractionIndexRe,GaAs[i].RefractionIndexIm);
          complex<PetscScalar> n2(GaAs[i+1].RefractionIndexRe,GaAs[i+1].RefractionIndexIm);
          PetscScalar d1 = lamda-GaAs[i].wavelength;
          PetscScalar d2 = GaAs[i+1].wavelength-lamda;
          n = (n1*d2 + n2*d1)/(d1+d2);
          break;  
       }
    }
    return n;   
  }           

// constructions
public:
  GSS_AlGaAs_Optical(const PMIS_Environment &env):PMIS_Optical(env)
  {
    table_size = sizeof(GaAs)/sizeof(table);
    //the wave length should be scaled
    for(int i=0;i<table_size;i++)
    {
        GaAs[i].wavelength*=um;    
    }
  }

  ~GSS_AlGaAs_Optical()
  {}
}
;

extern "C"
{
  PMIS_Optical*  PMIS_AlGaAs_Optical_Default (const PMIS_Environment& env)
  {
    return new GSS_AlGaAs_Optical(env);
  }
}

