// Copyright (c) 2018 ensmallen developers.
//
// Licensed under the 3-clause BSD license (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.opensource.org/licenses/BSD-3-Clause

#include <ensmallen.hpp>
#include "catch.hpp"
#include "test_function_tools.hpp"

using namespace ens;
using namespace ens::test;

/**
 * Run SPALeRA SGD on logistic regression and make sure the results are
 * acceptable.
 */
TEST_CASE("LogisticRegressionTest","[SPALeRASGDTest]")
{
  arma::mat data, testData, shuffledData;
  arma::Row<size_t> responses, testResponses, shuffledResponses;

  LogisticRegressionTestData(data, testData, shuffledData,
      responses, testResponses, shuffledResponses);

  // Now run mini-batch SGD with a couple of batch sizes.
  for (size_t batchSize = 30; batchSize < 50; batchSize += 5)
  {
    SPALeRASGD<> optimizer(0.05 / batchSize, batchSize, 10000, 1e-4);
    LogisticRegression<> lr(shuffledData, shuffledResponses, 0.5);

    arma::mat coordinates = lr.GetInitialPoint();
    optimizer.Optimize(lr, coordinates);

    // Ensure that the error is close to zero.
    const double acc = lr.ComputeAccuracy(data, responses, coordinates);
    REQUIRE(acc == Approx(100.0).epsilon(0.015)); // 1.5% error tolerance.

    const double testAcc = lr.ComputeAccuracy(testData, testResponses,
        coordinates);
    REQUIRE(testAcc == Approx(100.0).epsilon(0.024)); // 2.4% error tolerance.
  }
}