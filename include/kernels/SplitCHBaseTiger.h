/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef SPLITCHBASETIGER_H
#define SPLITCHBASETIGER_H

#include "Kernel.h"

// Forward Declarations
class SplitCHBaseTiger;

template <>
InputParameters validParams<SplitCHBaseTiger>();

/// The couple, SplitCHBaseTiger and SplitCHWRes, splits the CH equation by replacing chemical potential with 'w'.
class SplitCHBaseTiger : public Kernel
{
public:
  SplitCHBaseTiger(const InputParameters & parameters);

protected:
  enum PFFunctionType
  {
    Jacobian,
    Residual
  };
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  virtual Real computeDFDC(PFFunctionType type);
  virtual Real computeDEDC(PFFunctionType type);
};

#endif // SPLITCHBASE_H
