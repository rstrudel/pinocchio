#ifndef __se3_motion_hpp__
#define __se3_motion_hpp__

#include <Eigen/Core>
#include <Eigen/Geometry>
#include "pinocchio/spatial/fwd.hpp"
#include "pinocchio/spatial/force.hpp"

namespace se3
{
  template<typename _Scalar, int _Options>
  class MotionTpl
  {
  public:
    typedef _Scalar Scalar;
    enum { Options = _Options };
    typedef Eigen::Matrix<Scalar,3,1,Options> Vector3;
    typedef Eigen::Matrix<Scalar,3,3,Options> Matrix3;
    typedef Eigen::Matrix<Scalar,6,1,Options> Vector6;
    typedef Eigen::Matrix<Scalar,6,6,Options> Matrix6;
    typedef ForceTpl<Scalar,Options> Force;
    typedef SE3Tpl<Scalar,Options> SE3;
    enum { LINEAR = 0, ANGULAR = 3 };

  public:
    // Constructors
    MotionTpl() : m_w(), m_v() {}
    MotionTpl(const Vector3 & v, const Vector3 & w) : m_w(w), m_v(v) {}
    MotionTpl(const Vector6 & v) : 
      m_w(v.template segment<3>(ANGULAR)),
      m_v(v.template segment<3>(LINEAR)) {}

    static MotionTpl Zero()   { return MotionTpl(Vector3::Zero(),  Vector3::Zero());   }
    static MotionTpl Random() { return MotionTpl(Vector3::Random(),Vector3::Random()); }
    Vector6 toVector() const
    {
      Vector6 v;
      v.template segment<3>(ANGULAR) = m_w;
      v.template segment<3>(LINEAR)  = m_v;
      return v;
    }
    operator Vector6 () const { return toVector(); }

    // Getters
    const Vector3 & angular() const { return m_w; }
    const Vector3 & linear()  const { return m_v; }
    void angular(const Vector3 & w) { m_w=w; }
    void linear (const Vector3 & v) { m_v=v; }

    // Arithmetic operators
    MotionTpl & operator=(const Vector6 & v)
    {
      m_w = v.template segment<3>(ANGULAR);
      m_v = v.template segment<3>(LINEAR);
      return *this;
    }

    MotionTpl operator-() const
    {
      return MotionTpl(-m_v, -m_w);
    }

    MotionTpl operator+(const MotionTpl & v2) const
    {
      return MotionTpl(m_v+v2.m_v,m_w+v2.m_w);
    }
    MotionTpl& operator+=(const MotionTpl & v2)
    {
      m_v+=v2.m_v;
      m_w+=v2.m_w;
      return *this;
    }


    // MotionTpl operator*(Scalar a) const
    // {
    //   return MotionTpl(m_w*a, m_v*a);
    // }

    // friend MotionTpl operator*(Scalar a, const MotionTpl & mv)
    // {
    //   return MotionTpl(mv.w()*a, mv.v()*a);
    // }

    MotionTpl cross(const MotionTpl& v2) const
    {
      return MotionTpl( m_v.cross(v2.m_w)+m_w.cross(v2.m_v),
			m_w.cross(v2.m_w) );
    }

    ForceTpl<Scalar,Options> cross(const ForceTpl<Scalar,Options>& phi) const
    {
      return ForceTpl<Scalar,Options>
	( m_w.cross(phi.linear()),
	  m_w.cross(phi.angular())+m_v.cross(phi.linear()) );
    }


    // template<typename S,int O>
    // friend MotionTpl<S,O> operator^( const MotionTpl<S,O> &, const MotionTpl<S,O> & );
    // template<typename S,int O>
    // friend ForceTpl<S,O> operator^( const MotionTpl<S,O> &, const ForceTpl<S,O> & );

    MotionTpl se3Action(const SE3 & m) const
    {
      Vector3 Rw = static_cast<Vector3>(m.rotation() * angular());
      return MotionTpl(m.rotation()*linear() + m.translation().cross(Rw), Rw);
    }
    /// bv = aXb.actInv(av)
    MotionTpl se3ActionInverse(const SE3 & m) const
    {
      return MotionTpl(m.rotation().transpose()*(linear()-m.translation().cross(angular())),
		       m.rotation().transpose()*angular());
    }




    friend std::ostream & operator << (std::ostream & os, const MotionTpl & mv)
    {
      os << "v =\n" << mv.linear() << std::endl
      << "w =\n" << mv.angular() << std::endl;
      return os;
    }

  public:
  private:
    Vector3 m_w;
    Vector3 m_v;
  };

  typedef MotionTpl<double> Motion;

} // namespace se3

#endif // ifndef __se3_motion_hpp__