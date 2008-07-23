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
/*  Last update: Nov 28, 2007                                                */
/*                                                                           */
/*  Gong Ding                                                                */
/*  gdiso@ustc.edu                                                           */
/*  NINT, No.69 P.O.Box, Xi'an City, China                                   */
/*                                                                           */
/*****************************************************************************/
//
// Material Type: Silicon

#include "PMI.h"

class GSS_Si_Avalanche_Valdinoci : public PMIS_Avalanche
{
private:
  PetscScalar A0N_VALD;
  PetscScalar A0P_VALD;
  PetscScalar A1N_VALD;
  PetscScalar A1P_VALD;
  PetscScalar A2N_VALD;
  PetscScalar A2P_VALD;
  PetscScalar B0N_VALD;
  PetscScalar B0P_VALD;
  PetscScalar B1N_VALD;
  PetscScalar B1P_VALD;
  PetscScalar C0N_VALD;
  PetscScalar C0P_VALD;
  PetscScalar C1N_VALD;
  PetscScalar C1P_VALD;
  PetscScalar C2N_VALD;
  PetscScalar C2P_VALD;
  PetscScalar C3N_VALD;
  PetscScalar C3P_VALD;
  PetscScalar D0N_VALD;
  PetscScalar D0P_VALD;
  PetscScalar D1N_VALD;
  PetscScalar D1P_VALD;
  PetscScalar D2N_VALD;
  PetscScalar D2P_VALD;

  PetscScalar ElecTauw;
  PetscScalar HoleTauw;
  PetscScalar T300    ;

  void 	Avalanche_Init()
  {
    A0N_VALD = 4.338300E+00*V;
    A0P_VALD = 2.376000E+00*V;
    A2N_VALD = 4.123300E+00;
    A2P_VALD = 1.000000E+00;
    A1N_VALD =-2.420000E-12*V/pow(K,A2N_VALD);
    A1P_VALD = 1.033000E-02*V/pow(K,A2P_VALD);
    B0N_VALD = 2.350000E-01*V;
    B0P_VALD = 1.771400E-01*V;
    B1N_VALD = 0.000000E+00/K;
    B1P_VALD =-2.178000E-03/K;
    C0N_VALD = 1.683100E+04*V/cm;
    C0P_VALD = 0.000000E+00*V/cm;
    C2N_VALD = 1.000000E+00;
    C2P_VALD = 2.492400E+00;
    C1N_VALD = 4.379600E+00*V/(cm*pow(K,C2N_VALD));
    C1P_VALD = 9.470000E-03*V/(cm*pow(K,C2P_VALD));
    C3N_VALD = 1.300500E-01*V/(cm*K*K);
    C3P_VALD = 0.000000E+00*V/(cm*K*K);
    D0N_VALD = 1.233735E+06*V/cm;
    D0P_VALD = 1.404300E+06*V/cm;
    D1N_VALD = 1.203900E+03*V/(cm*K);
    D1P_VALD = 2.974400E+03*V/(cm*K);
    D2N_VALD = 5.670300E-01*V/(cm*K*K);
    D2P_VALD = 1.482900E+00*V/(cm*K*K);
    //
    ElecTauw  = 6.800000E-13*s;
    HoleTauw  = 2.000000E-13*s;
    T300      = 300.0*K;
  }
public:
  //---------------------------------------------------------------------------
  // Electron Impact Ionization rate for DDM 
  PetscScalar ElecGenRate (const PetscScalar &Tl,const PetscScalar &Ep,const PetscScalar &Eg) const
  {
    if (Ep < 1*V/cm)
    {
      return 0;
    }
    else
    {
      PetscScalar a = A0N_VALD + A1N_VALD*pow(Tl,A2N_VALD);
      PetscScalar b = B0N_VALD*exp(B1N_VALD*Tl);
      PetscScalar c = C0N_VALD + C1N_VALD*pow(Tl,C2N_VALD) + C3N_VALD*Tl*Tl;
      PetscScalar d = D0N_VALD + D1N_VALD*Tl + D2N_VALD*Tl*Tl;
      return Ep/(a+b*exp(d/(Ep+c)));
    }
  }
  AutoDScalar ElecGenRate (const AutoDScalar &Tl,const AutoDScalar &Ep,const AutoDScalar &Eg) const
  {
    if (Ep < 1*V/cm)
    {
      return 0;
    }
    else
    {
      AutoDScalar a = A0N_VALD + A1N_VALD*pow(Tl,A2N_VALD);
      AutoDScalar b = B0N_VALD*exp(B1N_VALD*Tl);
      AutoDScalar c = C0N_VALD + C1N_VALD*pow(Tl,C2N_VALD) + C3N_VALD*Tl*Tl;
      AutoDScalar d = D0N_VALD + D1N_VALD*Tl + D2N_VALD*Tl*Tl;
      return Ep/(a+b*exp(d/(Ep+c)));
    }
  }
  
