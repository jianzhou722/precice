#include "ScaleByDtAction.hpp"
#include "mesh/Data.hpp"
#include "mesh/Edge.hpp"
#include "mesh/Mesh.hpp"

namespace precice
{
namespace action
{

ScaleByDtAction::ScaleByDtAction(
    Timing               timing,
    int                  sourceDataID,
    int                  targetDataID,
    const mesh::PtrMesh &mesh,
    Scaling              scaling)
    : Action(timing, mesh),
      _sourceData(mesh->data(sourceDataID)),
      _targetData(mesh->data(targetDataID)),
      _scaling(scaling)
{
  assertion(_sourceData->getDimensions() == _targetData->getDimensions(),
            _sourceData->getDimensions(), _targetData->getDimensions());
}

void ScaleByDtAction::performAction(
    double time,
    double dt,
    double computedPartFullDt,
    double fullDt)
{
  TRACE(dt, computedPartFullDt, fullDt);
  auto &sourceValues = _sourceData->values();
  auto &targetValues = _targetData->values();
  assertion(sourceValues.size() == targetValues.size(),
            sourceValues.size(), targetValues.size());
  if (_scaling == SCALING_BY_COMPUTED_DT_RATIO) {
    double scaling = dt / fullDt;
    DEBUG("Scale by computed dt ratio " << scaling);
    for (int i = 0; i < targetValues.size(); i++) {
      targetValues[i] = sourceValues[i] * scaling;
    }
  } else if (_scaling == SCALING_BY_DT) {
    DEBUG("Scale by dt " << fullDt);
    for (int i = 0; i < targetValues.size(); i++) {
      targetValues[i] = sourceValues[i] * fullDt;
    }
  } else {
    assertion(_scaling == SCALING_BY_COMPUTED_DT_PART_RATIO, _scaling);
    double scaling = computedPartFullDt / fullDt;
    DEBUG("Scale by computed dt part ratio " << scaling);
    for (int i = 0; i < targetValues.size(); i++) {
      targetValues[i] = sourceValues[i] * scaling;
    }
  }
}

} // namespace action
} // namespace precice
