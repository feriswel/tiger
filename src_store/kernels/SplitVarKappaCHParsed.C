/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "SplitVarKappaCHParsed.h"

template <>
InputParameters
validParams<SplitVarKappaCHParsed>()
{
  InputParameters params = validParams<SplitVarKappaCHCRes>();
  params.addClassDescription(
      "Split formulation Cahn-Hilliard Kernel that uses a DerivativeMaterial Free Energy");
  params.addRequiredParam<MaterialPropertyName>(
      "f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  params.addCoupledVar("args", "Vector of additional arguments to F");
  return params;
}

SplitVarKappaCHParsed::SplitVarKappaCHParsed(const InputParameters & parameters)
  : DerivativeMaterialInterface<JvarMapKernelInterface<SplitVarKappaCHCRes>>(parameters),
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
SplitVarKappaCHParsed::initialSetup()
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
SplitVarKappaCHParsed::computeDFDC(PFFunctionType type)
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
SplitVarKappaCHParsed::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _w_var)
    return SplitVarKappaCHCRes::computeQpOffDiagJacobian(jvar);

  // get the coupled variable jvar is referring to
  const unsigned int cvar = mapJvarToCvar(jvar);

  return (*_d2Fdcdarg[cvar])[_qp] * _phi[_j][_qp] * _test[_i][_qp];
}
