
#include <sstream>
#include <iomanip>

namespace utility {
    class NonCopyable
    {
    protected:
        NonCopyable() = default;
        ~NonCopyable() = default;
        
    private:
        NonCopyable(NonCopyable &&) = default;
        NonCopyable(const NonCopyable &) = delete;
        NonCopyable& operator=(NonCopyable &&) = default;
        NonCopyable& operator=(const NonCopyable &) = delete;
    };

    class NonMovable
    {
    protected:
        NonMovable() = default;
        ~NonMovable() = default;
        
    private:
        NonMovable(NonMovable &&) = delete;
        NonMovable(const NonMovable &) = default;
        NonMovable& operator=(NonMovable &&) = delete;
        NonMovable& operator=(const NonMovable &) = default;
    };

    template <typename = void> std::istream &expect(std::istream &stream) {
        return stream;
    }
    template <char Ch, char... Chs> std::istream &expect(std::istream &stream) {
        (stream >> std::ws).peek() == Ch ? (void)stream.ignore() : stream.setstate(std::ios_base::failbit);
        return expect<Chs...>(stream);
    }
}
