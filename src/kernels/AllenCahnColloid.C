/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "AllenCahnColloid.h"

template <>
InputParameters
validParams<AllenCahnColloid>()
{
  InputParameters params = ACBulk<Real>::validParams();
  params.addClassDescription("Allen-Cahn Kernel that uses a DerivativeMaterial Free Energy");
  params.addRequiredParam<MaterialPropertyName>(
      "f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  params.addParam<MaterialPropertyName>("mob_name", "L", "The reaction rate coefficient used with the kernel");
  return params;
}

AllenCahnColloid::AllenCahnColloid(const InputParameters & parameters)
  : ACBulk<Real>(parameters),
    _nvar(_coupled_moose_vars.size()),
    _dFdEta(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
    _d2FdEta2(getMaterialPropertyDerivative<Real>("f_name", _var.name(), _var.name())),
    _d2FdEtadarg(_nvar)
{
  // Iterate over all coupled variables
  for (unsigned int i = 0; i < _nvar; ++i)
    _d2FdEtadarg[i] =
        &getMaterialPropertyDerivative<Real>("f_name", _var.name(), _coupled_moose_vars[i]->name());
}

void
AllenCahnColloid::initialSetup()
{
  ACBulk<Real>::initialSetup();
  validateNonlinearCoupling<Real>("f_name");
  validateDerivativeMaterialPropertyBase<Real>("f_name");
}

Real
AllenCahnColloid::computeDFDOP(PFFunctionType type)
{
  switch (type)
  {
    case Residual:
      return -(std::exp( -_dFdEta[_qp] / 2.0) - std::exp( _dFdEta[_qp] / 2.0));

    case Jacobian:
      return 0.5 * (std::exp( -_dFdEta[_qp] / 2.0) + std::exp( _dFdEta[_qp] / 2.0)) * _d2FdEta2[_qp] * _phi[_j][_qp];
  }

  mooseError("Internal error");
}

Real
AllenCahnColloid::computeQpOffDiagJacobian(unsigned int jvar)
{
  // get the coupled variable jvar is referring to
  const unsigned int cvar = mapJvarToCvar(jvar);

  return ACBulk<Real>::computeQpOffDiagJacobian(jvar) +
         (1 / 2.0) * (exp( -_dFdEta[_qp] / 2.0) + exp( _dFdEta[_qp] / 2.0)) *
         _L[_qp] * (*_d2FdEtadarg[cvar])[_qp] * _phi[_j][_qp] * _test[_i][_qp];
}
