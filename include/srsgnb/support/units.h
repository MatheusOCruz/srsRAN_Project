/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#pragma once

#include "srsgnb/adt/strong_type.h"
#include "fmt/format.h"
#include <climits>

namespace srsgnb {

namespace units {

namespace detail {
/// Tag struct used to uniquely identify the bit units type.
struct bit_tag {
  /// Text representation for the units.
  static const char* str() { return "bits"; }
};

/// Tag struct used to uniquely identify the byte units type.
struct byte_tag {
  /// Text representation for the units.
  static const char* str() { return "bytes"; }
};
} // namespace detail

class bytes;

/// \brief Abstraction of bit as a unit of digital information.
///
/// An object of this type will represent an amount of digital information expressed in bits.
class bits : public strong_type<unsigned, detail::bit_tag, strong_arithmetic, strong_increment_decrement>
{
  /// Type alias for the base class of the bits units class.
  using bits_base = strong_type<unsigned, detail::bit_tag, strong_arithmetic, strong_increment_decrement>;

public:
  using bits_base::bits_base;

  constexpr bits(const bits_base& other) : bits_base(other) {}

  /// Returns true if the amount of digital information expressed as bits is a multiple of a byte.
  constexpr bool is_byte_exact() const { return ((value() % CHAR_BIT) == 0); }

  /// Returns the amount of digital information expressed as an integer number of bytes, rounded down.
  constexpr bytes truncate_to_bytes() const;

  /// Returns the amount of digital information expressed as an integer number of bytes, rounded up.
  constexpr bytes round_up_to_bytes() const;
};

/// \brief Abstraction of byte as a unit of digital information.
///
/// An object of this class will represent an amount of digital information expressed in bytes. The class also provides
/// a method to convert such amount into a number of information bits.
class bytes : public strong_type<unsigned, detail::byte_tag, strong_arithmetic, strong_increment_decrement>
{
  /// Type alias for the base class of the byte units class.
  using bytes_base = strong_type<unsigned, detail::byte_tag, strong_arithmetic, strong_increment_decrement>;

public:
  using bytes_base::bytes_base;

  explicit constexpr operator bits() const { return to_bits(); }

  /// Returns the amount of digital information as a number of bits.
  constexpr bits to_bits() const { return bits(value() * CHAR_BIT); }
};

constexpr bytes bits::truncate_to_bytes() const
{
  return bytes(value() / CHAR_BIT);
}

constexpr bytes bits::round_up_to_bytes() const
{
  return bytes((value() + CHAR_BIT - 1) / CHAR_BIT);
}

namespace literals {

/// User defined literal for byte units.
constexpr bytes operator"" _bytes(unsigned long long n)
{
  return bytes(n);
}

/// User defined literal for bit units.
constexpr bits operator"" _bits(unsigned long long n)
{
  return bits(n);
}

} // namespace literals
} // namespace units
} // namespace srsgnb

namespace fmt {

/// Formatter for bit units.
template <>
struct formatter<srsgnb::units::bits> : public formatter<srsgnb::units::bits::value_type> {
  template <typename FormatContext>
  auto format(srsgnb::units::bits s, FormatContext& ctx) -> decltype(std::declval<FormatContext>().out())
  {
    return fmt::format_to(ctx.out(), "{}{}", s.value(), srsgnb::units::bits::tag_type::str());
  }
};

/// Formatter for byte units.
template <>
struct formatter<srsgnb::units::bytes> : public formatter<srsgnb::units::bytes::value_type> {
  template <typename FormatContext>
  auto format(srsgnb::units::bytes s, FormatContext& ctx) -> decltype(std::declval<FormatContext>().out())
  {
    return fmt::format_to(ctx.out(), "{}{}", s.value(), srsgnb::units::bytes::tag_type::str());
  }
};

} // namespace fmt