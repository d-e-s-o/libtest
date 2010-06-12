// TestCase.hpp

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

#ifndef TSTTESTCASE_HPP
#define TSTTESTCASE_HPP

#include "TestBase.hpp"
#include "TestResult.hpp"
#include "TestContainer.hpp"


namespace tst
{
  /**
   * This class is the base class for all classes that are to be tested.
   */
  template<typename T>
  class TestCase: public TestBase
  {
  public:
    typedef void (T::*Test)(TestResult&);

    TestCase(T& instance, char const* name = 0);

    virtual void Run(TestResult& result);
    virtual bool Add(Test const& test);

  protected:
    virtual void SetUp();
    virtual void TearDown();

  private:
    /* We have a fixed upper limit of tests that we support. */
    typedef TestContainer<Test, 256> Tests;

    T* instance_;
    char const* name_;
    Tests tests_;
  };


  /**
   * Test that an assertion holds, include the given message in the
   * reported error if not.
   * @param assertion_ boolean value to check for trueness
   * @param message_ message to include in error report
   */
  #define ASSERTM(assertion_, message_)\
    do\
    {\
      result.Assert((assertion_), __FILE__, __LINE__, message_);\
    } while (false)

  /**
   * Test that an assertion holds.
   * @param assertion_ boolean value to check for trueness
   */
  #define ASSERT(assertion_)\
    ASSERTM(assertion_, 0)
}

namespace tst
{
  /**
   * The default constructor creates an empty TestCase.
   */
  template<typename T>
  inline TestCase<T>::TestCase(T& instance, char const* name)
    : instance_(&instance),
      name_(name),
      tests_()
  {
  }

  /**
   * @copydoc TestBase::Run
   */
  template<typename T>
  inline void TestCase<T>::Run(TestResult& result)
  {
    result.StartTest(name_);

    for (typename Tests::Iterator it = tests_.Begin(); it != tests_.End(); ++it)
    {
      SetUp();
      (instance_->*(*it))(result);
      TearDown();
    }

    result.EndTest();
  }

  /**
   * This method can be used to add a new test function to the list of
   * tests to execute.
   * @param test a test case or test suite to add
   * @return true if adding the test was successful, false if not
   */
  template<typename T>
  inline bool TestCase<T>::Add(Test const& test)
  {
    if (test != 0)
      return tests_.Add(test);

    return false;
  }

  /**
   * This method can be overwritten to do some initialization work
   * before each test.
   */
  template<typename T>
  inline void TestCase<T>::SetUp()
  {
  }

  /**
   * This method can be overwritten to undo the initialization work
   * done in setUp. It is called right after the test finished.
   */
  template<typename T>
  inline void TestCase<T>::TearDown()
  {
  }
}


#endif
