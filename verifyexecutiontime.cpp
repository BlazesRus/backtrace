#include "verifyexecutiontime.h"
//#include "tasktimer.h"
#include <boost/timer/timer.hpp>
#include "backtrace.h"

#include <iomanip>
#include <sstream>
#include <boost/format.hpp>

using namespace std;

VerifyExecutionTime::report VerifyExecutionTime::default_report_func_;

//inline void VerifyExecutionTime::default_report(double expected_time, double execution_time, const std::string& label)
//{
    //string expected = TaskTimer::timeToString(expected_time);
    //string elapsed = TaskTimer::timeToString(execution_time);

    //TaskInfo(boost::format("!!! VerifyExecutionTime: Took %s > %s %s")
    //    % elapsed % expected % label);
//}
inline void VerifyExecutionTime::default_report(boost::timer::cpu_times expected_time, boost::timer::cpu_times execution_time, const std::string& label)
{
    (boost::format("!!! VerifyExecutionTime: Took  {%s} > %s %s") % boost::timer::format(expected_time) % boost::timer::format(execution_time) % label);
}

void VerifyExecutionTime::set_default_report(report report_func)
{
    default_report_func_ = report_func;
}

//VerifyExecutionTime::ptr VerifyExecutionTime::
//        start( double expected_time, report report_func )
//{
//    if (!report_func)
//    {
//        report_func = default_report_func_;
//    }
//
//    if (!report_func)
//    {
//        // It should print a BackTrace by default if no report function is given.
//        report_func = [](double expected_time, double execution_time){ default_report(expected_time, execution_time, Backtrace::make_string ()); };
//    }
//
//    return ptr(new VerifyExecutionTime( expected_time, report_func ));
//}
VerifyExecutionTime::ptr VerifyExecutionTime::start(boost::timer::cpu_times expected_time, report report_func)
{
    if (report_func== nullptr)
    {
        report_func = default_report_func_;
    }
    if (report_func== nullptr)
    {
        // It should print a BackTrace by default if no report function is given.
        report_func = [](boost::timer::cpu_times expected_time, boost::timer::cpu_times execution_time) { default_report(expected_time, execution_time, Backtrace::make_string()); };
    }

    return ptr(new VerifyExecutionTime(expected_time, report_func));
}

//VerifyExecutionTime::VerifyExecutionTime(double expected_time, report report_func):
//expected_time_(expected_time), report_func_(report_func)
//{
//}
VerifyExecutionTime::VerifyExecutionTime(boost::timer::cpu_times expected_time, report report_func) :
    expectedTime(expected_time), report_func_(report_func)
{
}

VerifyExecutionTime::~VerifyExecutionTime()
{
    if (std::uncaught_exception())
        return;

    auto execution_time = executionTimer.elapsed();
    //double execution_time = timer_.elapsed ();

    if (expectedTime < execution_time)
    {
        report_func_(expectedTime, execution_time);
    }
}

////////////////////////////////////
//// VerifyExecutionTime::test
//
//#include "exceptionassert.h"
////#include <boost/thread.hpp>
//#include <thread>
//
//void VerifyExecutionTime::test()
//{
//    // It should warn if it takes longer than specified to execute a scope.
//    {
//        double expected_time=0.004, execution_time=0;
//
//        {
//            VerifyExecutionTime::ptr x = VerifyExecutionTime::start (expected_time, [&](double, double v){
//                execution_time = v;
//            });
//            this_thread::sleep_for (chrono::milliseconds(1));
//        }
//
//        EXCEPTION_ASSERT_LESS(execution_time, expected_time);
//
//        bool did_report = false;
//        {
//            VerifyExecutionTime::ptr x = VerifyExecutionTime::start (0.001, [&did_report](double,double){did_report = true;});
//            this_thread::sleep_for (chrono::milliseconds(1));
//
//            EXCEPTION_ASSERT(!did_report);
//        }
//        EXCEPTION_ASSERT(did_report);
//    }
//
//    // It should print a backtrace by default if no report function is given.
//    {
//        // See VerifyExecutionTime::start
//    }
//
//    // It should not warn about execution time if unwinding from an exception.
//    {
//        bool did_report = false;
//
//        try {
//            VerifyExecutionTime::ptr x = VerifyExecutionTime::start (0.001, [&did_report](double,double){did_report = true;});
//            this_thread::sleep_for (chrono::milliseconds(2));
//            throw 0;
//        } catch (int) {}
//
//        EXCEPTION_ASSERT(!did_report);
//    }
//
//    // It should cause an overhead of less than 1.5 microseconds in a release
//    // build and less than 3 microseconds in a debug build.
//    {
//        bool debug = false;
//#ifdef _DEBUG
//        debug = true;
//#endif
//
//        int N = 100000;
//        Timer t;
//        for (int i=0; i<N; ++i) {
//            VerifyExecutionTime(0.1);
//        }
//        double T = t.elapsedAndRestart () / N;
//
//        EXCEPTION_ASSERT_LESS( T, debug ? 2e-6 : 0.8e-6 );
//
//        for (int i=0; i<N; ++i) {
//            VerifyExecutionTime::ptr a = VerifyExecutionTime::start (0.1);
//        }
//        T = t.elapsed () / N;
//
//        EXCEPTION_ASSERT_LESS( T, debug ? 3e-6 : 1.5e-6 );
//    }
//}