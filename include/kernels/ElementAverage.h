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

#ifndef ELEMENTAVERAGE_H
#define ELEMENTAVERAGE_H

#include "ODEKernel.h"
#include "ElementAverageValue.h"

// Forward Declarations
class ElementAverage;

template <>
InputParameters validParams<ElementAverage>();

/**
 * Constant auxiliary value
 */
class ElementAverage : public ODEKernel
{
public:
  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  ElementAverage(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  /// The value being set for the current node/element

  const ElementAverageValue & _integral_uo;
};

#endif // CONSTANTAUX_H
