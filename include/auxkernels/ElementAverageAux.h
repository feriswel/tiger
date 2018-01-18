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

#ifndef ELEMENTAVERAGEAUX_H
#define ELEMENTAVERAGEAUX_H

#include "AuxScalarKernel.h"
#include "ElementAverageValue.h"

// Forward Declarations
class ElementAverageAux;

template <>
InputParameters validParams<ElementAverageAux>();

/**
 * Constant auxiliary value
 */
class ElementAverageAux : public AuxScalarKernel
{
public:
  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  ElementAverageAux(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  /// The value being set for the current node/element

  const ElementAverageValue & _integral_uo;
};

#endif // CONSTANTAUX_H
