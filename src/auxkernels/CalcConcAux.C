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


// This custom kernel is used to solve the ODE for the bulk concentration explicitly
// dc_dt = -(R_coef/phi) * dphi_dt + D_coef * ( 1 - c),
// where c must be normalized w.r.t. the maximum supersaturation
// (i.e., the value at which the source phase stops dissolving)

#include "CalcConcAux.h"
template <>
InputParameters
validParams<CalcConcAux>()
{
  InputParameters params = validParams<AuxScalarKernel>();
  params.addRequiredParam<Real>("reaction_coef",
      "change in concentration per unit of CSH precipitated");
  params.addRequiredParam<Real>("diffusion_coef",
      "change in concentration per unit of CSH precipitated");
  params.addParam<Real>("gamma",1.0,
      "change in concentration per unit of CSH precipitated");
  params.addRequiredCoupledVar("phi_mean", "mean packing density");
  return params;
}

CalcConcAux::CalcConcAux(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
  _reaction_coef(getParam<Real>("reaction_coef")),
  _diffusion_coef(getParam<Real>("diffusion_coef")),
  _gamma(getParam<Real>("gamma")),
  _phi(coupledScalarValue("phi_mean")),
  _phi_old(coupledScalarValueOld("phi_mean"))
{
}

Real
CalcConcAux::computeValue()
{
  return _u[_i] - _reaction_coef * (_phi[_i] / _phi_old[_i] - 1.0)
    + _diffusion_coef * (1.0 - _u[_i]) * exp( - _gamma * _phi[_i] / (1.0 - _phi[_i]));
}
