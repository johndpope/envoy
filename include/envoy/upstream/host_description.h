#pragma once

#include "envoy/stats/stats_macros.h"
#include "envoy/upstream/outlier_detection.h"

namespace Upstream {

/**
 * All per host stats. @see stats_macros.h
 */
// clang-format off
#define ALL_HOST_STATS(COUNTER, GAUGE)                                                             \
  COUNTER(cx_total)                                                                                \
  GAUGE  (cx_active)                                                                               \
  COUNTER(cx_connect_fail)                                                                         \
  COUNTER(rq_total)                                                                                \
  GAUGE  (rq_active)
// clang-format on

/**
 * All per host stats defined. @see stats_macros.h
 */
struct HostStats {
  ALL_HOST_STATS(GENERATE_COUNTER_STRUCT, GENERATE_GAUGE_STRUCT)
};

class Cluster;

/**
 * A description of an upstream host.
 */
class HostDescription {
public:
  virtual ~HostDescription() {}

  /**
   * @return whether the host is a canary.
   */
  virtual bool canary() const PURE;

  /**
   * @return the cluster the host is a member of.
   */
  virtual const Cluster& cluster() const PURE;

  /**
   * @return the host's outlier detection sink.
   */
  virtual OutlierDetectorHostSink& outlierDetector() const PURE;

  /**
   * @return the URL used to connect to the host.
   */
  virtual const std::string& url() const PURE;

  /**
   * @return host specific stats.
   */
  virtual const HostStats& stats() const PURE;

  /**
   * @return the "zone" of the host (deployment specific). Empty is unknown.
   */
  virtual const std::string& zone() const PURE;
};

typedef std::shared_ptr<const HostDescription> HostDescriptionPtr;

} // Upstream
