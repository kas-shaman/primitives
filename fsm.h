#pragma once
#include <functional>

namespace utility {
    struct FSMState {
        std::function<void(float)> update;
        std::function<void()> entered;
        std::function<void()> leaving;
    };

    template <typename NameType> class FSM {
        //template <typename T, typename = decltype(T::_count)> bool constexpr _hasCount() {
        //    return true;
        //}
        //template <typename T> bool constexpr _hasCount() {
        //    return false;
        //}

        //static_assert(std::is_enum_v<NameType>, "NameType must be enum class with _count member");
        //static_assert(FSM::_hasCount<NameType>(), "NameType must be enum class with _count member");

    public:
        FSM() {}
        FSM(FSM &&other) {
            *this = std::move(other);
        }

        FSM &operator =(FSM &&other) {
            for (std::size_t i = 0; i < std::size_t(NameType::_count); i++) {
                _states[i] = std::move(other._states[i]);
            }

            other._current = other._states;
        }

        void startWithState(const NameType &name) {
            _current = &_states[std::size_t(name)];

            if (_current->entered) {
                _current->entered();
            }
        }

        void changeState(const NameType &name) {
            if (&_states[std::size_t(name)] != _current) {
                if (_current->leaving) {
                    _current->leaving();
                }

                _current = &_states[std::size_t(name)];

                if (_current->entered) {
                    _current->entered();
                }
            }
        }

        void update(float dtMs) {
            if (_current->update) {
                _current->update(dtMs);
            }
        }

        bool isState(const NameType &name) const {
            return _current == _states + std::size_t(name);
        }

        FSMState &operator[](const NameType &name) {
            return _states[std::size_t(name)];
        }

    private:
        FSMState _states[std::size_t(NameType::_count)];
        FSMState *_current = _states;

        FSM(const FSM &) = delete;
        FSM &operator =(const FSM &) = delete;
    };
}