#
# Example problem showing how to use the DerivativeParsedMaterial with SplitCHParsed.
# The free energy is identical to that from SplitCHMath, f_bulk = 1/4*(1-c)^2*(1+c)^2.
#

[Mesh]
  type = GeneratedMesh
  dim = 2 #######CHANGE FOR 3D
  nx = 64
  ny = 64
  #nz = 32 #######UNCOMMENT FOR 3D
  xmax = 100
  ymax = 100
  #zmax = 60 #######UNCOMMENT FOR 3D
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Variables]
  #PHASE-FIELD VARIABLES
  [./c]
  [../]
  [./w]
  [../]
  [./eta]
  [../]

  #MECHANICS VARIABLES
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[Kernels]
  [./TensorMechanics]
    displacements = 'disp_x disp_y'
    use_displaced_mesh = false
  [../]
[]

[AuxVariables]
  #PHASE-FIELD AUX-VARIABLES
  [./local_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]

  #MECHANICS AUX-VARIABLES
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
[]

[ICs]
  [./cIC]
    type = RandomIC
    variable = c
    min = 0.01
    max = 0.02
  [../]
[]

[Kernels]
  #PHASE-FIELD
  [./c_dot]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
  [./c_res]
    type = SplitCHParsed
    variable = c
    f_name = fbulk
    kappa_name = kappa_c
    w = w
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = w
    mob_name = M
  [../]
  [./nucleation]
    type = ReactiveSource
    variable = w
    v = c
    mob_name = R0
    gamma = 0.2
    v_max = 0.74
  [../]

  #ORDER PARAMETER
  [./eta_bulk]
    type = AllenCahn
    variable = eta
    f_name = fbulk
    mob_name = k_eta
  [../]
  [./eta_interface]
    type = ACInterface
    variable = eta
    kappa_name = kappa_eta
    mob_name = k_eta
  [../]
  [./eta_dot]
    type = TimeDerivative
    variable = eta
  [../]
[]

[AuxKernels]
  #PHASE-FIELD
  [./local_energy]
    type = TotalFreeEnergy
    variable = local_energy
    f_name = fbulk
    interfacial_vars = c
    kappa_names = kappa_c
    execute_on = timestep_end
  [../]

  #MECHANICS
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
  #PHASE-FIELD
  [./Periodic]
    [./all]
      auto_direction = 'x y' #######ADD Z FOR 3D
    [../]
  [../]

  #MECHANICS (CONSTANT DISPLACEMENT)
  [./right]
    type = PresetBC
    variable = disp_x
    boundary = right
    value = 0.00
  [../]
  [./top]
    type = PresetBC
    variable = disp_y
    boundary = top
    value = 0.00
  [../]
  [./left]
    type = PresetBC
    variable = disp_x
    boundary = left
    value = 0.00
  [../]
  [./bottom]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0.00
  [../]
[]

[Materials]

  [./generic_mats]
    #Reaction rate according to classical nucleation theory in an open volume at a
    #concentration c
    type = GenericConstantMaterial
    prop_names  = 'kappa_c kappa_eta R0 k_eta'
    prop_values = '2.0 2.0 0.001 1.0'
  [../]

  #PHASEF-FIELD MATERIALS
  [./mob]
    type = DerivativeParsedMaterial
    args = c
    f_name = M
    constant_names = 'c_cr L W gamma nu D0'
    constant_expressions = '0.37 0.3 1e-03 0.2 0.88 c_cr^-gamma'
    function = 'D0 * L^( - gamma / nu) * ( -( c - c_cr) * L^( 1 / nu) * if((c-c_cr) < 0 , 1, 0) + W * exp(-abs( (c-c_cr) * L^( 1 / nu) )/2/W))^(gamma)'
    derivative_order = 1
    outputs = exodus
  [../]

  [./free_energy_A]
    type = DerivativeParsedMaterial
    f_name = Fa
    args = 'c'
    function = ' c * log( c) + ( 0.74 - c) * log( 0.74 - c) + 4.0 * c * ( 0.74 - c) - 0.74 * log(0.74)'
    derivative_order = 2
    enable_jit = true
  [../]
  [./free_energy_B]
    type = DerivativeParsedMaterial
    f_name = Fb
    args = 'c'
    function = 'c*log(c) + (0.64 - c)*log(0.64 - c) + 4.0*c*(0.64 - c) - 0.64 * log(0.64)'
    derivative_order = 2
    enable_jit = true
  [../]

  [./free_energy]
    type = DerivativeTwoPhaseMaterial
    f_name = fbulk
    args = 'c'
    fa_name = Fa
    fb_name = Fb
    g = '16 * eta^2 * ( 1 - eta)^2'
    h = 'eta^3 * ( 6 * eta^2 - 15*eta + 10)'
    W = 1.0
    eta = eta

    enable_jit = true
    outputs = exodus
  [../]

  #MECHANICS MATERIALS
  [./strain_increment]
    type = ComputeIncrementalSmallStrain
    displacements = 'disp_x disp_y'
    eigenstrain_names = eigenstrain
  [../]
  [./stress]
    type = ComputeFiniteStrainElasticStress
  [../]
  [./elasticity_tensor]
    type = ComputeVariableIsotropicElasticityTensor
    poissons_ratio = 0.2
    args = 'c'
    youngs_modulus = youngs_modulus
  [../]
  [./eigenstrain]
    type = ComputeVolumetricEigenstrain
    incremental_form = true
    volumetric_materials = volume_strain
    eigenstrain_name = eigenstrain
    args = 'c'
  [../]

  #FUNCTIONS USED FOR THE MECHANICS
  [./modulus]
    type = DerivativeParsedMaterial
    f_name = youngs_modulus
    constant_names = 'c_cr'
    constant_expressions = '0.37'
    function = '190 * (c - c_cr)^3.0 * if((c-c_cr) < 0 , 1, 0)'
    args = c
  [../]
  [./volume_strain]
    type = DerivativeParsedMaterial
    f_name = volume_strain
    constant_names = 'eps1 c_cr'
    constant_expressions = '1.0 0.37'
    args = c
    function = 'eps1 * (c - c_cr) * if((c-c_cr) < 0 , 1, 0)'
  [../]
[]

[Postprocessors]
  [./total_free_energy]
    type = ElementIntegralVariablePostprocessor
    variable = local_energy
  [../]
  [./mean_packing]
    type = ElementAverageValue
    variable = c
  [../]
[]

[Preconditioning]
  [./cw_coupling]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  scheme = bdf2

  petsc_options_iname = '-pc_type -sub_pc_type'
  petsc_options_value = 'asm      lu'

  l_max_its = 15
  l_tol = 1e-3
  nl_max_its = 15
  nl_rel_tol = 1e-7
  end_time = 500.0

  [./TimeStepper]
    # Turn on time stepping
    type = IterationAdaptiveDT
    dt = 0.01
    cutback_factor = 0.8
    growth_factor = 1.5
    optimal_iterations = 7
  [../]
[]

[Outputs]
  exodus = true
  csv = true
  print_perf_log = true
[]
