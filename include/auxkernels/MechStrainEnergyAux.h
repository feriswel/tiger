/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef MECHSTRAINENERGYAUX_H
#define MECHSTRAINENERGYAUX_H

#include "AuxKernel.h"
#include "RankTwoTensor.h"

// Forward declarations
class MechStrainEnergyAux;

template <>
InputParameters validParams<MechStrainEnergyAux>();

class MechStrainEnergyAux : public AuxKernel
{
public:
  MechStrainEnergyAux(const InputParameters & parameters);
  virtual ~MechStrainEnergyAux() {}

protected:
  virtual Real computeValue();

  std::string _base_name;

  const MaterialProperty<RankTwoTensor> & _stress;
  const MaterialProperty<RankTwoTensor> & _mechanical_strain;
};

#endif // MECHSTRAINENERGYAUX_H
