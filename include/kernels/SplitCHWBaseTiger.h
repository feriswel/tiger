/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef SPLITCHWBASETIGER_H
#define SPLITCHWBASETIGER_H

#include "Kernel.h"
#include "JvarMapInterface.h"
#include "DerivativeMaterialInterface.h"

/**
 * SplitCHWresBase implements the residual for the chemical
 * potential in the split form of the Cahn-Hilliard
 * equation in a general way that can be templated to a scalar or
 * tensor mobility.
 */
template <typename T>
class SplitCHWBaseTiger : public DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>
{
public:
  SplitCHWBaseTiger(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:

  virtual Real computeMob();
  virtual Real computedMobdop();
  const Real & _u_c;
  const Real & _L_cal;
  const Real & _gamma;
  const Real & _nu;
  const Real & _df;

};

template <typename T>
SplitCHWBaseTiger<T>::SplitCHWBaseTiger(const InputParameters & parameters)
  : DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>(parameters),
    _u_c(getParam<Real>("u_c")),
    _L_cal(getParam<Real>("L_cal")),
    _gamma(getParam<Real>("gamma")),
    _nu(getParam<Real>("nu")),
    _df(getParam<Real>("df"))
{
}

template <typename T>
InputParameters
SplitCHWBaseTiger<T>::validParams()
{
  InputParameters params = ::validParams<Kernel>();
  params.addClassDescription(
      "Split formulation Cahn-Hilliard Kernel for the chemical potential variable");
  params.addRequiredParam<Real>("u_c", "critical percolating packing fraction");
  params.addRequiredParam<Real>("L_cal", "mean-field length scale");
  params.addRequiredParam<Real>("gamma", "critical exponent");
  params.addRequiredParam<Real>("nu", "critical exponent for the correlation length");
  params.addRequiredParam<Real>("df", "fractal dimension");
  return params;
}

template <typename T>
Real
SplitCHWBaseTiger<T>::computeMob()
{
  return (_u[_qp] * ( 1.0 - _u[_qp]))
    * pow( pow( _u_c, -_gamma) * pow( _L_cal, -_gamma / _nu)
    * pow( - (_u[_qp] - _u_c) * pow( _L_cal, 1 / _nu) * ( (_u[_qp] - _u_c)  < 0.0)
    + exp( - abs( (_u[_qp] - _u_c) * pow( _L_cal, 1.0 / _nu) ) / 2.0)
    , _gamma), 1.0 / _df);
}

template <typename T>
Real
SplitCHWBaseTiger<T>::computedMobdop()
{
  return ( 1.0 - 2.0 * _u[_qp])
    * pow( pow( _u_c, -_gamma) * pow( _L_cal, -_gamma / _nu)
    * pow( - (_u[_qp] - _u_c) * pow( _L_cal, 1 / _nu) * ( (_u[_qp] - _u_c)  < 0.0)
    + exp( - abs( (_u[_qp] - _u_c) * pow( _L_cal, 1.0 / _nu) ) / 2.0)
    , _gamma), 1.0 / _df)
    +
    ( 1.0 / _df) * (_u[_qp] * ( 1.0 - _u[_qp]))
    * pow( pow( _u_c, -_gamma) * pow( _L_cal, -_gamma / _nu)
    * pow( - (_u[_qp] - _u_c) * pow( _L_cal, 1 / _nu) * ( (_u[_qp] - _u_c)  < 0.0)
    + exp( - abs( (_u[_qp] - _u_c) * pow( _L_cal, 1.0 / _nu) ) / 2.0)
    , -_df + _gamma), 1.0 / _df)
    *
    0.5 * pow(_L_cal, 1.0 / _nu) * ( -2.0 * ( (_u[_qp] - _u_c)  < 0.0)
    - exp( - abs( (_u[_qp] - _u_c) * pow( _L_cal, 1.0 / _nu) ) / 2.0)
    * (( ( _u[_qp] - _u_c)  < 0.0) * -1.0 + ( (_u[_qp] - _u_c)  > 0.0) * 1.0)
    )
    ;
}

template <typename T>
Real
SplitCHWBaseTiger<T>::computeQpResidual()
{
  return computeMob() * _grad_u[_qp] * _grad_test[_i][_qp];
}

template <typename T>
Real
SplitCHWBaseTiger<T>::computeQpJacobian()
{
  ///return _mob[_qp] * _grad_phi[_j][_qp] * _grad_test[_i][_qp];
  return computeMob() * _grad_phi[_j][_qp] * _grad_test[_i][_qp];
}

template <typename T>
Real
SplitCHWBaseTiger<T>::computeQpOffDiagJacobian(unsigned int jvar)
{

  if (jvar == 0)
  {
    return computedMobdop() * _phi[_j][_qp] * _grad_u[_qp] * _grad_test[_i][_qp];
  }
  return 0.0;
}

#endif // SPLITCHWRESBASE_H
