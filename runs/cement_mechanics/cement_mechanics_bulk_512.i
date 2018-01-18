[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 128
  ny = 128
  xmax = 1
  ymax = 1

  parallel_type = replicated
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./stress_11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_22]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_12]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./first_invariant]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./youngs_modulus]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[UserObjects]
  [./solution_uo]
    type = SolutionUserObject
    mesh = ./runs/cement_mechanics/youngs_modulus_in_512.e
    system_variables = youngs_modulus
    execute_on = initial
  [../]
[]

[ICs]
  [./uo_init]
    type = FunctionIC
    variable = youngs_modulus
    function = solution_fcn
  [../]
[]

[Functions]
  [./solution_fcn]
    type = SolutionFunction
    from_variable = youngs_modulus
    solution = solution_uo
  [../]
[]

[Modules/TensorMechanics/Master]
  [./all]
    strain = SMALL
    add_variables = true
  [../]
[]

[AuxKernels]
  [./ker_stress_11]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 0
    index_j = 0
    variable = stress_11
  [../]
  [./ker_stress_22]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 1
    index_j = 1
    variable = stress_22
  [../]
  [./ker_stress_12]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 0
    index_j = 1
    variable = stress_12
  [../]
  [./ker_first_invariant]
    type = RankTwoScalarAux
    rank_two_tensor = stress
    variable = first_invariant
    scalar_type = FirstInvariant
  [../]
[]

[BCs]
  [./top_y]
    type = PresetBC
    variable = disp_y
    boundary = top
    value = 0.01
  [../]
  [./bottom_y]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0.00
  [../]
  [./right_x]
    type = PresetBC
    variable = disp_x
    boundary = right
    value = 0.01
  [../]
  [./left_x]
    type = PresetBC
    variable = disp_x
    boundary = left
    value = 0.00
  [../]
[]

[Materials]
  [./stress]
    type = ComputeLinearElasticStress
  [../]
  [./temp]
    type = DerivativeParsedMaterial
    f_name = temp
    function = '1 * youngs_modulus'
    args = youngs_modulus
  [../]
  [./elasticity_tensor]
    type = ComputeVariableIsotropicElasticityTensor
    youngs_modulus = temp
    poissons_ratio = 0.25
    args = youngs_modulus
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Steady
  l_max_its = 100
  nl_max_its = 100
  solve_type = NEWTON
  nl_rel_tol = 1e-8
[]

[Outputs]
  exodus = true
[]
