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

#ifndef CALCCONCAUX_H
#define CALCCONCAUX_H

#include "AuxScalarKernel.h"
#include "ElementAverageValue.h"

// Forward Declarations
class CalcConcAux;

template <>
InputParameters validParams<CalcConcAux>();

/**
 * Constant auxiliary value
 */
class CalcConcAux : public AuxScalarKernel
{
public:
  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  CalcConcAux(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  /// The value being set for the current node/element
  const Real _reaction_coef;
  const Real _diffusion_coef;
  const Real _gamma;

  const VariableValue & _phi;
  const VariableValue & _phi_old;
};

#endif // CONSTANTAUX_H
