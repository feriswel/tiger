/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "SplitVarKappaCHCRes.h"

template <>
InputParameters
validParams<SplitVarKappaCHCRes>()
{
  InputParameters params = validParams<SplitCHBase>();
  params.addClassDescription("Split formulation Cahn-Hilliard Kernel");
  params.addRequiredCoupledVar("w", "chem poten");
  params.addRequiredParam<MaterialPropertyName>("kappa_name", "The kappa used with the kernel");
  params.addCoupledVar("args", "Vector of arguments of the gradient energy coefficient");
  return params;
}

template <typename T>
SplitVarKappaCHCRes::SplitVarKappaCHCRes(const InputParameters & parameters)
  : SplitCHBase(parameters),
    _kappa_name(getParam<MaterialPropertyName>("kappa_name")),
    _kappa(getMaterialProperty<T>("kappa_name")),
    _w_var(coupled("w")),
    _w(coupledValue("w"))
{
  // Get number of coupled variables
  unsigned int nvar = _coupled_moose_vars.size();

  // reserve space for derivatives
  _dkappadarg.resize(nvar);

  // Iterate over all coupled variables
  for (unsigned int i = 0; i < nvar; ++i)
    _dkappadarg[i] = &getMaterialPropertyDerivative<T>(_kappa_name, _coupled_moose_vars[i]->name());
}

/*Real
SplitVarKappaCHCRes::computeDFDC(PFFunctionType type)
{
  switch (type)
  {
  case Residual:
    return _u[_qp]*_u[_qp]*_u[_qp] - _u[_qp]; // return Residual value

  case Jacobian:
    return (3.0*_u[_qp]*_u[_qp] - 1.0)*_phi[_j][_qp]; //return Jacobian value

  }

  mooseError("Invalid type passed in");
}*/

Real
SplitVarKappaCHCRes::computeQpResidual()
{
  Real residual =
      SplitCHBase::computeQpResidual(); //(f_prime_zero+e_prime)*_test[_i][_qp] from SplitCHBase

  residual += -_w[_qp] * _test[_i][_qp];
  residual += _kappa[_qp] * _grad_u[_qp] * _grad_test[_i][_qp];

  return residual;
}

Real
SplitVarKappaCHCRes::computeQpJacobian()
{
  Real jacobian = SplitCHBase::computeQpJacobian(); //(df_prime_zero_dc+de_prime_dc)*_test[_i][_qp];
                                                    // from SplitCHBase

  jacobian += _kappa[_qp] * _grad_phi[_j][_qp] * _grad_test[_i][_qp];

  return jacobian;
}

Real
SplitVarKappaCHCRes::computeQpOffDiagJacobian(unsigned int jvar)
{

  Real offDiagJacobian = SplitCHBase::computeQpOffDiagJacobian();

  if (jvar == _w_var)
  {
    offDiagJacobian = -_phi[_j][_qp] * _test[_i][_qp];
  }

  offDiagJacobian += (*_dkappadarg[cvar])[_qp] * _phi[_j][_qp] * _grad_u[_qp] * _grad_test[_i][_qp];

  return offDiagJacobian;
}
