/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#include "DirectionalDiffusionMaterial.h"

template <>
InputParameters
validParams<DirectionalDiffusionMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addCoupledVar("variable", "Variable to compute the gradient magnitude of");
  params.addParam<MaterialPropertyName>("self_diffusion", 1.0, "The reaction rate used with the kernel");
  params.addRequiredParam<MaterialPropertyName>(
      "prop", "Material property to store the gradient magnitude in");
  return params;
}

DirectionalDiffusionMaterial::DirectionalDiffusionMaterial(const InputParameters & parameters)
  : Material(parameters),
    _grad(coupledGradient("variable")),
    _prop(declareProperty<Real>(getParam<MaterialPropertyName>("prop")))
{
}

void
DirectionalDiffusionMaterial::computeQpProperties()
{
  _prop[_qp] = _grad[_qp].norm();
}
