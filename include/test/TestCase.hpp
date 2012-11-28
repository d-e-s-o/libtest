// TestCase.hpp

/***************************************************************************
 *   Copyright (C) 2009-2010,2012 Daniel Mueller (deso@posteo.net)         *
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

    TestCase(T& instance, char const* name = nullptr);

    TestCase(TestCase&&) = delete;
    TestCase(TestCase const&) = delete;

    TestCase& operator =(TestCase&&) = delete;
    TestCase& operator =(TestCase const&) = delete;

    virtual void run(TestResult& result);
    virtual bool add(Test const& test);

  protected:
    virtual void setUp();
    virtual void tearDown();

  private:
    /* We have a fixed upper limit of tests that we support. */
    typedef TestContainer<Test, 256> Tests;

    T* instance_;
    char const* name_;
    Tests tests_;
  };


  /**
   * This little helper function creates and returns a statically
   * allocated instance of the given test case. This allows some more
   * convinient creating and adding to suites.
   */
  template<typename T>
  T& createTestCase();


  /**
   * Test that an assertion holds, include the given message in the
   * reported error if not.
   * @param assertion_ boolean value to check for trueness
   * @param message_ message to include in error report
   */
  #define ASSERTM(assertion_, message_)\
    do\
    {\
      result.assert((assertion_), __FILE__, __LINE__, message_);\
    } while (false)

  /**
   * Test that an assertion holds.
   * @param assertion_ boolean value to check for trueness
   */
  #define ASSERT(assertion_)\
    ASSERTM(assertion_, nullptr)
}

namespace tst
{
  /**
   * @return a newly created test case
   */
  template<typename T>
  inline T& createTestCase()
  {
    static T t;
    return t;
  }

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
   * @copydoc TestBase::run
   */
  template<typename T>
  inline void TestCase<T>::run(TestResult& result)
  {
    result.startTest(name_);

    for (auto it = tests_.begin(); it != tests_.end(); ++it)
    {
      result.startTestFunction();

      setUp();
      (instance_->*(*it))(result);
      tearDown();

      result.endTestFunction();
    }

    result.endTest();
  }

  /**
   * This method can be used to add a new test function to the list of
   * tests to execute.
   * @param test a test case or test suite to add
   * @return true if adding the test was successful, false if not
   */
  template<typename T>
  inline bool TestCase<T>::add(Test const& test)
  {
    if (test != nullptr)
      return tests_.add(test);

    return false;
  }

  /**
   * This method can be overwritten to do some initialization work
   * before each test.
   */
  template<typename T>
  inline void TestCase<T>::setUp()
  {
  }

  /**
   * This method can be overwritten to undo the initialization work
   * done in setUp. It is called right after the test finished.
   */
  template<typename T>
  inline void TestCase<T>::tearDown()
  {
  }
}


#endif
