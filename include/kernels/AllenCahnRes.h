/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef ALLENCAHNRES_H
#define ALLENCAHNRES_H

#include "Kernel.h"
#include "JvarMapInterface.h"
#include "DerivativeMaterialInterface.h"

// Forward Declarations
class AllenCahnRes;

template <>
InputParameters validParams<AllenCahnRes>();

/**
 * AllenCahnRes uses the Free Energy function and derivatives
 * provided by a DerivativeParsedMaterial to compute the
 * residual for the bulk part of the Allen-Cahn equation.
 */
class AllenCahnRes : public DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>
{
public:
  AllenCahnRes(const InputParameters & parameters);

  virtual void initialSetup();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  const unsigned int _nvar;
  const MaterialPropertyName _mob_name;
  const MaterialProperty<Real> & _L;

  std::vector<const MaterialProperty<Real> *> _dLdarg;
};

#endif
