#ifndef EAL_INCLUDE_EAL_H
#define EAL_INCLUDE_EAL_H

#include <json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

using namespace std;

namespace eal {

  json constructStateFromIsd(const string positionRotationData);

  vector<double> getPosition(vector<vector<double>> coords, vector<double> times,
                             string interp, double time);
  vector<double> getVelocity(vector<vector<double>> coords, vector<double> times,
                             string interp, double time, bool interpolation);

  vector<double> getPosition(vector<double> coeffs, string interp, double time);
  vector<double> getVelocity(vector<double> coeffs, string interp, double time);

  vector<double> getRotation(string from, string to, vector<vector<double>> rotations,
                             vector<double> times, string interp, double time);
  vector<double> getAngularVelocity(string from, string to, vector<vector<double>> rotations,
                                    vector<double> times, string interp, double time, bool interpolation);

  vector<double> getRotation(string from, string to, vector<double> coefficients,
                             string interp, double time);
  vector<double> getAngularVelocity(string from, string to, vector<double> coefficients,
                                    string interp, double time);

}

#endif // EAL_H