  //---------------------------------------------------------------------------
  // Hole Impact Ionization rate for DDM
  PetscScalar HoleGenRate (const PetscScalar &Tl,const PetscScalar &Ep,const PetscScalar &Eg) const
  {
    if (Ep < 1*V/cm)
    {
      return 0;
    }
    else
    {
      PetscScalar a = A0P_VALD + A1P_VALD*pow(Tl,A2P_VALD);
      PetscScalar b = B0P_VALD*exp(B1P_VALD*Tl);
      PetscScalar c = C0P_VALD + C1P_VALD*pow(Tl,C2P_VALD) + C3P_VALD*Tl*Tl;
      PetscScalar d = D0P_VALD + D1P_VALD*Tl + D2P_VALD*Tl*Tl;
      return Ep/(a+b*exp(d/(Ep+c)));
    }
  }
  AutoDScalar HoleGenRate (const AutoDScalar &Tl,const AutoDScalar &Ep,const AutoDScalar &Eg) const
  {
    if (Ep < 1*V/cm)
    {
      return 0;
    }
    else
    {
      AutoDScalar a = A0P_VALD + A1P_VALD*pow(Tl,A2P_VALD);
      AutoDScalar b = B0P_VALD*exp(B1P_VALD*Tl);
      AutoDScalar c = C0P_VALD + C1P_VALD*pow(Tl,C2P_VALD) + C3P_VALD*Tl*Tl;
      AutoDScalar d = D0P_VALD + D1P_VALD*Tl + D2P_VALD*Tl*Tl;
      return Ep/(a+b*exp(d/(Ep+c)));
    }
  }

  //---------------------------------------------------------------------------
  // Electron Impact Ionization rate for EBM
  PetscScalar ElecGenRateEBM (const PetscScalar &Tn,const PetscScalar &Tl,const PetscScalar &Eg) const
  {
    PetscScalar vsat = (2.4e7*cm/s)/(1+0.8*exp(Tl/(2*T300)));
    PetscScalar Eeff=1.5*kb*(Tn-Tl)/vsat/ElecTauw;
    PetscScalar a = A0N_VALD + A1N_VALD*pow(Tl,A2N_VALD);
    PetscScalar b = B0N_VALD*exp(B1N_VALD*Tl);
    PetscScalar c = C0N_VALD + C1N_VALD*pow(Tl,C2N_VALD) + C3N_VALD*Tl*Tl;
    PetscScalar d = D0N_VALD + D1N_VALD*Tl + D2N_VALD*Tl*Tl;
    return Eeff/(a+b*exp(d/(Eeff+c)));
  }
  AutoDScalar ElecGenRateEBM (const AutoDScalar &Tn,const AutoDScalar &Tl,const AutoDScalar &Eg) const
  {
    AutoDScalar vsat = (2.4e7*cm/s)/(1+0.8*exp(Tl/(2*T300)));
    AutoDScalar Eeff=1.5*kb*(Tn-Tl)/vsat/ElecTauw;
    AutoDScalar a = A0N_VALD + A1N_VALD*pow(Tl,A2N_VALD);
    AutoDScalar b = B0N_VALD*exp(B1N_VALD*Tl);
    AutoDScalar c = C0N_VALD + C1N_VALD*pow(Tl,C2N_VALD) + C3N_VALD*Tl*Tl;
    AutoDScalar d = D0N_VALD + D1N_VALD*Tl + D2N_VALD*Tl*Tl;
    return Eeff/(a+b*exp(d/(Eeff+c)));
  }
  
  //---------------------------------------------------------------------------
  // Hole Impact Ionization rate for EBM
  PetscScalar HoleGenRateEBM (const PetscScalar &Tp,const PetscScalar &Tl,const PetscScalar &Eg) const
  {
    PetscScalar vsat = (2.4e7*cm/s)/(1+0.8*exp(Tl/(2*T300)));
    PetscScalar Eeff=1.5*kb*(Tp-Tl)/vsat/HoleTauw;
    PetscScalar a = A0P_VALD + A1P_VALD*pow(Tl,A2P_VALD);
    PetscScalar b = B0P_VALD*exp(B1P_VALD*Tl);
    PetscScalar c = C0P_VALD + C1P_VALD*pow(Tl,C2P_VALD) + C3P_VALD*Tl*Tl;
    PetscScalar d = D0P_VALD + D1P_VALD*Tl + D2P_VALD*Tl*Tl;
    return Eeff/(a+b*exp(d/(Eeff+c)));
  }
  AutoDScalar HoleGenRateEBM (const AutoDScalar &Tp,const AutoDScalar &Tl,const AutoDScalar &Eg) const
  {
    AutoDScalar vsat = (2.4e7*cm/s)/(1+0.8*exp(Tl/(2*T300)));
    AutoDScalar Eeff=1.5*kb*(Tp-Tl)/vsat/HoleTauw;
    AutoDScalar a = A0P_VALD + A1P_VALD*pow(Tl,A2P_VALD);
    AutoDScalar b = B0P_VALD*exp(B1P_VALD*Tl);
    AutoDScalar c = C0P_VALD + C1P_VALD*pow(Tl,C2P_VALD) + C3P_VALD*Tl*Tl;
    AutoDScalar d = D0P_VALD + D1P_VALD*Tl + D2P_VALD*Tl*Tl;
    return Eeff/(a+b*exp(d/(Eeff+c)));
  }


  //----------------------------------------------------------------
  GSS_Si_Avalanche_Valdinoci(const PMIS_Environment &env):PMIS_Avalanche(env)
  {
    Avalanche_Init();
  }
  ~GSS_Si_Avalanche_Valdinoci()
  {}

}
;

extern "C"
{
  PMIS_Avalanche* PMIS_Si_Avalanche_Valdinoci (const PMIS_Environment& env)
  {
    return new GSS_Si_Avalanche_Valdinoci(env);
  }
}
