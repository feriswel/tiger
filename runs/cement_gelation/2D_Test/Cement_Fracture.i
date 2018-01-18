#[Mesh]
#  file = ./runs/cement_gelation/2D_Test/Cement_Gelation_out.e
#[]

[Mesh]
  type = GeneratedMesh
  dim = 2 #######CHANGE FOR 3D
  nx = 128
  ny = 128
  #nz = 64 #######UNCOMMENT FOR 3D
  xmax = 50
  ymax = 50
  #zmax = 50 #######UNCOMMENT FOR 3D
[]


[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./d] #damage parameter: 0 represents no damage, 1 represents complete damage
  [../]
  [./b]
  [../]
[]

[UserObjects]
  [./solution_uo]
    type = SolutionUserObject
    mesh = ./runs/cement_gelation/2D_Test/Cement_Gelation_out.e
    system_variables = c
    timestep = 300
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
  [./trace_stress]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./first_invariant]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./strain_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./c]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Functions]
  [./tfunc]
    type = ParsedFunction
    value = 't'
  [../]
  [./solution_fcn]
    type = SolutionFunction
    from_variable = c
    solution = solution_uo
  [../]
[]

[Kernels]
  [./pfbulk]
    type = SplitPFFractureBulkRate
    variable = d
    width = 0.5
    beta = b
    viscosity = 1e-1 #as visc ->0, the fracture process becomes rate independent
    gc = 'gc_prop'
    G0 = 'G0_pos'
    dG0_dstrain = 'dG0_pos_dstrain'
  [../]
  [./DynamicTensorMechanics]
    displacements = 'disp_x disp_y'
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
  [./d_dot]
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
[]

[ICs]
  [./uo_init]
    type = FunctionIC
    variable = c
    function = solution_fcn
  [../]
[]

[AuxKernels]
  [./stress_11]
    type = RankTwoAux
    variable = stress_11
    rank_two_tensor = stress
    index_j = 0
    index_i = 0
    execute_on = timestep_end
  [../]
  [./stress_22]
    type = RankTwoAux
    variable = stress_22
    rank_two_tensor = stress
    index_j = 1
    index_i = 1
    execute_on = timestep_end
  [../]
  [./stress_12]
    type = RankTwoAux
    variable = stress_12
    rank_two_tensor = stress
    index_j = 0
    index_i = 1
    execute_on = timestep_end
  [../]
  [./ker_trace_stress]
    type = RankTwoScalarAux
    variable = trace_stress
    rank_two_tensor = stress
    scalar_type = VolumetricStrain
    execute_on = timestep_end
  [../]
  [./ker_first_invariant]
    type = RankTwoScalarAux
    rank_two_tensor = stress
    variable = first_invariant
    scalar_type = FirstInvariant
    execute_on = timestep_end
  [../]
  [./ker_strain_energy]
    type = MechStrainEnergyAux
    variable = strain_energy
    execute_on = timestep_end
  [../]

  [./load_c]
     type = FunctionAux
     variable = c
     function = solution_fcn
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

  [./xdisp]
    type = FunctionPresetBC
    variable = disp_x
    boundary = right
    function = tfunc
  [../]
  [./xfix]
    type = PresetBC
    variable = disp_x
    boundary = left
    value = 0
  [../]
[]

[Materials]
  [./pfbulkmat]
    type = PFFracBulkRateMaterial
    gc = 1e-2 #fracture toughness
  [../]
  [./elastic]
    type = LinearIsoElasticPFDamage
    c = d
    kdamage = 1e-8 #stiffness of the damaged region
  [../]

  [./small_strain]
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
  [./total_strain_energy]
    type = ElementIntegralVariablePostprocessor
    variable = strain_energy
  [../]
  [./total_trace_stress]
    type = ElementIntegralVariablePostprocessor
    variable = trace_stress
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

  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-8
  l_max_its = 30
  nl_max_its = 50

  num_steps = 1500

  [./TimeStepper]
    # Turn on time stepping
    type = IterationAdaptiveDT
    dt = 1e-2
    cutback_factor = 0.8
    growth_factor = 1.5
    optimal_iterations = 7
  [../]
[]

[Outputs]
  exodus = true
  csv = true
[]
