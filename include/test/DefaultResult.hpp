// DefaultResult.hpp

/***************************************************************************
 *   Copyright (C) 2009-2011 Daniel Mueller (deso@posteo.net)              *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef TSTDEFAULTRESULT_HPP
#define TSTDEFAULTRESULT_HPP

#include "TestResult.hpp"


namespace tst
{
  /**
   * This class represents a reasonable default implementation of a
   * TestResult. Being minimalistic it merely prints the results to a
   * stream.
   */
  template<typename T>
  class DefaultResult: public TestResult
  {
  public:
    DefaultResult(T& printer, bool verbose = false);

    virtual void StartTest(char const* test);
    virtual void EndTest();

    virtual void StartTestFunction();
    virtual void EndTestFunction();

    virtual void Assert(bool assertion, char const* file, int line, char const* message);

    void PrintSummary();

    int GetTestsRun() const;
    int GetTestsFailed() const;

    int GetFunctionsRun() const;
    int GetFunctionsFailed() const;

    int GetAssertionsChecked() const;
    int GetAssertionsFailed() const;

  private:
    T* printer_;
    bool verbose_;

    int test_id_;
    int tests_run_;
    int tests_failed_;

    int function_id_;
    int functions_run_;
    int functions_failed_;
    int functions_run_this_test_;
    int functions_failed_this_test_;

    int assertions_checked_;
    int assertions_failed_;

    char const* current_test_;
    int last_failed_test_;
    int last_failed_function_;

    void PrintTestResult() const;
    void PrintError(char const* file, int line, char const* message) const;
  };
}

namespace tst
{
  /**
   * The default constructor creates an empty DefaultResult object.
   */
  template<typename T>
  inline DefaultResult<T>::DefaultResult(T& printer, bool verbose)
    : printer_(&printer),
      verbose_(verbose),
      test_id_(0),
      tests_run_(0),
      tests_failed_(0),
      function_id_(0),
      functions_run_(0),
      functions_failed_(0),
      functions_run_this_test_(0),
      functions_failed_this_test_(0),
      assertions_checked_(0),
      assertions_failed_(0),
      current_test_(0),
      last_failed_test_(0),
      last_failed_function_(0)
  {
  }

  /**
   * @copydoc TestResult::StartTest
   */
  template<typename T>
  void DefaultResult<T>::StartTest(char const* test)
  {
    test_id_++;
    tests_run_++;

    functions_run_this_test_ = 0;
    functions_failed_this_test_ = 0;

    current_test_ = test;
  }

  /**
   * @copydoc TestResult::EndTest
   */
  template<typename T>
  void DefaultResult<T>::EndTest()
  {
    if (verbose_)
      PrintTestResult();

    current_test_ = 0;
  }

  /**
   * @copydoc TestResult::StartTestFunction
   */
  template<typename T>
  void DefaultResult<T>::StartTestFunction()
  {
    function_id_++;
  }

  /**
   * @copydoc TestResult::EndTestFunction
   */
  template<typename T>
  void DefaultResult<T>::EndTestFunction()
  {
    functions_run_this_test_++;
    functions_run_++;
  }

  /**
   * @copydoc TestResult::Assert
   */
  template<typename T>
  void DefaultResult<T>::Assert(bool assertion, char const* file, int line, char const* message)
  {
    assertions_checked_++;

    if (!assertion)
    {
      assertions_failed_++;

      if (last_failed_test_ != test_id_)
      {
        tests_failed_++;
        last_failed_test_ = test_id_;
      }

      if (last_failed_function_ != function_id_)
      {
        functions_failed_++;
        functions_failed_this_test_++;

        last_failed_function_ = function_id_;
      }

      PrintError(file, line, message);
    }
  }

  template<typename T>
  void DefaultResult<T>::PrintSummary()
  {
    (*printer_) << "Tests run:          " << GetTestsRun()          << '\n';
    (*printer_) << "Tests failed:       " << GetTestsFailed()       << '\n';
    (*printer_) << "Functions run:      " << GetFunctionsRun()      << '\n';
    (*printer_) << "Functions failed:   " << GetFunctionsFailed()   << '\n';
    (*printer_) << "Assertions checked: " << GetAssertionsChecked() << '\n';
    (*printer_) << "Assertions failed:  " << GetAssertionsFailed()  << '\n';
  }

  /**
   * @return number of tests that were run
   */
  template<typename T>
  inline int DefaultResult<T>::GetTestsRun() const
  {
    return tests_run_;
  }

  /**
   * @return number of tests that were run and that failed
   * @note a test can only fail once, meaning although an assertion may
   *       fail, the test will continue (and other assertions might fail
   *       as well) but it will be recorded as one failed test
   */
  template<typename T>
  inline int DefaultResult<T>::GetTestsFailed() const
  {
    return tests_failed_;
  }

  /**
   * @return number of test functions that were run
   */
  template<typename T>
  inline int DefaultResult<T>::GetFunctionsRun() const
  {
    return functions_run_;
  }

  /**
   * @return number of test functions that were run and that failed
   */
  template<typename T>
  inline int DefaultResult<T>::GetFunctionsFailed() const
  {
    return functions_failed_;
  }

  /**
   * @return number of assertions that were checked
   */
  template<typename T>
  inline int DefaultResult<T>::GetAssertionsChecked() const
  {
    return assertions_checked_;
  }

  /**
   * @return number of assertions that were checked and that failed
   */
  template<typename T>
  inline int DefaultResult<T>::GetAssertionsFailed() const
  {
    return assertions_failed_;
  }

  /**
   * This small helper method prints the result for the current test.
   */
  template<typename T>
  void DefaultResult<T>::PrintTestResult() const
  {
    // if the test has a name we print that, otherwise we use the id
    if (current_test_ != 0)
      (*printer_) << current_test_;
    else
      (*printer_) << test_id_;

    int successful = functions_run_this_test_ - functions_failed_this_test_;
    int percentage = functions_run_ > 0 ? successful * 100 / functions_run_this_test_ : 100;

    (*printer_) << ":\n\t";
    (*printer_) << successful << '/' << functions_run_this_test_ << " (" << percentage << "%)"
                << ":\t" << (last_failed_test_ != test_id_ ? "Successful" : "Failed") << '\n';
  }

  /**
   * @param file file the error occurred in
   * @param line line the error occurred in
   * @param message optional message to print (may be null)
   */
  template<typename T>
  void DefaultResult<T>::PrintError(char const* file, int line, char const* message) const
  {
    (*printer_) << "\tError: " << file << " (" << line << ")";

    if (current_test_ != 0)
      (*printer_) << ": " << current_test_;

    if (message != 0)
      (*printer_) << ": " << message;

    (*printer_) << '\n';
  }
}


#endif
