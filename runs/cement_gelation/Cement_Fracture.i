[Mesh]
  file = ./runs/cement_gelation/2D_Master/Cement_Gelation_out.e
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./d]
  [../]
  [./b]
  [../]
  [./c]
  [../]
[]

[UserObjects]
  [./solution_uo]
    type = SolutionUserObject
    mesh = ./runs/cement_gelation/2D_Master/Cement_Gelation_out.e
    system_variables = c
    timestep = 100
  [../]
[]

[AuxVariables]
  [./resid_x]
  [../]
  [./resid_y]
  [../]
  [./stress_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Functions]
  [./tfunc]
    type = ParsedFunction
    value = '0.5 * t'
  [../]
[]

[Kernels]
  [./pfbulk]
    type = SplitPFFractureBulkRate
    variable = d
    width = 0.08
    beta = b
    viscosity = 1e-1
    gc = 'gc_prop'
    G0 = 'G0_pos'
    dG0_dstrain = 'dG0_pos_dstrain'
  [../]
  [./DynamicTensorMechanics]
    displacements = 'disp_x disp_y'
    save_in = 'resid_x resid_y'
  [../]
  [./solid_x]
    type = PhaseFieldFractureMechanicsOffDiag
    variable = disp_x
    component = 0
    c = d
  [../]
  [./solid_y]
    type = PhaseFieldFractureMechanicsOffDiag
    variable = disp_y
    component = 1
    c = d
  [../]
  [./dcdt]
    type = TimeDerivative
    variable = d
  [../]
  [./pfintvar]
    type = Reaction
    variable = b
  [../]
  [./pfintcoupled]
    type = LaplacianSplit
    variable = b
    c = d
  [../]

  [./load_c]
     type = SolutionAux
     variable = c
     solution = solution_uo
  [../]
[]

[AuxKernels]
  [./stress_yy]
    type = RankTwoAux
    variable = stress_yy
    rank_two_tensor = stress
    index_j = 1
    index_i = 1
    execute_on = timestep_end
  [../]
[]

[BCs]
  [./ydisp]
    type = FunctionPresetBC
    variable = disp_y
    boundary = top
    function = tfunc
  [../]
  [./yfix]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0
  [../]
[]

[Materials]
  [./pfbulkmat]
    type = PFFracBulkRateMaterial
    gc = 1e-3
  [../]
  [./elastic]
    type = LinearIsoElasticPFDamage
    c = d
    kdamage = 1e-8
  [../]
  [./strain]
    type = ComputeSmallStrain
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

[Postprocessors]
  [./resid_x]
    type = NodalSum
    variable = resid_x
    boundary = 2
  [../]
  [./resid_y]
    type = NodalSum
    variable = resid_y
    boundary = 2
  [../]
[]

[Preconditioning]
  [./smp]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient

  solve_type = PJFNK
  petsc_options_iname = '-pc_type -ksp_gmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      31                  preonly       lu           1'

  nl_rel_tol = 1e-8
  l_max_its = 10
  nl_max_its = 10

  dt = 1e-4
  dtmin = 1e-4
  num_steps = 200
[]

[Outputs]
  exodus = true
[]
