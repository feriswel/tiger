/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef SPLITCHCRESCOLLOID_H
#define SPLITCHCRESCOLLOID_H

#include "SplitCHBaseColloid.h"

// Forward Declarations
class SplitCHCResColloid;

template <>
InputParameters validParams<SplitCHCResColloid>();

/// The couple, SplitCHCResColloid and SplitCHWRes, splits the CH equation by replacing chemical potential with 'w'.
class SplitCHCResColloid : public SplitCHBaseColloid
{
public:
  SplitCHCResColloid(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  const MaterialProperty<Real> & _kappa;
  const MaterialProperty<Real> & _dkappadop;
  unsigned int _w_var;
  const VariableValue & _w;
};

#endif // SPLITCHCRES_H
