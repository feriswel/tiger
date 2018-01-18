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

#include "ElementAverage.h"

template <>
InputParameters
validParams<ElementAverage>()
{
  InputParameters params = validParams<ODEKernel>();
  params.addRequiredParam<UserObjectName>( "user_object", "Doc");
  return params;
}

ElementAverage::ElementAverage(const InputParameters & parameters)
  : ODEKernel(parameters),
  _integral_uo(getUserObject<ElementAverageValue>("user_object"))
{
}

Real
ElementAverage::computeQpResidual()
{
  return 3.;//_integral_uo.retrieveValue();
}

Real
ElementAverage::computeQpJacobian()
{
  return 0.0;
}
