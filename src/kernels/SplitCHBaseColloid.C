/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "SplitCHBaseColloid.h"

template <>
InputParameters
validParams<SplitCHBaseColloid>()
{
  InputParameters params = validParams<Kernel>();

  return params;
}

SplitCHBaseColloid::SplitCHBaseColloid(const InputParameters & parameters)
  : DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>(parameters)
  {

  }

/*Real //Example of what the virtual function should look like
SplitCHBaseColloid::computeDFDC(PFFunctionType type)
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
SplitCHBaseColloid::computeQpResidual()
{
  Real f_prime_zero = computeDFDC(Residual);
  Real e_prime = computeDEDC(Residual);

  Real residual = (f_prime_zero + e_prime) * _test[_i][_qp];

  return residual;
}

Real
SplitCHBaseColloid::computeQpJacobian()
{
  Real df_prime_zero_dc = computeDFDC(Jacobian);
  Real de_prime_dc = computeDEDC(Jacobian);

  Real jacobian = (df_prime_zero_dc + de_prime_dc) * _test[_i][_qp];

  return jacobian;
}

Real
SplitCHBaseColloid::computeQpOffDiagJacobian(unsigned int /*jvar*/)
{
  return 0.0;
}

Real SplitCHBaseColloid::computeDFDC(PFFunctionType /*type*/) { return 0.0; }

Real SplitCHBaseColloid::computeDEDC(PFFunctionType /*type*/) { return 0.0; }
