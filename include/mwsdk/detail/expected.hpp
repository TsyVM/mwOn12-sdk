// SPDX-License-Identifier: MIT
//
// mwsdk/detail/expected.hpp
// -------------------------
// Portability shim for std::expected (C++23, P0323).
//
// The Engineering Constitution mandates explicit error handling with no global
// state, and MWSDK follows VanHooks in expressing that with std::expected.
// On a conforming C++23 standard library MWSDK uses std::expected directly.
// When the standard header is unavailable (e.g. libstdc++ < 12) this file
// provides a minimal, standards-faithful subset with the same names and
// semantics so the rest of the SDK compiles unchanged.
//
// This shim implements only the subset MWSDK uses: construction from a value or
// an mwsdk::unexpected, has_value()/operator bool, value(), error(),
// value_or(), and the monadic and_then()/transform()/transform_error().
// It is intentionally small — the moment <expected> exists, it is used instead.

#ifndef MWSDK_DETAIL_EXPECTED_HPP
#define MWSDK_DETAIL_EXPECTED_HPP

#if defined(__has_include)
#  if __has_include(<expected>) && defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
#    define MWSDK_HAS_STD_EXPECTED 1
#  endif
#endif

#ifdef MWSDK_HAS_STD_EXPECTED

#include <expected>

namespace mwsdk {
template <class T, class E> using expected = std::expected<T, E>;
template <class E>          using unexpected = std::unexpected<E>;
} // namespace mwsdk

#else // ---- fallback implementation ----------------------------------------

#include <type_traits>
#include <utility>
#include <exception>

namespace mwsdk {

template <class E>
class unexpected {
public:
    constexpr explicit unexpected(E e) : error_(std::move(e)) {}
    constexpr const E&  error() const& noexcept { return error_; }
    constexpr E&        error()      & noexcept { return error_; }
    constexpr E&&       error()     && noexcept { return std::move(error_); }
private:
    E error_;
};

template <class E> unexpected(E) -> unexpected<E>;

// Primary template: T is a real value type.
template <class T, class E>
class expected {
public:
    using value_type = T;
    using error_type = E;

    constexpr expected() : has_(true), val_() {}
    constexpr expected(const T& v) : has_(true), val_(v) {}
    constexpr expected(T&& v) : has_(true), val_(std::move(v)) {}

    template <class G>
    constexpr expected(unexpected<G> u) : has_(false), err_(std::move(u.error())) {}

    constexpr expected(const expected& o) : has_(o.has_) {
        if (has_) new (&val_) T(o.val_); else new (&err_) E(o.err_);
    }
    constexpr expected(expected&& o) noexcept : has_(o.has_) {
        if (has_) new (&val_) T(std::move(o.val_)); else new (&err_) E(std::move(o.err_));
    }
    constexpr expected& operator=(expected o) noexcept {
        destroy();
        has_ = o.has_;
        if (has_) new (&val_) T(std::move(o.val_)); else new (&err_) E(std::move(o.err_));
        return *this;
    }
    ~expected() { destroy(); }

    constexpr bool has_value() const noexcept { return has_; }
    constexpr explicit operator bool() const noexcept { return has_; }

    constexpr const T&  value() const& { return val_; }
    constexpr T&        value()      & { return val_; }
    constexpr T&&       value()     && { return std::move(val_); }

    constexpr const T&  operator*() const& noexcept { return val_; }
    constexpr T&        operator*()      & noexcept { return val_; }
    constexpr const T*  operator->() const noexcept { return &val_; }
    constexpr T*        operator->()       noexcept { return &val_; }

    constexpr const E&  error() const& noexcept { return err_; }
    constexpr E&        error()      & noexcept { return err_; }

    template <class U>
    constexpr T value_or(U&& d) const& { return has_ ? val_ : static_cast<T>(std::forward<U>(d)); }

    template <class F> constexpr auto and_then(F&& f) const& {
        using Ret = std::invoke_result_t<F, const T&>;
        return has_ ? std::forward<F>(f)(val_) : Ret(unexpected<E>(err_));
    }
    template <class F> constexpr auto transform(F&& f) const& {
        using U = std::remove_cv_t<std::invoke_result_t<F, const T&>>;
        return has_ ? expected<U, E>(std::forward<F>(f)(val_)) : expected<U, E>(unexpected<E>(err_));
    }
    template <class F> constexpr auto transform_error(F&& f) const& {
        using G = std::remove_cv_t<std::invoke_result_t<F, const E&>>;
        return has_ ? expected<T, G>(val_) : expected<T, G>(unexpected<G>(std::forward<F>(f)(err_)));
    }

private:
    void destroy() { if (has_) val_.~T(); else err_.~E(); }
    bool has_;
    union { T val_; E err_; };
};

// Specialisation: expected<void, E>.
template <class E>
class expected<void, E> {
public:
    using value_type = void;
    using error_type = E;

    constexpr expected() : has_(true) {}
    template <class G>
    constexpr expected(unexpected<G> u) : has_(false), err_(std::move(u.error())) {}

    constexpr bool has_value() const noexcept { return has_; }
    constexpr explicit operator bool() const noexcept { return has_; }
    constexpr void value() const {}
    constexpr const E& error() const& noexcept { return err_; }

    template <class F> constexpr auto and_then(F&& f) const& {
        using Ret = std::invoke_result_t<F>;
        return has_ ? std::forward<F>(f)() : Ret(unexpected<E>(err_));
    }
    template <class F> constexpr auto transform_error(F&& f) const& {
        using G = std::remove_cv_t<std::invoke_result_t<F, const E&>>;
        return has_ ? expected<void, G>() : expected<void, G>(unexpected<G>(std::forward<F>(f)(err_)));
    }

private:
    bool has_;
    E    err_{};
};

} // namespace mwsdk

#endif // MWSDK_HAS_STD_EXPECTED
#endif // MWSDK_DETAIL_EXPECTED_HPP
