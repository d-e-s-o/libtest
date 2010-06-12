// TestResult.hpp

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

#ifndef TSTTESTRESULT_HPP
#define TSTTESTRESULT_HPP


namespace tst
{
  /**
   * Objects of derived classes are used for evaluating tests.
   */
  class TestResult
  {
  public:
    /**
     * This method marks the beginning of a new test case to run. The
     * method is invoked automatically by the framework.
     * @param test name of the test case that is about to be run
     */
    virtual void StartTest(char const* test) = 0;

    /**
     * This method marks the end of a test case previously started using
     * 'startTest'. The method is invoked automatically by the
     * framework.
     */
    virtual void EndTest() = 0;

    /**
     * Assert that a condition holds.
     * @param assertion condition to check
     * @param file file in which the assertion failure occurred
     * @param line line (in the given 'file') where the assertion
     *        failure occurred
     * @param message an optional message conveying additional
     *        information to the user
     */
    virtual void Assert(bool assertion, char const* file, int line, char const* message = 0) = 0;
  };
}


#endif