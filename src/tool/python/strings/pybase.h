#pragma once

// These two headers are part of the Python C Extension API
#include <Python.h>
#include <structmember.h>

#include <winrt/base.h>

namespace winrt::impl
{
    // Bug 19167653: C++/WinRT missing category for AsyncStatus and CollectionChange. 
    // The following lines can be removed after 19167653 is fixed
    template <> struct category<winrt::Windows::Foundation::AsyncStatus> { using type = enum_category; };
    template <> struct category<winrt::Windows::Foundation::Collections::CollectionChange> { using type = enum_category; };
}

namespace py
{
    template <typename T, typename = std::void_t<>>
    struct empty_instance
    {
        static T get() { return T{}; }
    };


    template <typename T>
    struct empty_instance<T, std::void_t<decltype(T{nullptr})>>
    {
        static T get() { return T{nullptr}; }
    };

    struct gil_state_traits
    {
        using type = PyGILState_STATE;

        static void close(type value) noexcept
        {
            PyGILState_Release(value);
        }

        static constexpr type invalid() noexcept
        {
            return static_cast<PyGILState_STATE>(0);
        }
    };

    template <typename Category>
    struct pinterface_checker
    {
        static constexpr bool value = false;
    };

    template <typename... Args>
    struct pinterface_checker<winrt::impl::pinterface_category<Args...>>
    {
        static constexpr bool value = true;
    };

    template <typename Category>
    struct struct_checker
    {
        static constexpr bool value = false;
    };

    template <typename... Args>
    struct struct_checker<winrt::impl::struct_category<Args...>>
    {
        static constexpr bool value = true;
    };

    template <typename T>
    struct pinterface_python_type
    {
        using abstract = void;
        using concrete = void;
    };

    template <typename T>
    struct delegate_python_type
    {
        using type = void;
    };

    template<typename T>
    constexpr bool is_basic_category_v = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::basic_category>;

    template<typename T>
    constexpr bool is_class_category_v = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::class_category>;

    template<typename T>
    constexpr bool is_delegate_category_v = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::delegate_category>;

    template<typename T>
    constexpr bool is_pdelegate_category_v = !std::is_base_of_v<winrt::Windows::Foundation::IInspectable, T> && pinterface_checker<typename winrt::impl::category<T>::type>::value;

    template<typename T>
    constexpr bool is_enum_category_v = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::enum_category>;

    template<typename T>
    constexpr bool is_interface_category_v = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::interface_category>;

    template<typename T>
    constexpr bool is_pinterface_category_v = std::is_base_of_v<winrt::Windows::Foundation::IInspectable, T> && pinterface_checker<typename winrt::impl::category<T>::type>::value;

    template<typename T>
    constexpr bool is_struct_category_v = struct_checker<typename winrt::impl::category<T>::type>::value;

    struct winrt_wrapper_base
    {
        PyObject_HEAD;

        // PyObject_New doesn't call type's constructor, so manually manage the "virtual" get_unknown function
        winrt::Windows::Foundation::IUnknown const&(*get_unknown)(winrt_wrapper_base* self);
    };

    template<typename T>
    struct winrt_struct_wrapper
    {
        PyObject_HEAD;
        T obj{ };
    };

    template<typename T>
    struct winrt_wrapper : winrt_wrapper_base
    {
        T obj{ nullptr };

        static winrt::Windows::Foundation::IUnknown const& fetch_unknown(winrt_wrapper_base* self)
        {
            return reinterpret_cast<winrt_wrapper<T>*>(self)->obj;
        }
    };

    template<typename T>
    struct winrt_pinterface_wrapper : winrt_wrapper_base
    {
        std::unique_ptr<T> obj{ nullptr };

        static winrt::Windows::Foundation::IUnknown const& fetch_unknown(winrt_wrapper_base* self)
        {
            return reinterpret_cast<winrt_pinterface_wrapper<T>*>(self)->obj->get_unknown();
        }
    };

