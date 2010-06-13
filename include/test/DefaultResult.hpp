// DefaultResult.hpp

/***************************************************************************
 *   Copyright (C) 2009-2010 Daniel Mueller (deso@posteo.net)              *
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

#include <iostream>

#include "TestResult.hpp"


namespace tst
{
  /**
   * This class represents a reasonable default implementation of a
   * TestResult. Being minimalistic it merely prints the results to a
   * stream.
   */
  class DefaultResult: public TestResult
  {
  public:
    DefaultResult();

    virtual void StartTest(char const* test);
    virtual void EndTest();

    virtual void Assert(bool assertion, char const* file, int line, char const* message);

    int GetTestsRun() const;
    int GetTestsFailed() const;

    int GetAssertionsChecked() const;
    int GetAssertionsFailed() const;

  private:
    int test_id_;

    int tests_run_;
    int tests_failed_;

    int assertions_checked_;
    int assertions_failed_;

    char const* current_test_;
    int last_failed_;

    void PrintTestMessage(char const* message) const;
  };
}

namespace tst
{
  /**
   * The default constructor creates an empty DefaultResult object.
   */
  inline DefaultResult::DefaultResult()
    : test_id_(0),
      tests_run_(0),
      tests_failed_(0),
      assertions_checked_(0),
      assertions_failed_(0),
      current_test_(0)
  {
  }

  /**
   * @copydoc TestResult::StartTest
   */
  inline void DefaultResult::StartTest(char const* test)
  {
    test_id_++;
    tests_run_++;
    current_test_ = test;

    PrintTestMessage("StartTest");
  }

  /**
   * @copydoc TestResult::EndTest
   */
  inline void DefaultResult::EndTest()
  {
    PrintTestMessage("EndTest");

    std::cout << '\n';

    current_test_ = 0;
  }

  /**
   * @copydoc TestResult::Assert
   */
  inline void DefaultResult::Assert(bool assertion, char const* file, int line, char const* message)
  {
    assertions_checked_++;

    if (!assertion)
    {
      assertions_failed_++;

      if (last_failed_ != test_id_)
        tests_failed_++;

      last_failed_ = test_id_;

      std::cout << "Error: " << file << " (" << line << ")";

      if (message != 0)
        std::cout << ": " << message;

      std::cout << '\n';
    }
  }

  /**
   * @return number of tests that were run
   */
  inline int DefaultResult::GetTestsRun() const
  {
    return tests_run_;
  }

  /**
   * @return number of tests that were run and that failed
   * @note a test can only fail once, meaning although an assertion may
   *       fail, the test will continue (and other assertions might fail
   *       as well) but it will be recorded as one failed test
   */
  inline int DefaultResult::GetTestsFailed() const
  {
    return tests_failed_;
  }

  /**
   * @return number of assertions that were checked
   */
  inline int DefaultResult::GetAssertionsChecked() const
  {
    return assertions_checked_;
  }

  /**
   * @return number of assertions that were checked and that failed
   */
  inline int DefaultResult::GetAssertionsFailed() const
  {
    return assertions_failed_;
  }

  /**
   * This small helper method prints out a message and optionally the
   * content of the current_test_ variable (if not null).
   */
  inline void DefaultResult::PrintTestMessage(char const* message) const
  {
    std::cout << message;

    if (current_test_ != 0)
      std::cout << ":\t" << current_test_;

    std::cout << '\n';
  }
}


#endif
