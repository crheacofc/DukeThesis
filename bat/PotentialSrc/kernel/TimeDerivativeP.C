/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "TimeDerivativeP.h"

// MOOSE includes
#include "Assembly.h"
#include "MooseVariable.h"

// libMesh includes
#include "libmesh/quadrature.h"

template <>
InputParameters
validParams<TimeDerivativeP>()
{
  InputParameters params = validParams<TimeKernel>();
  params.addParam<bool>("lumping", false, "True for mass matrix lumping, false otherwise");
  params.addRequiredParam<UserObjectName>(
      "fluid_density",
      "A RichardsDensity UserObject that defines the fluid density as a function of pressure.");
  return params;
}

TimeDerivativeP::TimeDerivativeP(const InputParameters & parameters)
  : TimeKernel(parameters), _lumping(getParam<bool>("lumping")),
   _density(getUserObject<RichardsDensity>("fluid_density")),
    _porosity(getMaterialProperty<Real>("porosity"))
{
}

Real
TimeDerivativeP::computeQpResidual()
{
    double density = _density.density(_u[_qp]);
  return _test[_i][_qp] * _porosity[_qp] * density * (1-_u_dot[_qp]);
}

Real
TimeDerivativeP::computeQpJacobian()
{
  double density = _density.density(_u[_qp]);

  return -_test[_i][_qp] * _phi[_j][_qp] * _porosity[_qp] * density * _du_dot_du[_qp];
}

void
TimeDerivativeP::computeJacobian()
{
  if (_lumping)
  {
    DenseMatrix<Number> & ke = _assembly.jacobianBlock(_var.number(), _var.number());

    for (_i = 0; _i < _test.size(); _i++)
      for (_j = 0; _j < _phi.size(); _j++)
        for (_qp = 0; _qp < _qrule->n_points(); _qp++)
          ke(_i, _i) += _JxW[_qp] * _coord[_qp] * computeQpJacobian();
  }
  else
    TimeDerivativeP::computeJacobian();
}
