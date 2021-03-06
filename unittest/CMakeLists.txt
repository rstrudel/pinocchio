#
# Copyright (c) 2015-2019 CNRS INRIA
# Copyright (c) 2015 Wandercraft, 86 rue de Paris 91400 Orsay, France.
#

# --- MACROS ------------------------------------------------------------------
# --- MACROS ------------------------------------------------------------------
# --- MACROS ------------------------------------------------------------------

MACRO(ADD_TEST_CFLAGS target flag)
  SET_PROPERTY(TARGET ${target} APPEND_STRING PROPERTY COMPILE_FLAGS " ${flag}")
ENDMACRO(ADD_TEST_CFLAGS)

MACRO(ADD_PINOCCHIO_UNIT_TEST NAME)
  SET(PKGS ${ARGN}) # Optional arguments

  SET(TEST_NAME "${NAME}-test")
  ADD_UNIT_TEST(${TEST_NAME} ${NAME}.cpp)
  SET_TARGET_PROPERTIES(${TEST_NAME} PROPERTIES LINKER_LANGUAGE CXX)
  TARGET_INCLUDE_DIRECTORIES(${TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

  ADD_TEST_CFLAGS(${TEST_NAME} "-DBOOST_TEST_DYN_LINK") 
  SET(MODULE_NAME "${NAME}Test")
  STRING(REGEX REPLACE "-" "_" MODULE_NAME ${MODULE_NAME})
  ADD_TEST_CFLAGS(${TEST_NAME} "-DBOOST_TEST_MODULE=${MODULE_NAME}") 

  PKG_CONFIG_USE_DEPENDENCY(${TEST_NAME} eigen3)
  FOREACH(PKG ${PKGS})
    PKG_CONFIG_USE_DEPENDENCY(${TEST_NAME} ${PKG})
  ENDFOREACH(PKG)

  IF(HPP_FCL_FOUND)
    PKG_CONFIG_USE_DEPENDENCY(${TEST_NAME} hpp-fcl)
  ENDIF(HPP_FCL_FOUND)

  TARGET_LINK_LIBRARIES(${TEST_NAME} ${PROJECT_NAME})
  TARGET_LINK_LIBRARIES(${TEST_NAME} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
ENDMACRO(ADD_PINOCCHIO_UNIT_TEST)

# --- RULES -------------------------------------------------------------------
# --- RULES -------------------------------------------------------------------
# --- RULES -------------------------------------------------------------------

# Math components
ADD_PINOCCHIO_UNIT_TEST(eigen-basic-op)
ADD_PINOCCHIO_UNIT_TEST(eigen-tensor)
ADD_PINOCCHIO_UNIT_TEST(sincos)
ADD_PINOCCHIO_UNIT_TEST(quaternion)
ADD_PINOCCHIO_UNIT_TEST(rotation)

# Pinocchio features
ADD_PINOCCHIO_UNIT_TEST(spatial)
ADD_PINOCCHIO_UNIT_TEST(symmetric)
ADD_PINOCCHIO_UNIT_TEST(aba)
ADD_PINOCCHIO_UNIT_TEST(rnea)
ADD_PINOCCHIO_UNIT_TEST(crba)
ADD_PINOCCHIO_UNIT_TEST(centroidal)
ADD_PINOCCHIO_UNIT_TEST(com)
ADD_PINOCCHIO_UNIT_TEST(joint-jacobian)
ADD_PINOCCHIO_UNIT_TEST(cholesky)
ADD_PINOCCHIO_UNIT_TEST(contact-dynamics)
ADD_PINOCCHIO_UNIT_TEST(sample-models)
ADD_PINOCCHIO_UNIT_TEST(kinematics)

IF(URDFDOM_FOUND)
  ADD_PINOCCHIO_UNIT_TEST(urdf urdfdom)
  ADD_TEST_CFLAGS(urdf-test '-DPINOCCHIO_MODEL_DIR=\\\"${PINOCCHIO_MODEL_DIR}\\\"')

  ADD_PINOCCHIO_UNIT_TEST(value urdfdom)
  ADD_TEST_CFLAGS(value-test '-DPINOCCHIO_MODEL_DIR=\\\"${PINOCCHIO_MODEL_DIR}\\\"')
  IF(HPP_FCL_FOUND)
    ADD_PINOCCHIO_UNIT_TEST(geom urdfdom)
    ADD_TEST_CFLAGS(geom-test '-DPINOCCHIO_MODEL_DIR=\\\"${PINOCCHIO_MODEL_DIR}\\\"')
    ADD_PINOCCHIO_UNIT_TEST(srdf urdfdom)
    ADD_TEST_CFLAGS(srdf-test '-DPINOCCHIO_MODEL_DIR=\\\"${PINOCCHIO_MODEL_DIR}\\\"')
  ENDIF(HPP_FCL_FOUND)
ENDIF(URDFDOM_FOUND)

IF(LUA5_FOUND)
  ADD_PINOCCHIO_UNIT_TEST(lua ${LUA5_PACKAGE})
  ADD_TEST_CFLAGS(lua-test '-DPINOCCHIO_MODEL_DIR=\\\"${PINOCCHIO_MODEL_DIR}\\\"')
ENDIF(LUA5_FOUND)

IF(BUILD_PYTHON_INTERFACE)
  ADD_PINOCCHIO_UNIT_TEST(python_parser eigenpy)
  ADD_TEST_CFLAGS(python_parser-test '-DPINOCCHIO_MODEL_DIR=\\\"${PINOCCHIO_MODEL_DIR}\\\"')

  TARGET_LINK_LIBRARIES(python_parser-test ${PYWRAP})
  TARGET_LINK_LIBRARIES(python_parser-test ${PYTHON_LIBRARIES})
  SET(ENV_VARIABLES "PYTHONPATH=${CMAKE_BINARY_DIR}/bindings/python:$ENV{PYTHONPATH}")
  IF(APPLE)
    LIST(APPEND ENV_VARIABLES "LD_LIBRARY_PATH=$ENV{LD_LIBRARY_PATH}")
    LIST(APPEND ENV_VARIABLES "DYLD_LIBRARY_PATH=$ENV{DYLD_LIBRARY_PATH}")
  ENDIF(APPLE)
  SET_PROPERTY(TEST python_parser-test PROPERTY ENVIRONMENT ${ENV_VARIABLES})
  ADD_SUBDIRECTORY(python)
ENDIF(BUILD_PYTHON_INTERFACE)

ADD_PINOCCHIO_UNIT_TEST(model)
ADD_PINOCCHIO_UNIT_TEST(data)
ADD_PINOCCHIO_UNIT_TEST(constraint)
ADD_PINOCCHIO_UNIT_TEST(joints)
ADD_PINOCCHIO_UNIT_TEST(compute-all-terms)
ADD_PINOCCHIO_UNIT_TEST(energy)
ADD_PINOCCHIO_UNIT_TEST(frames)
ADD_PINOCCHIO_UNIT_TEST(joint-configurations)
ADD_PINOCCHIO_UNIT_TEST(joint-generic)
ADD_PINOCCHIO_UNIT_TEST(explog)
ADD_PINOCCHIO_UNIT_TEST(finite-differences)
ADD_PINOCCHIO_UNIT_TEST(visitor)
ADD_PINOCCHIO_UNIT_TEST(algo-check)
ADD_PINOCCHIO_UNIT_TEST(joint-composite)
ADD_PINOCCHIO_UNIT_TEST(joint-mimic)

ADD_PINOCCHIO_UNIT_TEST(liegroups)
ADD_PINOCCHIO_UNIT_TEST(regressor)
ADD_PINOCCHIO_UNIT_TEST(version)
ADD_PINOCCHIO_UNIT_TEST(copy)
MAKE_DIRECTORY("${CMAKE_CURRENT_BINARY_DIR}/serialization-data")
ADD_PINOCCHIO_UNIT_TEST(serialization)
ADD_TEST_CFLAGS(serialization-test '-DTEST_SERIALIZATION_FOLDER=\\\"${CMAKE_CURRENT_BINARY_DIR}/serialization-data\\\"')

# Derivatives algo
ADD_PINOCCHIO_UNIT_TEST(kinematics-derivatives)
ADD_PINOCCHIO_UNIT_TEST(rnea-derivatives)
ADD_PINOCCHIO_UNIT_TEST(aba-derivatives)
ADD_PINOCCHIO_UNIT_TEST(centroidal-derivatives)
ADD_PINOCCHIO_UNIT_TEST(center-of-mass-derivatives)
ADD_PINOCCHIO_UNIT_TEST(contact-dynamics-derivatives)

# Automatic differentiation
IF(CPPAD_FOUND AND BUILD_WITH_AUTODIFF_SUPPORT)
  ADD_PINOCCHIO_UNIT_TEST(cppad-basic cppad)
  ADD_PINOCCHIO_UNIT_TEST(cppad-spatial cppad)
  ADD_PINOCCHIO_UNIT_TEST(cppad-joints cppad)
  ADD_PINOCCHIO_UNIT_TEST(cppad-algo cppad)
  ADD_PINOCCHIO_UNIT_TEST(cppad-algo-derivatives cppad)

  IF(CPPADCG_FOUND AND BUILD_WITH_CODEGEN_SUPPORT)
    ADD_PINOCCHIO_UNIT_TEST(cppadcg-basic cppadcg)
    SET_PROPERTY(TARGET cppadcg-basic-test PROPERTY CXX_STANDARD 11)
    TARGET_LINK_LIBRARIES(cppadcg-basic-test ${CMAKE_DL_LIBS})
    ADD_PINOCCHIO_UNIT_TEST(cppadcg-algo cppadcg)
    SET_PROPERTY(TARGET cppadcg-algo-test PROPERTY CXX_STANDARD 11)
    TARGET_LINK_LIBRARIES(cppadcg-algo-test ${CMAKE_DL_LIBS})
  ENDIF(CPPADCG_FOUND AND BUILD_WITH_CODEGEN_SUPPORT)

ENDIF(CPPAD_FOUND AND BUILD_WITH_AUTODIFF_SUPPORT)

IF(CASADI_FOUND AND BUILD_WITH_CASADI_SUPPORT)
  ADD_PINOCCHIO_UNIT_TEST(casadi-basic casadi)
  SET_PROPERTY(TARGET casadi-basic-test PROPERTY CXX_STANDARD 11)
  ADD_PINOCCHIO_UNIT_TEST(casadi-spatial casadi)
  SET_PROPERTY(TARGET casadi-spatial-test PROPERTY CXX_STANDARD 11)
  ADD_PINOCCHIO_UNIT_TEST(casadi-joints casadi)
  SET_PROPERTY(TARGET casadi-joints-test PROPERTY CXX_STANDARD 11)
  ADD_PINOCCHIO_UNIT_TEST(casadi-algo casadi)
  SET_PROPERTY(TARGET casadi-algo-test PROPERTY CXX_STANDARD 11)
  ADD_PINOCCHIO_UNIT_TEST(casadi-algo-derivatives casadi)
  SET_PROPERTY(TARGET casadi-algo-derivatives-test PROPERTY CXX_STANDARD 11)
ENDIF(CASADI_FOUND AND BUILD_WITH_CASADI_SUPPORT)

