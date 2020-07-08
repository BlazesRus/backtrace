#ifndef VERIFYEXECUTIONTIME_H
#define VERIFYEXECUTIONTIME_H

//#include "timer.h"
#include <boost/timer/timer.hpp>

#include <memory>
#include <functional>
#include <string>

/**
 * @brief The VerifyExecutionTime class should warn if it takes longer than
 * specified to execute a scope.
 *
 * It should print a backtrace by default if no report function is given.
 *
 * It should not warn about execution time if unwinding from an exception.
 *
 * It should cause an overhead of less than 1.5 microseconds in a release
 * build and less than 3 microseconds in a debug build.
 *
 * Never throw from the report function. Doing so results in undefined
 * behavior.
 */
class VerifyExecutionTime
{
public:
    using ptr = std::shared_ptr<VerifyExecutionTime>;
    using report = std::function<void(boost::timer::cpu_times expected_time, boost::timer::cpu_times execution_time )>;

    static ptr start(boost::timer::cpu_times expected_time, report report_func=nullptr );

    static void default_report(boost::timer::cpu_times expected_time, boost::timer::cpu_times execution_time, const std::string& label);
    static void set_default_report(report func);

    ~VerifyExecutionTime();

private:
    VerifyExecutionTime( boost::timer::cpu_times expected_time_, report func=0 );

    //Timer timer_;
    boost::timer::auto_cpu_timer executionTimer;
    //double expected_time_;
    boost::timer::cpu_times expectedTime;
    report report_func_;
    static report default_report_func_;

//public:
//    static void test();
};

#endif // VERIFYEXECUTIONTIME_H
