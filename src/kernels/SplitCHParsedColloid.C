/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "SplitCHParsedColloid.h"

template <>
InputParameters
validParams<SplitCHParsedColloid>()
{
  InputParameters params = validParams<SplitCHCResColloid>();
  params.addClassDescription(
      "Split formulation Cahn-Hilliard Kernel that uses a DerivativeMaterial Free Energy");
  params.addRequiredParam<MaterialPropertyName>(
      "f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  params.addCoupledVar("args", "Vector of additional arguments to F");
  return params;
}

SplitCHParsedColloid::SplitCHParsedColloid(const InputParameters & parameters)
  : SplitCHCResColloid(parameters),
    _nvar(_coupled_moose_vars.size()),
    _dFdc(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
    _d2Fdc2(getMaterialPropertyDerivative<Real>("f_name", _var.name(), _var.name()))
{
  // reserve space for derivatives
  _d2Fdcdarg.resize(_nvar);

  // Iterate over all coupled variables
  for (unsigned int i = 0; i < _nvar; ++i)
    _d2Fdcdarg[i] =
        &getMaterialPropertyDerivative<Real>("f_name", _var.name(), _coupled_moose_vars[i]->name());
}

void
SplitCHParsedColloid::initialSetup()
{
  /**
   * We are only interested if the necessary non-linear variables are coupled,
   * as those are the ones used in constructing the Jacobian. The AuxVariables
   * do not have Jacobian entries.
   */
  validateNonlinearCoupling<Real>("f_name", _var.name());
  validateDerivativeMaterialPropertyBase<Real>("f_name");
}

Real
SplitCHParsedColloid::computeDFDC(PFFunctionType type)
{
  switch (type)
  {
    case Residual:
      return _dFdc[_qp];

    case Jacobian:
      return _d2Fdc2[_qp] * _phi[_j][_qp];
  }

  mooseError("Internal error");
}

Real
SplitCHParsedColloid::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _w_var)
    return SplitCHCResColloid::computeQpOffDiagJacobian(jvar);

  // get the coupled variable jvar is referring to
  const unsigned int cvar = mapJvarToCvar(jvar);

  return (*_d2Fdcdarg[cvar])[_qp] * _phi[_j][_qp] * _test[_i][_qp];
}
