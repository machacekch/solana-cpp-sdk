#pragma once
/**
 * Helpers which are not part of the public interface of the SDK
 *
 * For the most part, these are re-implemented template programming tools from the C++ standard library.
 * Ideally, the standard headers should be added to Solana's clang++ distribution instead.
 */

namespace sol::internal {

// std::enable_if from <type_traits>
template<bool B, class T = void> struct enable_if {};
template<class T> struct enable_if<true, T> { typedef T type; };

} // namespace sol::internal
