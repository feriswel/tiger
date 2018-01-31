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

#ifndef ELEMENTAVERAGEVALUECONST_H
#define ELEMENTAVERAGEVALUECONST_H

#include "ElementIntegralVariablePostprocessor.h"

// Forward Declarations
class ElementAverageValueConst;

template <>
InputParameters validParams<ElementAverageValueConst>();

/**
 * This postprocessor computes a volume integral of the specified variable.
 *
 * Note that specializations of this integral are possible by deriving from this
 * class and overriding computeQpIntegral().
 */
class ElementAverageValueConst : public ElementIntegralVariablePostprocessor
{
public:
  ElementAverageValueConst(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual Real getValue() override;
  virtual void threadJoin(const UserObject & y) override;

  Real retrieveValue() const;

protected:
  Real _volume;
  Real _integral;
};

#endif
