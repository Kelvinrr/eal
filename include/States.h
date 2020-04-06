#ifndef ALE_STATES_H
#define ALE_STATES_H

#include <vector>
#include <stdexcept>
#include <gsl/gsl_interp.h>

#include "ale.h"
#include "Util.h"

namespace ale {


/** A 3D cartesian vector */
struct Vec3d {
  double x;
  double y;
  double z;

  // Accepts an {x,y,z} vector
  Vec3d(const std::vector<double>& vec) {
    if (vec.size() != 3) {
      throw std::invalid_argument("Input vector must have 3 entries.");
    }
    x = vec[0];
    y = vec[1];
    z = vec[2];
  };

  Vec3d(double x, double y, double z) : x(x), y(y), z(z) {};
  Vec3d() : x(0.0), y(0.0), z(0.0) {};
};



/** A state vector with position and velocity*/
struct State {
  Vec3d position;
  Vec3d velocity;

  // Accepts a {x, y, z, vx, vy, vz} vector
  State(const std::vector<double>& vec) {
    if (vec.size() != 6) {
      throw std::invalid_argument("Input vector must have 6 entries.");
    }
    position = {vec[0], vec[1], vec[2]};
    velocity = {vec[3], vec[4], vec[5]};
  };

  State(Vec3d position, Vec3d velocity) : position(position), velocity(velocity) {};
  State() {};
};

class States {
  public:
    // Constructors
    States();
    States(const std::vector<double>& ephemTimes, const std::vector<ale::Vec3d>& positions,
           int refFrame=1);

    States(const std::vector<double>& ephemTimes, const std::vector<ale::Vec3d>& positions,
           const std::vector<ale::Vec3d>& velocities, int refFrame=1);

    States(const std::vector<double>& ephemTimes, const std::vector<ale::State>& states,
           int refFrame=1);

    ~States();

    // Getters
    std::vector<ale::State> getStates() const; //! Returns state vectors (6-element positions&velocities)
    std::vector<ale::Vec3d> getPositions() const; //! Returns the current positions
    std::vector<ale::Vec3d> getVelocities() const; //! Returns the current velocities
    std::vector<double> getTimes() const; //! Returns the current times
    int getReferenceFrame() const; //! Returns reference frame as NAIF ID
    bool hasVelocity() const; //! Returns true if any velocities have been provided

  /**
   * Returns a single state by interpolating state.
   * If the Cache has been minimized, a cubic hermite is used to interpolate the
   * position and velocity over the reduced cache.
   * If not, a standard gsl interpolation will be done.
   *
   * @param time Time to get a value at
   * @param interp Interpolation type to use. Will be ignored if cache is minimized.
   *
   * @return ale::State
   */
    State getState(double time, interpolation interp=LINEAR) const;

    /** Gets a position at a single time. Operates the same way as getState() **/
    Vec3d getPosition(double time, interpolation interp=LINEAR) const;

    /** Gets a velocity at a single time. Operates the same way as getState() **/
    Vec3d getVelocity(double time, interpolation interp=LINEAR) const;

    /** Returns the first ephemeris time **/
    double getStartTime();

    /** Returns the last ephemeris time **/
    double getStopTime();

  /**
   * Perform a cache reduction. After running this, getStates(), getPositions(),
   * and getVelocities() will return vectors of reduced size, and getState(),
   * getPosition(), and getVelocity() will
   * returns values interpolated over the reduced cache using a cubic hermite spline
   *
   * Adapted from Isis::SpicePosition::reduceCache().
   *
   * @param tolerance Maximum error between hermite approximation and original value.
   */
    States minimizeCache(double tolerance=0.01);

  private:

  /**
   * Calculates the points (indicies) which need to be kept for the hermite spline to
   * interpolate between to mantain a maximum error of tolerance.
   *
   * Adapted from Isis::SpicePosition::HermiteIndices.
   *
   * @param tolerance Maximum error between hermite approximation and original value.
   * @param indexList The list of indicies that need to be kept.
   * @param baseTime Scaled base time for fit
   * @param timeScale Time scale for fit.
   *
   * @return std::vector<int>
   */
    std::vector<int> hermiteIndices(double tolerance, std::vector <int> indexList,
                                    double baseTime, double timeScale);
    std::vector<State> m_states; //! Represent at states internally to keep pos, vel together
    std::vector<double> m_ephemTimes; //! Time in seconds
    int m_refFrame;  //! Naif IDs for reference frames
  };
}

#endif