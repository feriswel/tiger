/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "AllenCahnRes.h"

template <>
InputParameters
validParams<AllenCahnRes>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Allen-Cahn Kernel that uses a DerivativeMaterial Free Energy");
  params.addParam<MaterialPropertyName>("mob_name", "L", "The reaction rate coefficient used with the kernel");
  params.addCoupledVar("args", "Vector of additional arguments in defining the reaction rate");
  return params;
}

AllenCahnRes::AllenCahnRes(const InputParameters & parameters)
  : DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>(parameters),
    _nvar(_coupled_moose_vars.size()),
    _L(getMaterialProperty<Real>("mob_name")),
    _dLdarg(_nvar)
{

  // Iterate over all coupled variables
  for (unsigned int i = 0; i < _nvar; ++i)
    _dLdarg[i] = &getMaterialPropertyDerivative<Real>(_mob_name, _coupled_moose_vars[i]->name());
}

void
AllenCahnRes::initialSetup()
{
  validateCoupling<Real>("mob_name");
}

Real
AllenCahnRes::computeQpResidual()
{
  return -_L[_qp] * _test[_i][_qp] * (exp( -_u[_qp] / 2.0) - exp( _u[_qp] / 2.0));
}

Real
AllenCahnRes::computeQpJacobian()
{
  Real val = _L[_qp] * _test[_i][_qp] * 0.5 * (exp( -_u[_qp] / 2.0) + exp( _u[_qp] / 2.0)) * _phi[_j][_qp];
  return val;
}

Real
AllenCahnRes::computeQpOffDiagJacobian(unsigned int jvar)
{
  // get the coupled variable jvar is referring to
  const unsigned int cvar = mapJvarToCvar(jvar);

  return -(*_dLdarg[cvar])[_qp] * _phi[_j][_qp] * _test[_i][_qp] * (exp( -_u[_qp] / 2.0) - exp( _u[_qp] / 2.0));
}
