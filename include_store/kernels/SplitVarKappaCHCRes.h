/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef SPLITVARKAPPACHCRES_H
#define SPLITVARKAPPACHCRES_H

#include "SplitCHBase.h"

// Forward Declarations
class SplitVarKappaCHCRes;

template <>
InputParameters validParams<SplitVarKappaCHCRes>();

/// The couple, SplitVarKappaCHCRes and SplitCHWRes, splits the CH equation by replacing chemical potential with 'w'.
class SplitVarKappaCHCRes : public SplitCHBase
{
public:
  SplitVarKappaCHCRes(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  const MaterialPropertyName _kappa_name;
  const MaterialProperty<Real> & _kappa;
  unsigned int _w_var;
  const VariableValue & _w;
};

#endif // SPLITCHCRES_H
