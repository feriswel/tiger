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

#include "ElementAverageValueConst.h"

template <>
InputParameters
validParams<ElementAverageValueConst>()
{
  InputParameters params = validParams<ElementIntegralVariablePostprocessor>();
  return params;
}

ElementAverageValueConst::ElementAverageValueConst(const InputParameters & parameters)
  : ElementIntegralVariablePostprocessor(parameters), _volume(0)
{
}

void
ElementAverageValueConst::initialize()
{
  ElementIntegralVariablePostprocessor::initialize();
  _volume = 0;
}

void
ElementAverageValueConst::execute()
{
  ElementIntegralVariablePostprocessor::execute();

  _volume += _current_elem_volume;
}

Real
ElementAverageValueConst::getValue()
{
  _integral = ElementIntegralVariablePostprocessor::getValue();

  gatherSum(_volume);

  return _integral / _volume;
}

Real ElementAverageValueConst::retrieveValue() const
{
  return _integral / _volume;
}

void
ElementAverageValueConst::threadJoin(const UserObject & y)
{
  ElementIntegralVariablePostprocessor::threadJoin(y);
  const ElementAverageValueConst & pps = static_cast<const ElementAverageValueConst &>(y);
  _volume += pps._volume;
}
