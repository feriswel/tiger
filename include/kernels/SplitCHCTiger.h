/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef SPLITCHCTIGER_H
#define SPLITCHCTIGER_H

#include "SplitCHBaseTiger.h"
#include "JvarMapInterface.h"
#include "DerivativeMaterialInterface.h"

// Forward Declarations
class SplitCHCTiger;

template <>
InputParameters validParams<SplitCHCTiger>();

/// The couple, SplitCHCTiger and SplitCHWRes, splits the CH equation by replacing chemical potential with 'w'.
class SplitCHCTiger : public SplitCHBaseTiger
{
public:
  SplitCHCTiger(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  virtual Real computeDFDC(PFFunctionType type);

  const Real & _kappa;
  const Real & _Omega;
  const Real & _Delta;

  unsigned int _w_var;
  const VariableValue & _w;
};

#endif // SPLITCHCRES_H
