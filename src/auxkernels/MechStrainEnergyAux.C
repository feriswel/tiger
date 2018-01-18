/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "MechStrainEnergyAux.h"

template <>
InputParameters
validParams<MechStrainEnergyAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addClassDescription("Compute the local mechanical energy");
  params.addParam<std::string>("base_name", "Mechanical property base name");
  return params;
}

MechStrainEnergyAux::MechStrainEnergyAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _stress(getMaterialProperty<RankTwoTensor>(_base_name + "stress")),
    _mechanical_strain(getMaterialProperty<RankTwoTensor>(_base_name + "mechanical_strain"))
{
}

Real
MechStrainEnergyAux::computeValue()
{
  return 0.5 * _stress[_qp].doubleContraction(_mechanical_strain[_qp]);
}
