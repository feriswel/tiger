/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "ACInterfaceColloid.h"

template <>
InputParameters
validParams<ACInterfaceColloid>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Gradient energy Allen-Cahn Kernel");
  params.addParam<MaterialPropertyName>("mob_name", "L", "The mobility used with the kernel");
  params.addParam<MaterialPropertyName>("kappa_name", "kappa_op", "The kappa used with the kernel");
  return params;
}

ACInterfaceColloid::ACInterfaceColloid(const InputParameters & parameters)
  : DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>(parameters),
    _L(getMaterialProperty<Real>("mob_name")),
    _kappa(getMaterialProperty<Real>("kappa_name")),
    _dLdop(getMaterialPropertyDerivative<Real>("mob_name", _var.name())),
    _d2Ldop2(getMaterialPropertyDerivative<Real>("mob_name", _var.name(), _var.name())),
    _dkappadop(getMaterialPropertyDerivative<Real>("kappa_name", _var.name())),
    _second_u(second()),
    _second_phi(secondPhi())
{
}

void
ACInterfaceColloid::initialSetup()
{
  validateCoupling<Real>("mob_name");
  validateCoupling<Real>("kappa_name");
}
//
// Real
// ACInterfaceColloid::kappaFunc()
// {
//   Real val = _kappa[_qp];// / (_u[_qp] * (1.0 - _u[_qp]));
//
//   return val;
// }
//
// Real
// ACInterfaceColloid::delKappa()
// {
//   Real val = _kappa[_qp] * ( 1.0 / std::pow((1 - _u[_qp]), 2.0) - 1.0 / std::pow(_u[_qp], 2.0)) ;
//
//   // return val;
//   return 0.0;
// }
//
// Real
// ACInterfaceColloid::del2Kappa()
// {
//   Real val = _kappa[_qp] * ( 2.0 / std::pow((1 - _u[_qp]), 3.0) + 2.0 / std::pow(_u[_qp], 3.0)) ;
//
//   // return val;
//   return 0.0;
// }
//
// Real
// ACInterfaceColloid::gradFactor()
// {
//   Real val = delKappa() * _grad_u[_qp] * _grad_u[_qp] / 2.0 - kappaFunc() * _second_u[_qp].tr();
//
//   return val;
// }
//
// Real
// ACInterfaceColloid::delGradFactor()
// {
//   // Real val = del2Kappa() * (_grad_u[_qp] * _grad_u[_qp]) * _phi[_j][_qp] / 2.0
//     // + delKappa() * _grad_u[_qp] * _grad_phi[_j][_qp]
//     // - delKappa() * _second_u[_qp].tr() * _phi[_j][_qp]
//   Real val =  -kappaFunc() * _second_phi[_j][_qp].tr();
//
//   return val;
// }

Real
ACInterfaceColloid::computeQpResidual()
{
  // return -_L[_qp] * _test[_i][_qp] * ( std::exp( -gradFactor() / 2.0) - std::exp( gradFactor() / 2.0));
  return -_L[_qp] * _kappa[_qp] * _test[_i][_qp] * _u[_qp];
}

Real
ACInterfaceColloid::computeQpJacobian()
{
  // Real val = -_dLdop[_qp] * _phi[_j][_qp] * _test[_i][_qp] * ( std::exp( -gradFactor()/ 2.0) - std::exp( gradFactor()/ 2.0));

  // Real val = _L[_qp] * _test[_i][_qp] * 0.5 * ( std::exp( -gradFactor() / 2.0) + std::exp( gradFactor() / 2.0)) * delGradFactor();
  Real val = -_L[_qp] * _kappa[_qp] * _test[_i][_qp] * _phi[_j][_qp];

  return val;
}
