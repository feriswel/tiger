/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "SplitCHCResColloid.h"

template <>
InputParameters
validParams<SplitCHCResColloid>()
{
  InputParameters params = validParams<SplitCHBaseColloid>();
  params.addClassDescription("Split formulation Cahn-Hilliard Kernel");
  params.addRequiredCoupledVar("w", "chem poten");
  params.addRequiredParam<MaterialPropertyName>("kappa_name", "The kappa used with the kernel");
  return params;
}

SplitCHCResColloid::SplitCHCResColloid(const InputParameters & parameters)
  : SplitCHBaseColloid(parameters),
    _kappa(getMaterialProperty<Real>("kappa_name")),
    _dkappadop(getMaterialPropertyDerivative<Real>("kappa_name", _var.name())),
    _w_var(coupled("w")),
    _w(coupledValue("w"))
{
}

/*Real
SplitCHCResColloid::computeDFDC(PFFunctionType type)
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
SplitCHCResColloid::computeQpResidual()
{
  Real residual =
      SplitCHBaseColloid::computeQpResidual(); //(f_prime_zero+e_prime)*_test[_i][_qp] from SplitCHBaseColloid

  residual += -_w[_qp] * _test[_i][_qp];
  residual += _kappa[_qp] * _grad_u[_qp] * _grad_test[_i][_qp];
  residual += -_dkappadop[_qp] * _grad_u[_qp] * _grad_u[_qp] * _test[_i][_qp] / 2.0;

  return residual;
}

Real
SplitCHCResColloid::computeQpJacobian()
{
  Real jacobian = SplitCHBaseColloid::computeQpJacobian(); //(df_prime_zero_dc+de_prime_dc)*_test[_i][_qp];
                                                    // from SplitCHBaseColloid

  jacobian += _kappa[_qp] * _grad_phi[_j][_qp] * _grad_test[_i][_qp];
  jacobian += -_dkappadop[_qp] * _grad_u[_qp] * _grad_phi[_j][_qp] * _test[_i][_qp];

  return jacobian;
}

Real
SplitCHCResColloid::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _w_var)
  {
    return -_phi[_j][_qp] * _test[_i][_qp];
  }

  return 0.0;
}
