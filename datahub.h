#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include <string>
#include <strstream>
#include <cmath>

#ifdef _MSC_VER
    #define strcasecmp _stricmp
#endif

#define datahubscope(name, ...) struct name : datahub::details::MovableBase __VA_ARGS__ name;
#define datahubarray(structname, arrayname, ...) struct structname : datahub::details::MovableBase __VA_ARGS__; datahub::details::ObservableArray<structname> arrayname;
#define datahubvalue(name, ...) datahub::details::ObservableValue<decltype(__VA_ARGS__)> name = __VA_ARGS__;
#define datahubevent(name, ...) datahub::details::EventHandler<__VA_ARGS__> name;

namespace datahub {
    namespace details {
        class MovableBase {
        protected:
            MovableBase() = default;
            ~MovableBase() = default;

        protected:
            MovableBase(MovableBase &&) = default;
            MovableBase(const MovableBase &) = default;
            MovableBase &operator =(MovableBase &&) = delete;
            MovableBase &operator =(const MovableBase &) = delete;
        };
        
        template<typename> class ObservableArray;
        template<typename, typename> class ObservableValue;
    }

    typedef struct {} * EventToken;
    typedef struct {} * ArrayToken;

    namespace details {
        template<typename> class EventHandler final : details::MovableBase {};
        template<typename... Args> class EventHandler<void(Args...)> final : details::MovableBase {
            template<typename> friend class details::ObservableArray;
            template<typename, typename> friend class details::ObservableValue;

        public:
            template<typename L, void(L::*)(Args...) const = &L::operator()> EventToken operator+=(L &&lambda) {
                EventToken token = reinterpret_cast<EventToken>(_handlers.size());
                return _handlers.emplace_back(_handlers.size(), std::move(lambda)), token;
            }
            void operator-=(EventToken id) {
                _handlers.erase(std::remove_if(std::begin(_handlers), std::end(_handlers), [id](const auto &item) {
                    return item.first == reinterpret_cast<std::size_t>(id);
                }), std::end(_handlers));
            }
            template <typename... CallArgs> void call(CallArgs&&... args) {
                for (const auto &handler : _handlers) {
                    handler.second(std::forward<CallArgs>(args)...);
                }
            }

        private:
            std::vector<std::pair<std::size_t, std::function<void(Args...)>>> _handlers;
        };

        template<typename Derived> class ObservableArray final : MovableBase {
        public:
            EventHandler<void(ArrayToken, Derived &)> onArrayElementAdded;
            EventHandler<void(ArrayToken)> onArrayElementRemoving;
            
            template<typename L, void(L::*)(Derived &) const = &L::operator()> ArrayToken add(L &&initializer) {
                ArrayToken result = reinterpret_cast<ArrayToken>(_uniqueid++);
                typename std::unordered_map<ArrayToken, Derived>::iterator position = _data.emplace(result, Derived{}).first;
                initializer(position->second);
                onArrayElementAdded.call(result, position->second);
                return result;
            }
            
            void remove(ArrayToken id) {
                onArrayElementRemoving.call(id);
                _data.erase(id);
            }
            
            Derived &operator[](ArrayToken id) {
                return _data[id];
            }

            template<typename L, void(L::*)(ArrayToken, Derived &) const = &L::operator()> void foreach(L &&functor) {
                for (auto it = _data.begin(); it != _data.end(); ++it) {
                    functor(it->first, it->second);
                }
            }

        private:
            std::unordered_map<ArrayToken, Derived> _data;
            std::size_t _uniqueid = 0x1;
        };

