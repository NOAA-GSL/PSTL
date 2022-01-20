/**
 * \file       stop_watch.hpp
 * \author     Bryan Flynt
 * \date       Jan 18, 2022
 */
#pragma once

#include <chrono>
#include <cstdint>

namespace xstd {
namespace detail {

/** Detect the highest resolution non-sleeping clock
 *
 * If the std high_resolution_clock is steady then choose it otherwise
 * select the std steady_clock which by definition is steady but may
 * not be as high resolution.
 */
using maxres_nonsleep_clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
                                                 std::chrono::high_resolution_clock, std::chrono::steady_clock>;

} /* namespace detail */

/// StopWatch class for calculating the time of things
/**
 * StopWatch class for timing high resolution wall clock times.
 *
 * @note
 * xstd::chrono::Clock can be used to time all (real, user, system)
 * times but at a lower resolution.
 */
class StopWatch final {
   public:
    using value_type = double;

   public:
    StopWatch(const StopWatch& other) = default;
    StopWatch(StopWatch&& other)      = default;
    ~StopWatch()                      = default;
    StopWatch& operator=(const StopWatch& other) = default;
    StopWatch& operator=(StopWatch&& other) = default;

    StopWatch() : duration_(clock_duration::zero()), is_running_(false) {}

    /** Return status of StopWatch
     */
    bool is_running() const noexcept { return is_running_; }

    /** Start the StopWatch from Current Elapsed
     */
    void start() noexcept {
        if (not this->is_running()) {
            is_running_ = true;
            start_      = clock::now();
        }
    }

    /** Stop the StopWatch
     */
    void stop() noexcept {
        if (this->is_running()) {
            duration_ += (clock::now() - start_);
            is_running_ = false;
        }
    }

    /** Reset the StopWatch to Zero Elapsed Time
     */
    void reset() noexcept {
        this->stop();
        duration_ = clock_duration::zero();
    }

    /** Restart the StopWatch from Zero Elapsed Time
     */
    void restart() noexcept {
        this->stop();
        this->reset();
        this->start();
    }

    /** Returns current timings but does not stop timer
     */
    value_type elapsed_seconds() noexcept {
        if (this->is_running()) {
            this->touch_();  // Start+Stop with only 1 time call
        }
        return std::chrono::duration_cast<output_duration>(duration_).count();
    }

   private:
    using clock           = detail::maxres_nonsleep_clock;
    using time_point      = typename clock::time_point;
    using clock_duration  = typename clock::duration;
    using output_duration = std::chrono::duration<value_type>;  // Seconds

    time_point start_;
    clock_duration duration_ = clock_duration::zero();
    bool is_running_         = false;

    // Start+Stop with only 1 time call (vs. 2)
    void touch_() noexcept {
        auto tm = clock::now();      // Only 1 time call
        duration_ += (tm - start_);  // Stop Action
        start_ = tm;                 // Start Action
    }
};

} /* namespace xstd */
