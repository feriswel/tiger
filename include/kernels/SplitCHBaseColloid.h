/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef SPLITCHBASECOLLOID_H
#define SPLITCHBASECOLLOID_H

#include "Kernel.h"
#include "JvarMapInterface.h"
#include "DerivativeMaterialInterface.h"

// Forward Declarations
class SplitCHBaseColloid;

template <>
InputParameters validParams<SplitCHBaseColloid>();

/// The couple, SplitCHBaseColloid and SplitCHWRes, splits the CH equation by replacing chemical potential with 'w'.
class SplitCHBaseColloid : public DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>
{
public:
  SplitCHBaseColloid(const InputParameters & parameters);

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
