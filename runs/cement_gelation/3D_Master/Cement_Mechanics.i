[Mesh]
  file = ./runs/cement_gelation/20170914_1/Cement_Gelation_out.e
[]

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]
[]

[AuxVariables]
  [./first_invariant]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./c]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[UserObjects]
  [./solution_uo]
    type = SolutionUserObject
    mesh = ./runs/cement_gelation/20170914_1/Cement_Gelation_out.e
    system_variables = c
  [../]
[]

[Functions]
  [./solution_fcn]
    type = SolutionFunction
    from_variable = c
    solution = solution_uo
  [../]
[]

[Modules/TensorMechanics/Master]
  [./all]
    strain = SMALL
    add_variables = true
    use_displaced_mesh = false
  [../]
[]

[AuxKernels]
  [./ker_first_invariant]
    type = RankTwoScalarAux
    rank_two_tensor = stress
    variable = first_invariant
    scalar_type = FirstInvariant
  [../]
  [./load_c]
     type = FunctionAux
     variable = c
     function = solution_fcn
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
  [./front_x]
    type = PresetBC
    variable = disp_z
    boundary = front
    value = 0.01
  [../]
  [./back_x]
    type = PresetBC
    variable = disp_z
    boundary = back
    value = 0.00
  [../]
[]

[Materials]
  [./stress]
    type = ComputeLinearElasticStress
  [../]
  [./youngs_modulus]
    type = DerivativeParsedMaterial
    f_name = youngs_modulus
    constant_names = 'c_cr'
    constant_expressions = '0.25'
    function = '0.00001 * c + if((c-c_cr) > 0 , (c - c_cr)^1.5, 0)'
    args = c
  [../]
  [./elasticity_tensor]
    type = ComputeVariableIsotropicElasticityTensor
    youngs_modulus = youngs_modulus
    poissons_ratio = 0.25
    args = c
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient

  l_max_its = 100
  nl_max_its = 100
  solve_type = NEWTON
  nl_abs_tol = 1e-8
  end_time = 116.0
  [./TimeStepper]
    # Turn on time stepping
    type = ConstantDT
    dt = 1.0
  [../]
[]

[Outputs]
  execute_on = 'timestep_end'
  exodus = true
[]
