#pragma once

#include <torch/nn/options/padding.h>

namespace torch {
namespace nn {
namespace functional {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace detail {
inline Tensor pad(const Tensor& input,
                  IntArrayRef pad,
                  PadFuncOptions::mode_t mode,
                  double value) {
  TORCH_CHECK(pad.size() % 2 == 0, "Padding length must be divisible by 2");
  TORCH_CHECK(((int64_t)(pad.size() / 2)) <= input.dim(), "Padding length too large");
  if (c10::get_if<enumtype::kConstant>(&mode)) {
    return torch::constant_pad_nd(input, pad, value);
  } else {
    TORCH_CHECK(
      value == 0,
      "Padding mode \"",
      torch::enumtype::get_enum_name(mode),
      "\" doesn't take in value argument");
    if (pad.size() == 2 && (input.dim() == 2 || input.dim() == 3)) {
      if (c10::get_if<enumtype::kReflect>(&mode)) {
        return torch::reflection_pad1d(input, pad);
      } else if (c10::get_if<enumtype::kReplicate>(&mode)) {
        return torch::replication_pad1d(input, pad);
      } else if (c10::get_if<enumtype::kCircular>(&mode)) {
        return torch::_pad_circular(input, pad);
      } else {
        TORCH_CHECK(false, "NotImplementedError");
      }
    } else if(pad.size() == 4 && (input.dim() == 3 || input.dim() == 4)) {
      if (c10::get_if<enumtype::kReflect>(&mode)) {
        return torch::reflection_pad2d(input, pad);
      } else if (c10::get_if<enumtype::kReplicate>(&mode)) {
        return torch::replication_pad2d(input, pad);
      } else if (c10::get_if<enumtype::kCircular>(&mode)) {
        return torch::_pad_circular(input, pad);
      } else {
        TORCH_CHECK(false, "NotImplementedError");
      }
    } else if (pad.size() == 6 && (input.dim() == 4 || input.dim() == 5)) {
      if (c10::get_if<enumtype::kReflect>(&mode)) {
        return torch::reflection_pad3d(input, pad);
      } else if (c10::get_if<enumtype::kReplicate>(&mode)) {
        return torch::replication_pad3d(input, pad);
      } else if (c10::get_if<enumtype::kCircular>(&mode)) {
        return torch::_pad_circular(input, pad);
      } else {
        TORCH_CHECK(false, "NotImplementedError");
      }
    } else {
      TORCH_CHECK(false, "Only 2D, 3D, 4D, 5D padding with non-constant padding are supported for now");
    }
  }
}
} // namespace detail
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/// See https://pytorch.org/docs/master/nn.functional.html#torch.nn.functional.pad
/// about the exact behavior of this functional.
///
/// See the documentation for `torch::nn::functional::PadFuncOptions` class to learn what
/// optional arguments are supported for this functional.
///
/// Example:
/// ```
/// namespace F = torch::nn::functional;
/// F::pad(input, F::PadFuncOptions({1, 2, 2, 1, 1, 2}).mode(torch::kReplicate));
/// ```
inline Tensor pad(const Tensor& input, const PadFuncOptions& options) {
  return detail::pad(input, options.pad(), options.mode(), options.value());
}

} // namespace functional
} // namespace nn
} // namespace torch
