/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef ACINTERFACECOLLOID_H
#define ACINTERFACECOLLOID_H

#include "Kernel.h"
#include "JvarMapInterface.h"
#include "DerivativeMaterialInterface.h"

class ACInterfaceColloid;

template <>
InputParameters validParams<ACInterfaceColloid>();

/**
 * Compute the Allen-Cahn interface term with the weak form residual
 * \f$ \left( \kappa_i \nabla\eta_i, \nabla (L_i \psi) \right) \f$
 */
class ACInterfaceColloid : public DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>
{
public:
  ACInterfaceColloid(const InputParameters & parameters);
  virtual void initialSetup();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  /// Mobility
  const MaterialProperty<Real> & _L;
  /// Interfacial parameter
  const MaterialProperty<Real> & _kappa;

  /// @{ Mobility derivatives w.r.t. order parameter
  const MaterialProperty<Real> & _dLdop;
  const MaterialProperty<Real> & _d2Ldop2;
  /// @}

  /// kappa derivative w.r.t. order parameter
  const MaterialProperty<Real> & _dkappadop;

  const VariableSecond & _second_u;
  const VariablePhiSecond & _second_phi;


  // Real kappaFunc();
  // Real delKappa();
  // Real del2Kappa();
  // Real gradFactor();
  // Real delGradFactor();

};

#endif // ACInterfaceColloid_H
