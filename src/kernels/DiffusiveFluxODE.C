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

#include "DiffusiveFluxODE.h"

template <>
InputParameters
validParams<DiffusiveFluxODE>()
{
  InputParameters params = validParams<DiffusiveFluxODE>();
  params.addRequiredCoupledVar("mean_pack", "Coupled variable");
  params.addParam<Real>("self_diffusion",1.0,
      "diffusion in the infinitely dilute, unobstructed case");
  params.addParam<Real>("overlap_coef",1.0,
      "crowding/overlap coefficient");
  return params;
}

DiffusiveFluxODE::DiffusiveFluxODE(const InputParameters & parameters)
  : ODETimeKernel(parameters),
  _mean_pack(coupledScalarValue("mean_pack")),
  _self_diffusion(getParam<Real>("self_diffusion")),
  _overlap_coef(getParam<Real>("overlap_coef"))
{
}

Real
DiffusiveFluxODE::computeQpResidual()
{
  return ( _u[_i] - 1.0) * _self_diffusion * exp( - _overlap_coef * _mean_pack[_i] / ( 1.0 - _mean_pack[_i]));
}

Real
DiffusiveFluxODE::computeQpJacobian()
{
  return _self_diffusion * exp( - _overlap_coef * _mean_pack[_i] / ( 1.0 - _mean_pack[_i]));
}
