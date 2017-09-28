/**
 * Copyright (c) 2016-present, Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "caffe2/operators/piecewise_linear_transform_op.h"

namespace caffe2 {

REGISTER_CPU_OPERATOR(
    PiecewiseLinearTransform,
    PiecewiseLinearTransformOp<float, CPUContext>);

OPERATOR_SCHEMA(PiecewiseLinearTransform)
    .NumInputs(1, 4)
    .NumOutputs(1)
    .SetDoc(R"DOC(
PiecewiseLinearTransform takes inputs -- predictions, a 2-D or 1-D tensor
(Tensor<float>) of size (batch_size x prediction_dimensions). The piecewise
linear functions are stored in bounds, slopes and intercepts. The output tensor
has the same shape of input `predictions` and contains the predictions
transformed by the piecewise linear functions. Each column of predictions has
its own piecewise linear transformation functions. Therefore the size of
piecewise function parameters are pieces x prediction_dimensions, except for
binary predictions where only the positive prediction needs them. Note that in
each piece, low bound is excluded while high bound is included. Also the
piecewise linear function must be continuous.

Notes
- If the input is binary predictions (Nx2 or Nx1 tensor), set the binary arg
to true so that one group of piecewise linear functions is needed (see
details below).
- The transform parameters (bounds, slopes, intercepts) can be passed either
through args or through input blobs.
- If we have multiple groups of piecewise linear functions, each group has the
same number of pieces.
- If a prediction is out of the bounds, it is capped to the smallest or largest
bound.
)DOC")
    .Arg(
        "bounds",
        "1-D vector of size (prediction_dimensions x (pieces+1)) contain the "
        "upper bounds of each piece of linear function. One special case is "
        "the first bound is the lower bound of whole piecewise function and we "
        "treat it the same as the left most functions. (bounds, slopes, "
        "intercepts) can be passed through either arg or input blobs.")
    .Arg(
        "slopes",
        "1-D vector of size (prediction_dimensions x pieces) containing the "
        "slopes of linear function")
    .Arg(
        "intercepts",
        "1-D vector of size (prediction_dimensions x pieces) containing the "
        "intercepts of linear function")
    .Arg(
        "binary",
        "If set true, we assume the input is a Nx1 or Nx2 tensor. If it is Nx1 "
        "tensor, it is positive predictions. If the input is Nx2 tensor, its "
        "first column is negative predictions and second column is positive "
        "and negative + positive = 1. We just need one group of piecewise "
        "linear functions for the positive predictions.")
    .Input(
        0,
        "predictions",
        "2-D tensor (Tensor<float>) of size "
        "(num_batches x num_classes) containing scores")
    .Input(
        1,
        "bounds (optional)",
        "See bounds in Arg. (bounds, slopes, intercepts) can be passed through "
        "either arg or input blobs.")
    .Input(
        2,
        "slopes (optional)",
        "See slopes in Arg. (bounds, slopes, intercepts) can be passed through "
        "either arg or input blobs.")
    .Input(
        3,
        "intercepts (optional)",
        "See intercepts in Arg. (bounds, slopes, intercepts) can be passed "
        "through either arg or input blobs.")
    .Output(
        0,
        "transforms",
        "2-D tensor (Tensor<float>) of size (num_batches x num_classes) "
        "containing transformed predictions");

SHOULD_NOT_DO_GRADIENT(PiecewiseLinearTransform);
} // namespace caffe2