    template<typename To>
    To as(winrt_wrapper_base* wrapper)
    {
        return wrapper->get_unknown(wrapper).as<To>();
    }

    struct winrt_base;

    template<typename T>
    struct winrt_type
    {
        static PyTypeObject* get_python_type()
        {
            return nullptr;
        }
    };

    template<>
    struct winrt_type<winrt_base>
    {
        static PyTypeObject* python_type;
    };

    template<typename T>
    PyTypeObject* get_python_type()
    {
        if constexpr (is_pinterface_category_v<T>)
        {
            return winrt_type<pinterface_python_type<T>::abstract>::get_python_type();
        }
        else
        {
            return winrt_type<T>::get_python_type();
        }
    }

    inline __declspec(noinline) PyObject* to_PyErr() noexcept
    {
        try
        {
            throw;
        }
        catch (winrt::hresult_error const& e)
        {
            PyErr_SetString(PyExc_RuntimeError, winrt::to_string(e.message()).c_str());
        }
        catch (std::bad_alloc const&)
        {
            return PyErr_NoMemory();
        }
        catch (std::out_of_range const& e)
        {
            PyErr_SetString(PyExc_IndexError, e.what());
        }
        catch (std::invalid_argument const& e)
        {
            PyErr_SetString(PyExc_TypeError, e.what());
        }
        catch (std::exception const& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        }

        return nullptr;
    }

    void wrapped_instance(std::size_t key, PyObject* obj);
    PyObject* wrapped_instance(std::size_t key);

    template<typename T>
    inline auto get_instance_hash(T const& instance)
    {
        return std::hash<winrt::Windows::Foundation::IUnknown>{}(instance);
    }

    template<typename T>
    PyObject* wrap_struct(T instance, PyTypeObject* type_object)
    {
        if (!type_object)
        {
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }

        auto py_instance = PyObject_New(py::winrt_struct_wrapper<T>, type_object);

        if (!py_instance)
        {
            return nullptr;
        }

        // PyObject_New doesn't call type's constructor, so manually initialize the wrapper's fields
        std::memset(&(py_instance->obj), 0, sizeof(py_instance->obj));
        py_instance->obj = instance;

        return reinterpret_cast<PyObject*>(py_instance);
    }

    template<typename T>
    PyObject* wrap(T instance, PyTypeObject* type_object)
    {
        if (!instance)
        {
            Py_RETURN_NONE;
        }

        if (!type_object)
        {
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }

        auto py_instance = PyObject_New(py::winrt_wrapper<T>, type_object);

        if (!py_instance)
        {
            return nullptr;
        }

        // PyObject_New doesn't call type's constructor, so manually initialize the wrapper's fields
        py_instance->get_unknown = &winrt_wrapper<T>::fetch_unknown;
        std::memset(&(py_instance->obj), 0, sizeof(py_instance->obj));
        py_instance->obj = instance;

        wrapped_instance(get_instance_hash(instance), reinterpret_cast<PyObject*>(py_instance));

        return reinterpret_cast<PyObject*>(py_instance);
    }

    template<typename T>
    PyObject* wrap_pinterface(T instance)
    {
        if (!instance)
        {
            Py_RETURN_NONE;
        }

        using ptype = pinterface_python_type<T>;

        PyTypeObject* type_object = get_python_type<T>();

        if (!type_object)
        {
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }

        auto py_instance = PyObject_New(py::winrt_pinterface_wrapper<ptype::abstract>, type_object);

        if (!py_instance)
        {
            return nullptr;
        }

        // PyObject_New doesn't call type's constructor, so manually initialize the wrapper's fields
        py_instance->get_unknown = &winrt_pinterface_wrapper<ptype::abstract>::fetch_unknown;
        std::memset(&(py_instance->obj), 0, sizeof(py_instance->obj));
        py_instance->obj = std::make_unique<ptype::concrete>(instance);

        wrapped_instance(get_instance_hash(instance), reinterpret_cast<PyObject*>(py_instance));

        return reinterpret_cast<PyObject*>(py_instance);
    }

