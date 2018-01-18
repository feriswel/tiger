/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#ifndef DIRECTIONALDIFFUSIONMATERIAL_H
#define DIRECTIONALDIFFUSIONMATERIAL_H

#include "Material.h"

class DirectionalDiffusionMaterial;

template <>
InputParameters validParams<DirectionalDiffusionMaterial>();

/**
 * Set a material property to the norm of the gradient of a non-linear variable
 */
class DirectionalDiffusionMaterial : public Material
{
public:
  DirectionalDiffusionMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  const VariableGradient & _grad;
  MaterialProperty<Real> & _prop;
};

#endif // VARIABLEGRADIENTMATERIAL_H
