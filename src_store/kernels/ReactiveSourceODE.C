/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "ReactiveSourceODE.h"

template <>
InputParameters
validParams<ReactiveSourceODE>()
{
  InputParameters params = validParams<ReactiveSourceODE>();
  params.addRequiredCoupledVar("mean_pack", "Coupled variable");
  params.addParam<Real>("conversion_constant",1.0,
      "change in concentration per unit of CSH precipitated");
  return params;
}

ReactiveSourceODE::ReactiveSourceODE(const InputParameters & parameters)
  : ODETimeKernel(parameters),
  _mean_pack(coupledScalarValue("mean_pack")),
  _mean_pack_old(coupledScalarValueOld("mean_pack")),
  _conversion_constant(getParam<Real>("conversion_constant"))
{
}

Real
ReactiveSourceODE::computeResidual()
{
  Real _porosity, _porosity_old;
  _porosity = 1.0 - _mean_pack;
  _porosity_old = 1.0 - _mean_pack_old;

  return _conversion_constant * (_mean_pack / _mean_pack_old - 1.0) / _dt;
}