    template<typename T>
    PyObject* wrap(T instance) noexcept
    {
        if (!instance)
        {
            Py_RETURN_NONE;
        }

        if constexpr (is_class_category_v<T> || is_interface_category_v<T>)
        {
            return wrap<T>(instance, get_python_type<T>());
        }
        else
        {
            if constexpr (std::is_same_v<pinterface_python_type<T>::abstract, void>)
            {
                PyErr_SetNone(PyExc_NotImplementedError);
                return nullptr;
            }
            else
            {
                return wrap_pinterface<T>(instance);
            }
        }
    }

    inline void throw_if_pyobj_null(PyObject* obj)
    {
        if (!obj)
        {
            throw winrt::hresult_invalid_argument();
        }
    }

    template <typename T, typename = void>
    struct converter
    {
        static PyObject* convert(T value) noexcept
        {
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }

        static T convert_to(PyObject* obj)
        {
            throw winrt::hresult_not_implemented();
        }
    };

    template <>
    struct converter<bool>
    {
        static PyObject* convert(bool value) noexcept
        {
            return PyBool_FromLong(value ? 1 : 0);
        }

        static bool convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            auto result = PyObject_IsTrue(obj);

            if (result == -1)
            {
                throw winrt::hresult_invalid_argument();
            }

            return result > 0;
        }
    };

    template <>
    struct converter<int8_t>
    {
        static PyObject* convert(int8_t value) noexcept
        {
            return PyLong_FromLong(static_cast<int32_t>(value));
        }

        static int8_t convert_to(PyObject* obj)
        {
            int32_t result = PyLong_AsLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            if (result < INT8_MIN || result > INT8_MAX)
            {
                throw winrt::hresult_invalid_argument();
            }

            return static_cast<int8_t>(result);
        }
    };

    template <>
    struct converter<uint8_t>
    {
        static PyObject* convert(uint8_t value) noexcept
        {
            return PyLong_FromLong(static_cast<int32_t>(value));
        }

        static uint8_t convert_to(PyObject* obj)
        {
            int32_t result = PyLong_AsLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            if (result < 0 || result > UINT8_MAX)
            {
                throw winrt::hresult_invalid_argument();
            }

            return static_cast<uint8_t>(result);
        }
    };

    template <>
    struct converter<int16_t>
    {
        static PyObject* convert(int16_t value) noexcept
        {
            return PyLong_FromLong(static_cast<int32_t>(value));
        }

        static int16_t convert_to(PyObject* obj)
        {
            int32_t result = PyLong_AsLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            if (result < INT16_MIN || result > INT16_MAX)
            {
                throw winrt::hresult_invalid_argument();
            }

            return static_cast<int16_t>(result);
        }
    };

    template <>
    struct converter<uint16_t>
    {
        static PyObject* convert(uint16_t value) noexcept
        {
            return PyLong_FromLong(static_cast<int32_t>(value));
        }

        static uint16_t convert_to(PyObject* obj)
        {
            int32_t result = PyLong_AsLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            if (result < 0 || result > UINT16_MAX)
            {
                throw winrt::hresult_invalid_argument();
            }

            return static_cast<uint16_t>(result);
        }
    };

    template <>
    struct converter<int32_t>
    {
        static PyObject* convert(int32_t value) noexcept
        {
            return PyLong_FromLong(value);
        }

        static int32_t convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            auto result = PyLong_AsLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            return result;
        }
    };

    template <>
    struct converter<uint32_t>
    {
        static PyObject* convert(uint32_t value) noexcept
        {
            return PyLong_FromUnsignedLong(value);
        }

        static uint32_t convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            auto result = PyLong_AsUnsignedLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            return result;
        }
    };

    template <>
    struct converter<int64_t>
    {
        static PyObject* convert(int64_t value) noexcept
        {
            return PyLong_FromLongLong(value);
        }

        static int64_t convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            auto result = PyLong_AsLongLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            return result;
        }
    };

    template <>
    struct converter<uint64_t>
    {
        static PyObject* convert(uint64_t value) noexcept
        {
            return PyLong_FromUnsignedLongLong(value);
        }

        static uint64_t convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            auto result = PyLong_AsUnsignedLongLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            return result;
        }
    };

    template <>
    struct converter<float>
    {
        static PyObject* convert(float value) noexcept
        {
            return PyFloat_FromDouble(value);
        }

        static float convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            auto result = PyFloat_AsDouble(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            return static_cast<float>(result);
        }
    };

    template <>
    struct converter<double>
    {
        static PyObject* convert(double value) noexcept
        {
            return PyFloat_FromDouble(value);
        }

        static double convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            auto result = PyFloat_AsDouble(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw winrt::hresult_invalid_argument();
            }

            return result;
        }
    };

    template <>
    struct converter<winrt::Windows::Foundation::IInspectable>
    {
        static PyObject* convert(winrt::Windows::Foundation::IInspectable const& value) noexcept
        {
            return wrap<winrt::Windows::Foundation::IInspectable>(value, winrt_type<winrt_base>::python_type);
        }

        static winrt::Windows::Foundation::IInspectable convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            if (PyObject_IsInstance(obj, reinterpret_cast<PyObject*>(winrt_type<winrt_base>::python_type)))
            {
                auto wrapper = reinterpret_cast<winrt_wrapper_base*>(obj);
                return as<winrt::Windows::Foundation::IInspectable>(wrapper);
            }

            throw winrt::hresult_invalid_argument();
        }
    };

    struct pystring
    {
        wchar_t* buffer{ nullptr };
        std::wstring_view::size_type size{};

        explicit pystring(PyObject* obj)
        {
            Py_ssize_t py_size;
            buffer = PyUnicode_AsWideCharString(obj, &py_size);
            if (buffer != nullptr)
            {
                size = static_cast<std::wstring_view::size_type>(py_size);
            }
        }

        pystring(pystring& other) = delete;
        pystring& operator=(pystring const&) = delete;

        pystring(pystring&& other) noexcept : buffer(other.buffer), size(other.size)
        {
            other.buffer = nullptr;
        }

        pystring& operator=(pystring&& rhs)
        {
            std::swap(buffer, rhs.buffer);
            size = rhs.size;
        }

        operator bool() const noexcept
        {
            return buffer != nullptr;
        }

        ~pystring()
        {
            PyMem_Free(buffer);
        }
    };

    struct pystringview : public pystring, public std::wstring_view
    {
        explicit pystringview(PyObject* obj) : pystring(obj), std::wstring_view(pystring::buffer, pystring::size)
        {
        }
    };

    template <>
    struct converter<winrt::hstring>
    {
        static PyObject* convert(winrt::hstring const& value) noexcept
        {
            return PyUnicode_FromWideChar(value.c_str(), value.size());
        }

        static pystringview convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            pystringview str{ obj };

            if (!str)
            {
                throw winrt::hresult_invalid_argument();
            }

            return str;
        }
    };

    template <typename T>
    struct converter<T, typename std::enable_if_t<is_enum_category_v<T>>>
    {
        static PyObject* convert(T instance) noexcept
        {
            using enum_type = std::underlying_type_t<T>;
            return converter<enum_type>::convert(static_cast<enum_type>(instance));
        }

        static auto convert_to(PyObject* obj)
        {
            using enum_type = std::underlying_type_t<T>;
            throw_if_pyobj_null(obj);
            return static_cast<T>(converter<enum_type>::convert_to(obj));
        }
    };

    template <typename T>
    struct converter<T, typename std::enable_if_t<is_class_category_v<T>>>
    {
        static PyObject* convert(T const& instance) noexcept
        {
            return wrap(instance);
        }

        static auto convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            if (Py_TYPE(obj) != get_python_type<T>())
            {
                throw winrt::hresult_invalid_argument();
            }

            return reinterpret_cast<winrt_wrapper<T>*>(obj)->obj;
        }
    };

    template <typename T>
    struct python_iterable final :
        winrt::implements<python_iterable<T>, winrt::Windows::Foundation::Collections::IIterable<T>>
    {
        PyObject* _iterable;

        explicit python_iterable(PyObject* iterable) : _iterable(iterable)
        {
        }

        auto First() const
        {
            return winrt::make<iterator>(PyObject_GetIter(_iterable));
        }

    private:
        struct iterator final : winrt::implements<iterator, winrt::Windows::Foundation::Collections::IIterator<T>>
        {
            PyObject* _iterator;
            std::optional<T> _current_value;

            static std::optional<T> get_next(PyObject* iterator)
            {
                if (iterator == nullptr)
                {
                    throw winrt::hresult_invalid_argument();
                }

                PyObject* next = PyIter_Next(iterator);
                if (next == nullptr)
                {
                    if (PyErr_Occurred())
                    {
                        // TODO: propagate Python error
                        throw winrt::hresult_invalid_argument();
                    }
                    else
                    {
                        return {};
                    }
                }

                return std::move(std::optional<T>{ converter<T>::convert_to(next) });
            }

            iterator(PyObject* i) : _iterator(i)
            {
                if (_iterator == nullptr)
                {
                    throw winrt::hresult_invalid_argument();
                }

                _current_value = get_next(_iterator);
            }

            auto Current() const
            {
                return _current_value.value();
            }

            bool HasCurrent() const
            {
                return _current_value.has_value();
            }

            bool MoveNext()
            {
                _current_value = get_next(_iterator);
                return _current_value.has_value();
            }

            uint32_t GetMany(winrt::array_view<T> values)
            {
                // TODO: implement GetMany
                throw winrt::hresult_not_implemented();
            }
        };
    };

    template <typename T>
    std::optional<T> convert_interface_to(PyObject* obj)
    {
        throw_if_pyobj_null(obj);

        if (Py_TYPE(obj) == get_python_type<T>())
        {
            return reinterpret_cast<winrt_wrapper<T>*>(obj)->obj;
        }

        if (PyObject_IsInstance(obj, reinterpret_cast<PyObject*>(winrt_type<winrt_base>::python_type)))
        {
            auto wrapper = reinterpret_cast<winrt_wrapper_base*>(obj);
            return as<T>(wrapper);
        }

        return {};
    }

    // TODO: specalization for Python Sequence Protocol -> IVector[View]
    // TODO: specalization for Python Mapping Protocol -> IMap[View]

    template <typename TItem>
    struct converter<winrt::Windows::Foundation::Collections::IIterable<TItem>>
    {
        using TCollection = winrt::Windows::Foundation::Collections::IIterable<TItem>;

        static PyObject* convert(TCollection const& instance) noexcept
        {
            return wrap(instance);
        }

        static auto convert_to(PyObject* obj)
        {
            if (auto result = convert_interface_to<TCollection>(obj))
            {
                return result.value();
            }

            if (PyObject* iterator = PyObject_GetIter(obj))
            {
                return winrt::make<python_iterable<TItem>>(obj);
            }

            throw winrt::hresult_invalid_argument();
        }
    };

    template <typename T>
    struct is_specalized_interface : std::false_type {};

    template <typename T>
    inline constexpr bool is_specalized_interface_v = is_specalized_interface<T>::value;

    template <typename TItem>
    struct is_specalized_interface<winrt::Windows::Foundation::Collections::IIterable<TItem>> : std::true_type {};

    template <typename T>
    struct converter<T, typename std::enable_if_t<(is_interface_category_v<T> || is_pinterface_category_v<T>) && !is_specalized_interface_v<T>>>
    {
        static PyObject* convert(T const& instance) noexcept
        {
            return wrap(instance);
        }

        static auto convert_to(PyObject* obj)
        {
            if (auto result = convert_interface_to<T>(obj))
            {
                return result.value();
            }

            throw winrt::hresult_invalid_argument();
        }
    };

    template <typename T>
    struct converter<T, typename std::enable_if_t<is_delegate_category_v<T> || is_pdelegate_category_v<T>>>
    {
        static PyObject* convert(T const& instance) noexcept
        {
            // TODO: support converting delegates
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }

        static auto convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);
            return delegate_python_type<T>::type::get(obj);
        }
    };

    template <typename T>
    struct converter<winrt::com_array<T>>
    {
        static PyObject* convert(winrt::com_array<T> const& instance) noexcept
        {
            PyObject* list = PyList_New(instance.size());
            if (list == nullptr)
            {
                return nullptr;
            }

            for (uint32_t index = 0; index < instance.size(); index++)
            {
                PyObject* item = converter<T>::convert(instance[index]);
                if (item == nullptr)
                {
                    return nullptr;
                }

                if (PyList_SetItem(list, index, item) == -1)
                {
                    return nullptr;
                }
            }

            return list;
        }

        static auto convert_to(PyObject* obj)
        {
            if (!obj || !PyList_Check(obj))
            {
                throw winrt::hresult_invalid_argument();
            }

            Py_ssize_t list_size = PyList_Size(obj);
            if (list_size == -1)
            {
                // TODO: propagate python error 
                throw winrt::hresult_invalid_argument();
            }

            winrt::com_array<T> items(static_cast<uint32_t>(list_size), empty_instance<T>::get());

            for (Py_ssize_t index = 0; index < list_size; index++)
            {
                PyObject* item = PyList_GetItem(obj, index);
                if (item == nullptr)
                {
                    // TODO: propagate python error 
                    throw winrt::hresult_invalid_argument();
                }

                items[static_cast<uint32_t>(index)] = converter<T>::convert_to(item);
            }

            return std::move(items);
        }
    };

    template<typename T>
    PyObject* convert(T const& instance)
    {
        return converter<T>::convert(instance);
    }

    template<typename T>
    auto convert_to(PyObject* value)
    {
        return converter<T>::convert_to(value);
    }

    template<typename T>
    auto convert_to(PyObject* args, int index)
    {
        return convert_to<T>(PyTuple_GetItem(args, index));
    }

    template <typename Async>
    PyObject* get_results(Async const& operation)
    {
        if constexpr (std::is_void_v<decltype(operation.GetResults())>)
        {
            operation.GetResults();
            Py_RETURN_NONE;
        }
        else
        {
            return convert(operation.GetResults());
        }
    }

    template <typename Async>
    PyObject* dunder_await(Async const& async)
    {
        PyObject* loop = PyObject_CallMethod(PyImport_ImportModule("asyncio"), "get_event_loop", nullptr);
        if (!loop)
        {
            return nullptr;
        }

        PyObject* future = PyObject_CallMethod(loop, "create_future", nullptr);
        if (!future)
        {
            return nullptr;
        }

        try
        {
            async.Completed([loop, future](Async const& operation, winrt::Windows::Foundation::AsyncStatus status)
            {
                winrt::handle_type<py::gil_state_traits> gil_state{ PyGILState_Ensure() };

                if (status == winrt::Windows::Foundation::AsyncStatus::Completed)
                {
                    // result = operation.GetResults()
                    PyObject* results = get_results(operation);

                    // loop.call_soon_threadsafe(future.set_result, results)
                    PyObject_CallMethod(loop, "call_soon_threadsafe", "OO", PyObject_GetAttrString(future, "set_result"), results);
                }
                else
                {
                    // loop.call_soon_threadsafe(future.set_exception, RuntimeError("AsyncOp failed"))
                    PyObject_CallMethod(loop, "call_soon_threadsafe", "OO", PyObject_GetAttrString(future, "set_exception"), PyExc_RuntimeError);
                }

                Py_DECREF(future);
                Py_DECREF(loop);
            });
        }
        catch (...)
        {
            return py::to_PyErr();
        }

        return PyObject_GetIter(future);
    }

}
