// Sample.cpp

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

#include <iostream>

#include <test/TestCase.hpp>
#include <test/TestSuite.hpp>
#include <test/DefaultResult.hpp>


class MyTest1: public tst::TestCase<MyTest1>
{
public:
  /** Create a new test case and register the test functions. */
  MyTest1()
    : tst::TestCase<MyTest1>(*this, "MyTest1")
  {
    Add(&MyTest1::TestMe1);
    Add(&MyTest1::TestMe2);
  }

  /** Illustrate the usage of the @ref ASSERTM functionality. */
  void TestMe1(tst::TestResult& result)
  {
    ASSERTM(false, "has to fail!");
  }

  /** Illustrate the usage of the @ref ASSERT functionality. */
  void TestMe2(tst::TestResult& result)
  {
    /* This invocation must not fail. */
    ASSERT(true);
  }
};

class MyTest2: public tst::TestCase<MyTest2>
{
public:
  /** Create a new test case and register the test functions. */
  MyTest2()
    : tst::TestCase<MyTest2>(*this, "MyTest2")
  {
    Add(&MyTest2::TestMe1);
    Add(&MyTest2::TestMe2);
  }

  void TestMe1(tst::TestResult& result)
  {
    ASSERTM(true, "must not fail!");
  }

  void TestMe2(tst::TestResult& result)
  {
    ASSERTM(true, "must not fail!");
  }
};

int main()
{
  tst::DefaultResult<std::ostream> result(std::cout, true);
  tst::TestSuite                   suite;

  MyTest1 my_test1;
  MyTest2 my_test2;

  suite.Add(my_test1);
  suite.Add(my_test2);

  std::cout << "Running Tests...\n";

  suite.Run(result);

  std::cout << "-----------------------------\n";
  std::cout << "Summary:\n";

  result.PrintSummary();
  return 0;
}
