/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef SPLITCHWTIGER_H
#define SPLITCHWTIGER_H

#include "SplitCHWBaseTiger.h"

/**
 * SplitCHWTiger creates the residual for the chemical
 * potential in the split form of the Cahn-Hilliard
 * equation with a scalar (isotropic) mobility.
 */
class SplitCHWTiger : public SplitCHWBaseTiger<Real>
{
public:
  SplitCHWTiger(const InputParameters & parameters);
};

template <>
InputParameters validParams<SplitCHWTiger>();

#endif // SPLITCHWRES_H
