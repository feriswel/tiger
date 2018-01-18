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

#include "BlockAverageAux.h"
// Originial file is ConstantAux.C in the framework/auxkernel
template <>
InputParameters
validParams<BlockAverageAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredParam<UserObjectName>("block_average_userobject","Doc");
  return params;
}

BlockAverageAux::BlockAverageAux(const InputParameters & parameters)
  : AuxKernel(parameters),
  _block_average_value(getUserObject<BlockAverageValue>("block_average_userobject"))
{
}

Real
BlockAverageAux::computeValue()
{
  return _block_average_value.averageValue(_current_elem->subdomain_id());
}
