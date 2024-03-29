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

#ifndef BLOCKAVERAGEAUX_H
#define BLOCKAVERAGEAUX_H

#include "AuxKernel.h"

// Forward Declarations
class BlockAverageAux;

template <>
InputParameters validParams<BlockAverageAux>();

/**
 * Constant auxiliary value
 */
class BlockAverageAux : public AuxKernel
{
public:
  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  BlockAverageAux(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  /// The value being set for the current node/element

  const BlockAverageValue & _block_average_value;
};

#endif // CONSTANTAUX_H
