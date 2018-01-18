/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef ALLENCAHNCOLLOID_H
#define ALLENCAHNCOLLOID_H

#include "ACBulk.h"

// Forward Declarations
class AllenCahnColloid;

template <>
InputParameters validParams<AllenCahnColloid>();

/**
 * AllenCahnColloid uses the Free Energy function and derivatives
 * provided by a DerivativeParsedMaterial to computer the
 * residual for the bulk part of the Allen-Cahn equation.
 */
class AllenCahnColloid : public ACBulk<Real>
{
public:
  AllenCahnColloid(const InputParameters & parameters);

  virtual void initialSetup();

protected:
  virtual Real computeDFDOP(PFFunctionType type);
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  const unsigned int _nvar;
  const MaterialProperty<Real> & _dFdEta;
  const MaterialProperty<Real> & _d2FdEta2;

  std::vector<const MaterialProperty<Real> *> _d2FdEtadarg;
};

#endif // ALLENCAHN_H
