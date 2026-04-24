/* optional_ref.hpp
 * Copyright © 2026 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v.2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <compare>
#include <functional>
#include <optional>
#include <type_traits>

namespace IOCore {

/// \brief A composited optional reference wrapper with clean reference
///        semantics.
///
/// Wraps `std::optional<std::reference_wrapper<T>>` with pointer-like
/// operators and safe/unsafe access methods. Prevents binding to temporaries
/// at compile time. Supports implicit conversion to `optional_ref<const T>`.
///
/// \tparam T The referenced type (may be const-qualified)
template<typename T>
class optional_ref
{
        using Wrapper = std::reference_wrapper<T>;
        using Storage = std::optional<Wrapper>;

        Storage storage;

    public:
        // -----------------------------------------------------------------
        // Types — STL-compatible nested aliases
        // -----------------------------------------------------------------
        using value_type = T;

        // -----------------------------------------------------------------
        // Constructors
        // -----------------------------------------------------------------
        constexpr optional_ref() noexcept = default;

        constexpr optional_ref(std::nullopt_t) noexcept
            : storage(std::nullopt)
        {
        }

        constexpr optional_ref(T& obj) noexcept
            : storage(std::ref(obj))
        {
        }

        /// \brief Deleted — prevents silent binding to temporaries
        optional_ref(T&&) = delete;

        constexpr optional_ref(const optional_ref&) noexcept = default;
        constexpr optional_ref(optional_ref&&) noexcept      = default;

        /// \brief Implicit conversion from optional_ref<T> to
        ///        optional_ref<const T>
        ///
        /// Only available when T is const-qualified and TU is the
        /// non-const counterpart.
        template<
            typename TU,
            typename = std::enable_if_t<
                std::is_same_v<TU, std::remove_const_t<T>> &&
                std::is_const_v<T>>>
        constexpr optional_ref(const optional_ref<TU>& other) noexcept
            : storage(
                  other.has_value()
                      ? Storage(std::ref(static_cast<T&>(other.get_unchecked())))
                      : Storage(std::nullopt)
              )
        {
        }

        // -----------------------------------------------------------------
        // Assignment
        // -----------------------------------------------------------------
        constexpr auto operator=(std::nullopt_t) noexcept -> optional_ref&
        {
                storage = std::nullopt;
                return *this;
        }

        constexpr auto operator=(T& obj) noexcept -> optional_ref&
        {
                storage = std::ref(obj);
                return *this;
        }

        /// \brief Deleted — prevents silent binding to temporaries
        auto operator=(T&&) -> optional_ref& = delete;

        constexpr auto operator=(const optional_ref&) noexcept
            -> optional_ref& = default;

        constexpr auto operator=(optional_ref&&) noexcept
            -> optional_ref& = default;

        // -----------------------------------------------------------------
        // Core optional interface
        // -----------------------------------------------------------------

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool
        {
                return storage.has_value();
        }

        constexpr explicit operator bool() const noexcept
        {
                return has_value();
        }

        constexpr auto reset() noexcept -> void { storage.reset(); }

        // -----------------------------------------------------------------
        // Checked access — throws std::bad_optional_access if empty
        // -----------------------------------------------------------------

        /// \brief Returns the referenced value, throwing if empty.
        /// \throws std::bad_optional_access if this holds no reference
        [[nodiscard]] constexpr auto value() const -> T&
        {
                return storage.value().get();
        }

        // -----------------------------------------------------------------
        // Unchecked access — UB if empty, mirrors raw pointer contract
        // -----------------------------------------------------------------

        /// \brief Unchecked dereference. UB if empty.
        [[nodiscard]] constexpr auto operator*() const noexcept -> T&
        {
                return storage->get();
        }

        /// \brief Unchecked pointer-style access. UB if empty.
        [[nodiscard]] constexpr auto operator->() const noexcept -> T*
        {
                return &storage->get();
        }

        // -----------------------------------------------------------------
        // value_or — returns reference, not value copy
        // -----------------------------------------------------------------

        /// \brief Returns the referenced value, or \p fallback if empty.
        [[nodiscard]] constexpr auto value_or(T& fallback) const noexcept -> T&
        {
                return has_value() ? storage->get() : fallback;
        }

        /// \brief Deleted — rvalue fallback would produce a dangling ref
        auto value_or(T&&) const -> T& = delete;

        // -----------------------------------------------------------------
        // Swap
        // -----------------------------------------------------------------
        constexpr auto swap(optional_ref& other) noexcept -> void
        {
                storage.swap(other.storage);
        }

    private:
        // Allows optional_ref<const T> to reach get_unchecked() on
        // optional_ref<T> during const-conversion construction.
        friend class optional_ref<const T>;

        /// \brief Unchecked raw access for use in converting constructor.
        [[nodiscard]] constexpr auto get_unchecked() const noexcept -> T&
        {
                return storage->get();
        }
};

// -------------------------------------------------------------------------
// Comparisons
// -------------------------------------------------------------------------

/// \brief Equality: compares referred-to values, or both-empty state.
template<typename T, typename TU>
    requires std::equality_comparable_with<T, TU>
[[nodiscard]] constexpr auto
operator==(const optional_ref<T>& lhs, const optional_ref<TU>& rhs) noexcept
    -> bool
{
        if (lhs.has_value() != rhs.has_value()) return false;
        if (!lhs.has_value()) return true;
        return *lhs == *rhs;
}

/// \brief Three-way comparison: nullopt sorts less than any value.
template<typename T, typename TU>
    requires std::three_way_comparable_with<T, TU>
[[nodiscard]] constexpr auto
operator<=>(const optional_ref<T>& lhs, const optional_ref<TU>& rhs) noexcept
{
        if (!lhs.has_value() && !rhs.has_value())
                return std::strong_ordering::equal;
        if (!lhs.has_value()) return std::strong_ordering::less;
        if (!rhs.has_value()) return std::strong_ordering::greater;
        return *lhs <=> *rhs;
}

/// \brief Equality against nullopt.
template<typename T>
[[nodiscard]] constexpr auto
operator==(const optional_ref<T>& lhs, std::nullopt_t) noexcept -> bool
{
        return !lhs.has_value();
}

/// \brief Three-way comparison against nullopt.
template<typename T>
[[nodiscard]] constexpr auto
operator<=>(const optional_ref<T>& lhs, std::nullopt_t) noexcept
    -> std::strong_ordering
{
        return lhs.has_value() ? std::strong_ordering::greater
                               : std::strong_ordering::equal;
}

/// \brief Equality against a bare value.
template<typename T, typename TU>
    requires std::equality_comparable_with<T, TU>
[[nodiscard]] constexpr auto
operator==(const optional_ref<T>& lhs, const TU& rhs) noexcept -> bool
{
        return lhs.has_value() && *lhs == rhs;
}

/// \brief Three-way comparison against a bare value.
template<typename T, typename TU>
    requires std::three_way_comparable_with<T, TU>
[[nodiscard]] constexpr auto
operator<=>(const optional_ref<T>& lhs, const TU& rhs) noexcept
{
        return lhs.has_value() ? *lhs <=> rhs : std::strong_ordering::less;
}

// -------------------------------------------------------------------------
// Free functions
// -------------------------------------------------------------------------

template<typename T>
constexpr auto swap(optional_ref<T>& a, optional_ref<T>& b) noexcept -> void
{
        a.swap(b);
}

/// \brief Convenience factory — deduces T from the argument.
template<typename T>
[[nodiscard]] constexpr auto make_optional_ref(T& obj) noexcept
    -> optional_ref<T>
{
        return optional_ref<T>(obj);
}

}  // namespace IOCore

// vim: set  noexpandtab tabstop=8 shiftwidth=8 :
