/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "ReactiveSource.h"
// ORIGINAL FILE: MATREACTION IN PHASE-FIELD MODULE
template <>
InputParameters
validParams<ReactiveSource>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("v",
                       "Set this to make v a coupled variable, otherwise it will use the "
                       "kernel's nonlinear variable for v");
  params.addClassDescription("Kernel to add L*R, where L=prefactor, R=reaction rate");
  params.addParam<MaterialPropertyName>("mob_name", "L", "The reaction rate used with the kernel");
  params.addParam<Real>("gamma", 1.0, "Coefficient in the exponential");
  params.addCoupledVar("args", "Vector of nonlinear variable arguments this object depends on");
  return params;
}

ReactiveSource::ReactiveSource(const InputParameters & parameters)
  : DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>(parameters),
    _is_coupled(isCoupled("v")),
    _v_name(_is_coupled ? getVar("v", 0)->name() : _var.name()),
    _v(_is_coupled ? coupledValue("v") : _u),
    _v_var(_is_coupled ? coupled("v") : _var.number()),
    _L(getMaterialProperty<Real>("mob_name")),
    _eta_name(_var.name()),
    _dLdop(getMaterialPropertyDerivative<Real>("mob_name", _eta_name)),
    _dLdv(getMaterialPropertyDerivative<Real>("mob_name", _v_name)),
    _nvar(_coupled_moose_vars.size()),
    _dLdarg(_nvar),
    _gamma(getParam<Real>("gamma"))
{
  // Get reaction rate derivatives
  for (unsigned int i = 0; i < _nvar; ++i)
  {
    MooseVariable * ivar = _coupled_moose_vars[i];
    _dLdarg[i] = &getMaterialPropertyDerivative<Real>("mob_name", ivar->name());
  }
}

void
ReactiveSource::initialSetup()
{
  validateNonlinearCoupling<Real>("mob_name");
}

Real
ReactiveSource::computeQpResidual()
{
  return -_L[_qp] * ( 1 - _v[_qp]) * sqrt( 1 - _gamma * log( 1 - _v[_qp])) * _test[_i][_qp];
}

Real
ReactiveSource::computeQpJacobian()
{

  if (_is_coupled)
    return -(_dLdop[_qp] * ( 1 - _v[_qp])
          * sqrt( 1 - _gamma * log( 1 - _v[_qp]))) * _phi[_j][_qp] * _test[_i][_qp];

  return -(_L[_qp] * ((-2 + _gamma + 2 * _gamma * log( 1 - _v[_qp])) /
        (2 * sqrt( 1 - _gamma * log( 1 - _v[_qp]))))
        + _dLdop[_qp] * ( 1 - _v[_qp])
        * sqrt( 1 - _gamma * log( 1 - _v[_qp]))) * _phi[_j][_qp] * _test[_i][_qp];
}

Real
ReactiveSource::computeQpOffDiagJacobian(unsigned int jvar)
{
  // first handle the case where jvar is a coupled variable v being added to residual
  // the first term in the sum just multiplies by L which is always needed
  // the second term accounts for cases where L depends on v
  if ((jvar == _v_var) && _is_coupled)
    return -(_L[_qp] * ((-2 + _gamma + 2 * _gamma * log( 1 - _v[_qp])) /
          (2 * sqrt( 1 - _gamma * log( 1 - _v[_qp]))))
          + _dLdv[_qp] * ( 1 - _v[_qp])
          * sqrt( 1 - _gamma * log( 1 - _v[_qp]))) * _phi[_j][_qp] * _test[_i][_qp];

  //  for all other vars get the coupled variable jvar is referring to
  const unsigned int cvar = mapJvarToCvar(jvar);

  return -(*_dLdarg[cvar])[_qp] * ( 1 - _v[_qp]) * sqrt( 1 - _gamma * log( 1 - _v[_qp])) * _test[_i][_qp] * _phi[_j][_qp];
}
