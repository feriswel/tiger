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

#include "ElementAverageAux.h"
// Originial file is ConstantAux.C in the framework/auxkernel
template <>
InputParameters
validParams<ElementAverageAux>()
{
  InputParameters params = validParams<AuxScalarKernel>();
  params.addRequiredParam<UserObjectName>( "user_object", "Doc");
  return params;
}

ElementAverageAux::ElementAverageAux(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
  _integral_uo(getUserObject<ElementAverageValue>("user_object"))
{
}

Real
ElementAverageAux::computeValue()
{
  // Real out;

  // out = _integral_uo.getValue();
  return _integral_uo.retrieveValue();
}