        template<typename Type, typename = void> class ObservableValue : MovableBase {};
        template<typename Type> class ObservableValue<Type, std::enable_if_t<std::is_class<Type>::value || std::is_pointer<Type>::value>> : MovableBase {
        public:
            EventHandler<void(const Type &)> onValueChanged;

            template<typename AssignType> ObservableValue(AssignType&& value) {
                _data = std::forward<AssignType>(value);
            }
            template<typename AssignType> void operator =(AssignType&& value) {
                _data = std::forward<AssignType>(value);
                onValueChanged.call(_data);
            }
            operator Type &() {
                return _data;
            }
            operator const Type &() const {
                return _data;
            }
            bool operator ==(const Type &other) {
                return _data == other;
            }

        private:
            Type _data;
        };
        template<typename Type> class ObservableValue<Type, std::enable_if_t<std::is_arithmetic<Type>::value>> : MovableBase {
        public:
            EventHandler<void(Type)> onValueChanged;
        
            template<typename AssignType, typename = std::enable_if_t<std::is_arithmetic<AssignType>::value>> ObservableValue(AssignType value) {
                _data = static_cast<long double>(value);
            }
            template<typename AssignType> auto operator =(AssignType value) -> typename std::enable_if<std::is_arithmetic<AssignType>::value, void>::type {
                _data = static_cast<long double>(value);
                onValueChanged.call(static_cast<Type>(_data));
            }
            void operator =(const char *string) {
                _data = static_cast<long double>(std::strtold(string, nullptr));
                onValueChanged.call(static_cast<Type>(_data));
            }
            operator const char *() const {
                thread_local static char buffer[32];
                ::snprintf(buffer, 32, "%g", static_cast<double>(_data));
                return buffer;
            }
            template <typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>> operator T() const {
                return static_cast<T>(_data);
            }
            template<typename AssignType> auto operator ==(AssignType value) -> typename std::enable_if<std::is_arithmetic<AssignType>::value, bool>::type {
                return std::abs(static_cast<long double>(value) - _data) < std::numeric_limits<long double>::epsilon();
            }
            bool operator ==(const char *string) {
                return std::abs(static_cast<long double>(std::strtold(string, nullptr)) - _data) < std::numeric_limits<long double>::epsilon();
            }

        private:
            long double _data;
        };
        template<std::size_t N> class ObservableValue<const char(&)[N]> : MovableBase {
        public:
            EventHandler<void(const char *)> onValueChanged;
            ObservableValue(const char *string) : _data(string) {}

            template<typename AssignType> auto operator =(AssignType value) -> typename std::enable_if<std::is_arithmetic<AssignType>::value, void>::type {
                thread_local static char buffer[32];
                ::snprintf(buffer, 32, "%g", static_cast<double>(value));
                _data = buffer;
                onValueChanged.call(_data.c_str());
            }
            void operator =(const char *string) {
                _data = string;
                onValueChanged.call(_data.c_str());
            }
            void operator =(bool value) {
                _data = value ? "true" : "false";
                onValueChanged.call(_data.c_str());
            }
            operator const char *() const {
                return _data.c_str();
            }
            operator bool() const {
                return ::strcasecmp(_data.c_str(), "true") == 0;
            }
            template <typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>> operator T() const {
                T result = {};
                std::istrstream stream (_data.c_str(), _data.length());
                stream >> result;
                return result;
            }
            template<typename AssignType> auto operator ==(AssignType&& value) -> typename std::enable_if<std::is_arithmetic<AssignType>::value, bool>::type {
                long double myself = static_cast<long double>(std::strtold(_data.c_str(), nullptr));
                return std::abs(myself - static_cast<long double>(value)) < std::numeric_limits<long double>::epsilon();
            }
            bool operator ==(const char *string) {
                return ::strcmp(_data.c_str(), string) == 0;
            }
            bool operator ==(bool value) {
                return ::strcasecmp(_data.c_str(), "true") == 0;
            }

        private:
            std::string _data;
        };
        template<> class ObservableValue<bool> : MovableBase {
        public:
            EventHandler<void(bool)> onValueChanged;
            ObservableValue(bool value) : _data(value) {}

            void operator =(bool value) {
                _data = value;
            }
            void operator =(const char *string) {
                _data = ::strcasecmp(string, "true") == 0;
            }
            operator const char *() const {
                return _data ? "true" : "false";
            }
            operator bool() const {
                return _data;
            }

        private:
            bool _data;
        };
    }

    // unittest scope
    // -----------------------------------------------------------------------------------------------------------------------------------------------

    extern
    datahubscope(unittest, {
        datahubvalue(teststring, "unittest")
        datahubvalue(testvalue, 99)
        datahubvalue(testbool, true)
        
        datahubarray(ArrayElement, testarray, {
            datahubvalue(teststring, "title")
        })
    })

    // -----------------------------------------------------------------------------------------------------------------------------------------------

    void runUnitTests();
}



